from math import sqrt
from PIL import Image, ImageDraw
import random

def readFile(fileName):
    lines = [line for line in file(fileName)]

    colNames = lines[0].strip().split('\t')[1:]
    rowNames = list()
    data = list()
    for line in lines[1:]:
        p = line.strip().split('\t')
        rowNames.append(p[0])
        data.append([float(x) for x in p[1:]])
    return rowNames, colNames, data

def pearson(v1, v2):
    sum1 = sum(v1)
    sum2 = sum(v2)

    sum1sq = sum([pow(x, 2) for x in v1])
    sum2sq = sum([pow(x, 2) for x in v2])

    pSum = sum([vi1 * vi2 for vi1, vi2 in zip(v1, v2)])

    num = pSum - (sum1 * sum2 / len(v1))
    den = sqrt((sum1sq - pow(sum1, 2) / len(v1)) * (sum2sq - pow(sum2, 2) / len(v2)))
    if den == 0: return 0

    return 1.0 - num / den

class biCluster:
    def __init__(self, vec, left = None, right = None, distance = 0.0, id = None):
        self.left = left
        self.right = right
        self.vec = vec
        self.id = id
        self.distance = distance

def hCluster(rows, distance = pearson):
    distances = dict()
    currentClustId = -1

    clust = [biCluster(rows[i], id = i) for i in range(len(rows))]

    while len(clust) > 1:
        lowestPair = (0,1)
        closest = distance(clust[0].vec, clust[1].vec)

        for i in range(len(clust)):
            for j in range(i + 1, len(clust)):
                if (clust[i].id, clust[j].id) not in distances:
                    distances[(clust[i].id, clust[j].id)] = distance(clust[i].vec, clust[j].vec)
                d = distances[(clust[i].id, clust[j].id)]

                if d < closest:
                    closest = d
                    lowestPair = (i, j)

        mergeVec = [(clust[lowestPair[0]].vec[i] + clust[lowestPair[1]].vec[i]) / 2.0 for i in range(len(clust[0].vec))]
        newCluster = biCluster(mergeVec, left = clust[lowestPair[0]], right = clust[lowestPair[1]],
                               distance = closest, id = currentClustId)

        currentClustId -= 1
        # has to be the following sequence
        del clust[lowestPair[1]]
        del clust[lowestPair[0]]
        clust.append(newCluster)

    return clust[0]

def printCluster(clust, labels = None, n = 0):
    for i in range(n): print ' ',
    if clust.id < 0:
        print '-'
    else:
        if labels == None: print clust.id
        else: print labels[clust.id]

    if clust.left != None: printCluster(clust.left, labels = labels, n = n + 1)
    if clust.right != None: printCluster(clust.right, labels = labels, n = n + 1)

def getHeight(clust):
    if clust.left == None and clust.right == None: return 1
    else: return getHeight(clust.left) + getHeight(clust.right)

def getDepth(clust):
    if clust.left == None and clust.right == None: return 0
    else: return max(getDepth(clust.left), getDepth(clust.right)) + clust.distance

def drawDendrogram(clust, labels, jpeg = 'clusters.jpg'):
    h = getHeight(clust) * 20
    w = 1200
    depth = getDepth(clust)

    scaling = float(w - 150) / depth

    img = Image.new('RGB', (w, h), (255, 255, 255))
    draw = ImageDraw.Draw(img)

    draw.line((0, h / 2, 10, h / 2), fill = (255, 0, 0))

    drawNode(draw, clust, 10, h / 2, scaling, labels)
    img.save(jpeg, 'JPEG')

def drawNode(draw, clust, x, y, scaling, labels):
    if clust.id < 0:
        h1 = getHeight(clust.left) * 20
        h2 = getHeight(clust.right) * 20
        top = y - (h1 + h2) / 2
        bottom = y + (h1 + h2) / 2

        ll = clust.distance * scaling

        draw.line((x, top + h1 / 2, x, bottom - h2 / 2), fill = (255, 0, 0))
        draw.line((x, top + h1 / 2, x + ll, top + h1 / 2), fill = (255, 0, 0))
        draw.line((x, bottom - h2 / 2, x + ll, bottom - h2 / 2), fill = (255, 0, 0))

        drawNode(draw, clust.left, x + ll, top + h1 / 2, scaling, labels)
        drawNode(draw, clust.right, x + ll, bottom - h2 / 2, scaling, labels)
    else:
        draw.text((x + 5, y - 7), labels[clust.id], (0,0,0))

def rotateMatrix(data):
    newData = list()
    for i in range(len(data[0])):
        newRow = [data[j][i] for j in range(len(data))]
        newData.append(newRow)
    return newData

def kCluster(rows, distance = pearson, k = 4):
    ranges = [(min([row[i] for row in rows]), max([row[i] for row in rows]))
              for i in range(len(rows[0]))]

    clusters = [[random.random() * (ranges[i][1] - ranges[i][0]) + ranges[i][0]
                 for i in range(len(rows[0]))] for j in range(k)]

    lastMatches = None
    for t in range(100):
        print 'iteration %d' % t
        bestMatches = [[] for i in range(k)]

        for j in range(len(rows)):
            row = rows[j]
            bestMatch = 0
            for i in range(k):
                d = distance(clusters[i], row)
                if d < distance(clusters[bestMatch], row): bestMatch = i
            bestMatches[bestMatch].append(j)

        if bestMatches == lastMatches: break
        lastMatches = bestMatches

    for i in range(k):
        avgs = [0.0] * len(rows[0])
        if len(bestMatches[i]) > 0:
            for rowId in bestMatches[i]:
                for m in range(len(rows[rowId])):
                    avgs[m] += rows[rowId][m]

            for j in range(len(avgs)):
                avgs[j] /= len(bestMatches[i])
            clusters[i] = avgs

    return bestMatches

def tanimoto(v1, v2):
    c1, c2, shr = 0, 0, 0

    for i in range(len(v1)):
        if (v1[i] != 0) : c1 += 1
        if (v2[i] != 0) : c2 += 1
        if (v1[i] != 0 and v2[i] != 0): shr += 1

    return 1.0 - float(shr) / (c1 + c2 - shr)

def scaleDown(data, distance = pearson, rate = 0.01):
    n = len(data)

    realdist = [[distance(data[i], data[j]) for j in range(n)]
                for i in range(0, n)]

    outerSum = 0.0

    loc = [[random.random(), random.random()] for i in range(n)]
    fakedist = [[0.0 for j in range(n)] for i in range(n)]

    lastError = None
    for m in range(0, 1000):
        for i in range(n):
            for j in range(n):
                fakedist[i][j] = sqrt(sum([pow(loc[i][x] - loc[j][x], 2)
                                           for x in range(len(loc[i]))]))

        grad = [[0.0, 0.0] for i in range(n)]

        totalError = 0
        for k in range(n):
            for j in range(n):
                if j == k: continue
                print j, k, realdist[j][k]
                errorTerm = (fakedist[j][k] - realdist[j][k]) / realdist[j][k]
                grad[k][0] += ((loc[k][0] - loc[j][0]) / fakedist[j][k]) * errorTerm
                grad[k][1] += ((loc[k][1] - loc[j][1]) / fakedist[j][k]) * errorTerm
                totalError += abs(errorTerm)

        print totalError

        if lastError and lastError < totalError: break
        lastError = totalError

        for k in range(n):
            loc[k][0] -= rate * grad[k][0]
            loc[k][1] -= rate * grad[k][1]

    return loc

def draw2D(data, labels, jpeg = 'mds2d.jpg'):
    img = Image.new('RGB', (2000, 2000), (255, 255, 255))
    draw = ImageDraw.Draw(img)

    for i in range(len(data)):
        x = (data[i][0] + 0.5) * 1000
        y = (data[i][1] + 0.5) * 1000
        draw.text((x, y), label[i], (0, 0, 0))

    img.save(jpeg, 'JPEG')
