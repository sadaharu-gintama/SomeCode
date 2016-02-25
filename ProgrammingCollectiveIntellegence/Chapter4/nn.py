from math import tanh
import sqlite3

def dtanh(y):
    return 1.0 - y * y

class searchNet:
    def __init__(self, dbname):
        self.conn = sqlite3.connect(dbname)

    def __del__(self):
        self.conn.close()

    def makeTables(self):
        self.conn.execute('create table hiddennode(create_key)')
        self.conn.execute('create table wordhidden(fromid, toid, strength)')
        self.conn.execute('create table hiddenurl(fromid, toid, strength)')
        self.conn.commit()

    def getStrength(self, fromId, toId, layer):
        if layer == 0: table = 'wordhidden'
        elif layer == 1: table = 'hiddenurl'
        res = self.conn.execute('select strength from %s where fromId=%d and toId=%d' \
                                % (table, fromId, toId)).fetchone()
        if res == None:
            if layer == 0: return -0.2
            elif layer == 1: return 0.0
        else:
            return res[0]

    def setStrength(self, fromId, toId, layer, strength):
        if layer == 0: table = 'wordhidden'
        elif layer == 1: table = 'hiddenurl'
        res = self.conn.execute('select rowid from %s where fromId=%d and toId=%d' \
                                % (table, fromId, toId)).fetchone()
        if res == None:
            self.conn.execute('insert into %s (fromid, toid, strength) values (%d,%d,%f)' \
                              % (table, fromId, toId, strength))
        else:
            rowid = res[0]
            self.conn.execute('update %s set strength=%f where rowid=%d' % \
                              (table, strength, rowid))

    def generateHiddenNode(self, wordIds, urls):
        if len(wordIds) > 3: return None

        createKey = '_'.join(sorted([str(wi) for wi in wordIds]))
        res = self.conn.execute("select rowid from hiddennode where create_key='%s'" % createKey).fetchone()

        if res == None:
            cur = self.conn.execute("insert into hiddennode (create_key) values ('%s')" % createKey)
            hiddenId = cur.lastrowid
            for wordId in wordIds:
                self.setStrength(wordId, hiddenId, 0, 1.0 / len(wordIds))
            for urlId in urls:
                self.setStrength(hiddenId, urlId, 1, 0.1)
            self.conn.commit()

    def getAllHiddenIds(self, wordIds, urlIds):
        l1 = {}
        for wordId in wordIds:
            cur = self.conn.execute(
                'select toid from wordhidden where fromid=%d' % wordId)
            for row in cur: l1[row[0]] = 1
        for urlId in urlIds:
            cur = self.conn.execute(
                'select fromid from hiddenurl where toid=%d' % urlId)
            for row in cur: l1[row[0]] = 1
        return l1.keys()

    def setupNetwork(self, wordIds, urlIds):
        self.wordIds = wordIds
        self.hiddenIds = self.getAllHiddenIds(wordIds, urlIds)
        self.urlIds = urlIds

        self.ai = [1.0] * len(self.wordIds)
        self.ah = [1.0] * len(self.hiddenIds)
        self.ao = [1.0] * len(self.urlIds)

        self.wi = [[self.getStrength(wordId, hiddenId, 0) for hiddenId in self.hiddenIds] for wordId in self.wordIds]
        self.wo = [[self.getStrength(hiddenId, urlId, 1) for urlId in self.urlIds] for hiddenId in self.hiddenIds]

    def feedForward(self):
        for i in range(len(self.wordIds)):
            self.ai[i] = 1.0

        for j in range(len(self.hiddenIds)):
            sum = 0.0
            for i in range(len(self.wordIds)):
                sum += self.ai[i] * self.wi[i][j]
            self.ah[j] = tanh(sum)

        for k in range(len(self.urlIds)):
            sum = 0.0
            for j in range(len(self.hiddenIds)):
                sum += self.ah[j] * self.wo[j][k]
            self.ao[k] = tanh(sum)

        return self.ao[:]

    def getResult(self, wordIds, urlIds):
        self.setupNetwork(wordIds, urlIds)
        return self.feedForward()

    def backPropagate(self, targets, N = 0.5):
        output_deltas = [0.0] * len(self.urlIds)
        for k in range(len(self.urlIds)):
            error = targets[k] - self.ao[k]
            output_deltas[k] = dtanh(self.ao[k]) * error

        hidden_deltas = [0.0] * len(self.hiddenIds)
        for j in range(len(self.hiddenIds)):
            error = 0.0
            for k in range(len(self.urlIds)):
                error += output_deltas[k] * self.wo[j][k]
            hidden_deltas[j] = dtanh(self.ah[j]) * error

        for j in range(len(self.hiddenIds)):
            for k in range(len(self.urlIds)):
                change = output_deltas[k] * self.ah[j]
                self.wo[j][k] += N * change

        for i in range(len(self.wordIds)):
            for j in range(len(self.hiddenIds)):
                change = hidden_deltas[j] * self.ai[i]
                self.wi[i][j] += N * change

    def trainQuery(self, wordIds, urlIds, selectedUrl):
        self.generateHiddenNode(wordIds, urlIds)

        self.setupNetwork(wordIds, urlIds)
        self.feedForward()
        targets = [0.0] * len(urlIds)
        targets[urlIds.index(selectedUrl)] = 1.0
        self.backPropagate(targets)
        self.updateDatabase()

    def updateDatabase(self):
        for i in range(len(self.wordIds)):
            for j in range(len(self.hiddenIds)):
                self.setStrength(self.wordIds[i], self.hiddenIds[j], 0, self.wi[i][j])

        for i in range(len(self.hiddenIds)):
            for j in range(len(self.urlIds)):
                self.setStrength(self.hiddenIds[i], self.urlIds[j], 1, self.wo[i][j])

        self.conn.commit()

