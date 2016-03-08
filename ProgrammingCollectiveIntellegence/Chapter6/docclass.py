import re
import math
import sqlite3

def sampleTrain(cl):
    cl.train('Nobody owns the water.', 'good')
    cl.train('The quick rabbit jumps fences', 'good')
    cl.train('buy pharmaceuticals now', 'bad')
    cl.train('make quick money at the online casino', 'bad')
    cl.train('the quick brown fox jumps', 'good')

def getWords(doc):
    splitter = re.compile('\\W*')
    words = [word.lower() for word in splitter.split(doc)
             if len(word) > 2 and len(word) < 20]
    return dict([(word, 1) for word in words])

class classifier:
    def __init__(self, getFeatures, fileName = None):
        self.getFeatures = getFeatures

    def setDb(self, dbFile):
        self.conn = sqlite3.connect(dbFile)
        self.conn.execute('create table if not exists fc(feature, category, count)')
        self.conn.execute('create table if not exists cc(category, count)')

    def incf(self, f, cat):
        count = self.fCount(f, cat)
        if count == 0:
            self.conn.execute("insert into fc values ('%s', '%s', 1)" %
                              (f, cat))
        else:
            self.conn.execute("update fc set count=%d where feature='%s' and category='%s'" % (count + 1, f, cat))

    def incc(self, cat):
        count = self.catCount(cat)
        if count == 0:
            self.conn.execute("insert into cc values('%s', 1)" % (cat))
        else:
            self.conn.execute("update cc set count=%d where category='%s'" %
                              (count + 1, cat))

    def fCount(self, f, cat):
        res = self.conn.execute("select count from fc where feature='%s' and category='%s'" % (f, cat)).fetchone()
        if res == None: return 0
        else: return float(res[0])

    def catCount(self, cat):
        res = self.conn.execute("select count from cc where category='%s'" %
                                (cat)).fetchone()
        if res == None: return 0
        else: return float(res[0])

    def totalCount(self):
        res = self.conn.execute("select sum(count) from cc").fetchone()
        if res == None: return 0
        else: return res[0]

    def categories(self):
        cur = self.conn.execute("select category from cc")
        return [d[0] for d in cur]

    def train(self, item, cat):
        features = self.getFeatures(item)
        for f in features:
            self.incf(f, cat)
        self.incc(cat)
        self.conn.commit()

    def fProb(self, f, cat):
        if self.catCount(cat) == 0.0: return 0.0
        return self.fCount(f, cat) / self.catCount(cat)

    def weightedProb(self, f, cat, prf, weight = 1.0, ap = 0.5):
        basicProb = prf(f, cat)
        totals = sum([self.fCount(f, c) for c in self.categories()])
        return ((weight * ap) + (totals * basicProb)) / (weight + totals)

class naiveBayes(classifier):
    def __init__(self, getFeatures):
        classifier.__init__(self, getFeatures)
        self.threshold = dict()

    def docProb(self, item, cat):
        features = self.getFeatures(item)

        p = 1
        for f in features: p *= self.weightedProb(f, cat, self.fProb)
        return p

    def prob(self, item, cat):
        catProb = self.catCount(cat) / self.totalCount()
        docProb = self.docProb(item, cat)
        return docProb * catProb

    def setThreshold(self, cat, t):
        self.threshold[cat] = t

    def getThreshold(self, cat):
        if cat not in self.threshold: return 1.0
        return self.threshold[cat]

    def classify(self, item, default = None):
        probs = dict()
        max = 0.0
        for cat in self.categories():
            probs[cat] = self.prob(item, cat)
            if probs[cat] > max:
                max = probs[cat]
                best = cat

        for cat in probs:
            if cat == best: continue
            if probs[cat] * self.getThreshold(best) > probs[best]:
                return default
        return best

class fisherClassifier(classifier):
    def __init__(self, getFeatures):
        classifier.__init__(self, getFeatures)
        self.minimum = dict()

    def cProb(self, f, cat):
        clf = self.fProb(f, cat)
        if clf == 0.0: return 0.0

        freqSum = sum([self.fProb(f, c) for c in self.categories()])
        p = clf / freqSum
        return p

    def fisherProb(self, item, cat):
        p = 1.0
        features = self.getFeatures(item)
        for f in features:
            p *= self.weightedProb(f, cat, self.cProb)

        fScore = -2 * math.log(p)
        return self.invChi2(fScore, len(features) * 2)

    def invChi2(self, chi, df):
        m = chi / 2.0
        sum = term = math.exp(-m)
        for i in range(1, df // 2):
            term *= m / i
            sum += term
        return min(sum, 1.0)

    def setMinimum(self, cat, min):
        self.minimum[cat] = min

    def getMinimum(self, cat):
        if cat not in self.minimum.keys(): return 0
        return self.minimum[cat]

    def classify(self, item, default = None):
        best = default
        max = 0.0
        for c in self.categories():
            p = self.fisherProb(item, c)
            if p > self.getMinimum(c) and p > max:
                best = c
                max = p
        return best
