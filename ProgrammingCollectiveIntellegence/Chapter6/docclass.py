import re
import math

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
        self.fc = dict()
        self.cc = dict()
        self.getFeatures = getFeatures

    def incf(self, f, cat):
        self.fc.setdefault(f, {})
        self.fc[f].setdefault(cat, 0)
        self.fc[f][cat] += 1

    def incc(self, cat):
        self.cc.setdefault(cat, 0)
        self.cc[cat] += 1

    def fCount(self, f, cat):
        if f in self.fc and cat in self.fc[f]:
            return float(self.fc[f][cat])
        else:
            return 0.0

    def catCount(self, cat):
        if cat in self.cc:
            return float(self.cc[cat])
        else:
            return 0.0

    def totalCount(self):
        return sum(self.cc.values())

    def categories(self):
        return self.cc.keys()

    def train(self, item, cat):
        features = self.getFeatures(item)
        for f in features:
            self.incf(f, cat)
        self.incc(cat)

    def fprob(self, f, cat):
        if self.catCount(cat) == 0.0: return 0.0
        return self.fCount(f, cat) / self.catCount(cat)

    def weightedProb(self, f, cat, prf, weight = 1.0, ap = 0.5):
        basicProb = prf(f, cat)
        totals = sum([self.fCount(f, c) for c in self.categories()])
        return ((weight * ap) + (totals * basicProb)) / (weight + totals)
