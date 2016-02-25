import urllib2
from bs4 import BeautifulSoup
from urlparse import urljoin
import sqlite3
import re
import nn

ignoreWords = set(['the','of','to','and','a','in','is','it'])
myNet = nn.searchNet('nn.db')

class crawler:
    def __init__(self, dbname):
        self.conn = sqlite3.connect(dbname)

    def __del__(self):
        self.conn.close()

    def dbCommit(self):
        self.conn.commit()

    def getEntryId(self, table, field, value, createNew = True):
        curr = self.conn.execute("select rowid from %s where %s = '%s'" % (table, field, value))
        res = curr.fetchone()
        if res == None:
            cur = self.conn.execute("insert into %s (%s) values ('%s')" % (table, field, value))
            return cur.lastrowid
        else:
            return res[0]

    def addToIndex(self, url, soup):
        if self.isIndexed(url): return
        print 'Indexing ' + url

        text = self.getTextOnly(soup)
        words = self.separateWords(text)

        urlid = self.getEntryId('urllist', 'url', url)

        for i in range(len(words)):
            word = words[i]
            if word in ignoreWords: continue
            wordid = self.getEntryId('wordlist', 'word', word)
            self.conn.execute("insert into wordlocation(urlid, wordid, location) values (%d, %d, %d)" %
                              (urlid, wordid, i))

    def getTextOnly(self, soup):
        v = soup.string
        if v == None:
            c = soup.contents
            resultText = ''
            for t in c:
                subText = self.getTextOnly(t)
                resultText += subText + '\n'
            return resultText
        else:
            return v.strip()

    def separateWords(self, text):
        splitter = re.compile('\\W*')
        return [s.lower() for s in splitter.split(text) if s != '']

    def isIndexed(self, url):
        u = self.conn.execute("select rowid from urllist where url = '%s'" % url).fetchone()
        if u != None:
            v = self.conn.execute("select * from wordlocation where urlid = %d" % u[0]).fetchone()
            if v != None: return True
        return False

    def addLinkRef(self, urlFrom, urlTo, linkText):
        fromId = self.getEntryId('urllist', 'url', urlFrom)
        toId = self.getEntryId('urllist', 'url', urlTo)
        if fromId == toId: return
        cur = self.conn.execute('insert into link (fromid, toid) values (%d, %d)'
                               % (fromId, toId))

        linkId = cur.lastrowid
        # Remember each word in link text
        for word in self.separateWords(linkText):
            if word in ignoreWords: continue
            wordId = self.getEntryId('wordlist', 'word', word)
            self.conn.execute('insert into linkwords (wordid, linkid) \
            values (%d, %d)' % (wordId, linkId))

    def crawl(self, pages, depth = 2):
        for i in range(depth):
            newPages = set()
            for page in pages:
                try:
                    c = urllib2.urlopen(page)
                except:
                    print "Cannot Open %s" % page
                    continue
                soup = BeautifulSoup(c.read(), 'lxml')
                self.addToIndex(page, soup)

                links = soup('a')
                for link in links:
                    if 'href' in dict(link.attrs):
                        url = urljoin(page, link['href'])
                        if url.find("'") != -1: continue
                        url = url.split('#')[0]
                        if url[0:4] == 'http' and not self.isIndexed(url):
                            newPages.add(url)
                        linkText = self.getTextOnly(link)
                        self.addLinkRef(page, url, linkText)
                self.dbCommit()
            pages = newPages
        self.calculatePageRank()

    def createIndexTables(self):
        self.conn.execute('create table urllist(url)')
        self.conn.execute('create table wordlist(word)')
        self.conn.execute('create table wordlocation(urlid, wordid, location)')
        self.conn.execute('create table link(fromid integer, toid integer)')
        self.conn.execute('create table linkwords(wordid, linkid)')
        self.conn.execute('create index wordidx on wordlist(word)')
        self.conn.execute('create index urlidx on urllist(url)')
        self.conn.execute('create index wordurlidx on wordlocation(wordid)')
        self.conn.execute('create index urltoidx on link(toid)')
        self.conn.execute('create index urlfromidx on link(fromid)')
        self.dbCommit()

    def calculatePageRank(self, iterations = 40):
        self.conn.execute('drop table if exists pagerank')
        self.conn.execute('create table pagerank(urlid primary key, score)')

        self.conn.execute('insert into pagerank select rowid, 1.0 from urllist')
        self.dbCommit()

        for i in range(iterations):
            print 'iteration %d / %d' % (i, iterations)
            for (urlId,) in self.conn.execute('select rowid from urllist'):
                pr = 0.15
                for (linker,) in self.conn.execute('select distinct fromid from link where toid=%d' % urlId):
                    linkingPr = self.conn.execute('select score from pagerank where urlid=%d' % linker).fetchone()[0]
                    linkingCount = self.conn.execute('select count(*) from link where fromid=%d' % linker).fetchone()[0]
                    pr += 0.85 * (linkingPr / linkingCount)
                self.conn.execute('update pagerank set score=%f where urlid=%d' % (pr, urlId))
            self.dbCommit()

#############################################################################################
# Search Engine
#
class searcher:
    def __init__(self, dbname):
        self.conn = sqlite3.connect(dbname)

    def __del__(self):
        self.conn.close()

    def getMatchRows(self, q):
        fieldList = 'w0.urlid'
        tableList = ''
        clauseList = ''
        wordIds = list()

        words = q.split(' ')
        tableNumber = 0

        for word in words:
            wordRow = self.conn.execute(
                "select rowid from wordlist where word = '%s'" % word).fetchone()
            if wordRow != None:
                wordId = wordRow[0]
                wordIds.append(wordId)
                if tableNumber > 0:
                    tableList += ' '
                    clauseList += ' and '
                    clauseList += 'w%d.urlid=w%d.urlid and ' % (tableNumber - 1, tableNumber)
                fieldList += ',w%d.location' % tableNumber
                tableList += 'wordlocation w%d' % tableNumber
                clauseList += 'w%d.wordid=%d' % (tableNumber, wordId)
                tableNumber += 1

        fullQuery = 'select %s from %s where %s' % (fieldList, tableList, clauseList)
        print fullQuery
        cur = self.conn.execute(fullQuery)
        rows = [row for row in cur]
        return rows, wordIds

    def getScoredList(self, rows, wordIds):
        totalScores = dict([(row[0], 0) for row in rows])
        ## Change the weights here
        weights = [(1.0, self.frequencyScore(rows)),
                   (0.5, self.locationScore(rows)),
                   (0.5, self.distanceScore(rows)),
                   (1.5, self.pageRankScore(rows)),
                   (1.0, self.nnScore(rows, wordIds))]

        for (weight, scores) in weights:
            for url in totalScores:
                totalScores[url] += weight * scores[url]

        return totalScores

    def getUrlName(self, id):
        return self.conn.execute(
            "select url from urllist where rowid = %d" % id).fetchone()[0]

    def query(self, q):
        rows, wordIds = self.getMatchRows(q)
        scores = self.getScoredList(rows, wordIds)
        rankedScores = sorted([(score, url) for (url, score) in scores.items()], reverse = True)
        for (score, urlId) in rankedScores[0:10]:
            print "%f\t%s" % (score, self.getUrlName(urlId))
        return wordIds, [r[1] for r in rankedScores[0:10]]

    def normalizeScores(self, scores, smallIsBetter = False):
        vSmall = 0.0000001
        if smallIsBetter:
            minScore = min(scores.values())
            return dict([(u, float(minScore) / max(vSmall, l))
                         for (u, l) in scores.items()])
        else:
            maxScore = max(scores.values())
            if maxScore == 0: maxScore = vSmall
            return dict([(u, float(c) / maxScore)
                         for (u, c) in scores.items()])

    def frequencyScore(self, rows):
        counts = dict([(row[0], 0) for row in rows])
        for row in rows: counts[row[0]] += 1
        return self.normalizeScores(counts)

    def locationScore(self, rows):
        locations = dict([(row[0], 99999999) for row in rows])
        for row in rows:
            loc = sum(row[1:])
            if loc < locations[row[0]]: locations[row[0]] = loc
        return self.normalizeScores(locations, smallIsBetter = True)

    def distanceScore(self, rows):
        if len(rows[0]) < 2: return dict([(row[0], 1.0) for row in rows])
        mindDistance = dict([(row[0], 99999999) for row in rows])
        for row in rows:
            dist = sum([abs(row[i] - row[i - 1]) for i in range(2, len(row))])
            if dist < mindDistance[row[0]]: mindDistance[row[0]] = dist
        return self.normalizeScores(mindDistance, smallIsBetter = True)

    def inboundLinkScore(self, rows):
        uniqueUrls = set([row[0] for row in rows])
        inboundCounts = dict([(u, self.conn.execute(\
                                "select count(*) from link where toid=%d" % u).fetchone()[0]) \
                              for u in uniqueUrls])
        return self.normalizeScores(inboundCounts)

    def pageRankScore(self, rows):
        pageRanks = dict([(row[0], self.conn.execute(\
                        'select score from pagerank where urlid=%d' % row[0]).fetchone()[0])
                          for row in rows])
        maxRank = max(pageRanks.values())
        normalizedScores = dict([(u, float(l) / maxRank) for (u,l) in pageRanks.items()])
        return normalizedScores

    def linkTextScore(self, rows, wordIds):
        linkScores = dict([(row[0], 0.0) for row in rows])
        for wordId in wordIds:
            cur = self.conn.execute('select link.fromid,link.toid from linkwords,link where wordid=%d and linkwords.linkid=link.rowid' % wordId)
            for (fromId, toId) in cur:
                if toId in linkScores:
                    pr = self.execute('select score from pagerank where urlid=%d' % fromId).fetchone()[0]
                    linkScores[toId] += pr
        maxScore = max(linkScores.values())
        normalizedScores = dict([(u, float(l) / maxScore) for (u,l) in linkScores.items()])
        return normalizedScores

    def nnScore(self, rows, wordIds):
        urlIds = [urlId for urlId in set([row[0] for row in rows])]
        nnRes = myNet.getResult(wordIds, urlIds)
        scores = dict([(urlIds[i], nnRes[i]) for i in range(len(urlIds))])
        return self.normalizeScores(scores)
