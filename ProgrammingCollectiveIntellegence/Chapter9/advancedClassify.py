import matplotlib.pylab as plt

class matchRow:
    def __init__(self, row, allNum = False):
        if allNum:
            self.data = [float(row[i]) for i in range(len(row) - 1)]
        else:
            self.data = row[0 : len(row) - 1]
        self.match = int(row[len(row) - 1])

def loadMatch(f, allNum = False):
    rows = list()
    for line in file(f):
        rows.append(matchRow(line.split(','), allNum))
    return rows

def plotAgeMatches(rows):
    xdm, ydm = [r.data[0] for r in rows if r.match == 1],\
               [r.data[1] for r in rows if r.match == 1]
    xdn, ydn = [r.data[0] for r in rows if r.match == 0],\
               [r.data[1] for r in rows if r.match == 0]

    plt.plot(xdm, ydm, 'go')
    plt.plot(xdn, ydn, 'ro')
    plt.show()

def linearTrain(rows):
    averages = dict()
    counts = dict()

    for row in rows:
        cl = row.match
        averages.setdefault(cl, [0.0] * (len(row.data)))
        counts.setdefault(cl, 0)

        for i in range(len(row.data)):
            averages[cl][i] += float(row.data[i])
        counts[cl] += 1

    for cl, avg in averages.items():
        for i in range(len(avg)):
            avg[i] /= counts[cl]

    return averages

def dotProduct(v1, v2):
    return sum([v1[i] * v2[i] for i in range(len(v1))])

def dpClassify(point, avgs):
    b = (dotProduct(avgs[1], avgs[1]) - dotProduct(avgs[0], avgs[0])) / 2
    y = dotProduct(point, avgs[0]) - dotProduct(point, avgs[1]) + b
    if y > 0: return 0
    else: return 1

def yesNo(v):
    if v == 'yes': return 1
    elif v == 'no': return -1
    else: return 0

def matchCount(interest1, interest2):
    l1 = interest1.split(':')
    l2 = interest2.split(':')
    x = 0
    for v in l1:
        if v in l2: x += 1
    return x

yahookey="YOUR API KEY"
from xml.dom.minidom import parseString
from urllib import urlopen,quote_plus

loc_cache={}
def getLocation(address):
  if address in loc_cache: return loc_cache[address]
  data = urlopen('http://api.local.yahoo.com/MapsService/V1/'+\
               'geocode?appid=%s&location=%s' %
               (yahookey,quote_plus(address))).read()
  doc = parseString(data)
  lat = doc.getElementsByTagName('Latitude')[0].firstChild.nodeValue
  long = doc.getElementsByTagName('Longitude')[0].firstChild.nodeValue
  loc_cache[address] = (float(lat), float(long))
  return loc_cache[address]

def milesDistance(a1,a2):
  lat1,long1 = getLocation(a1)
  lat2,long2 = getLocation(a2)
  latdif = 69.1 * (lat2 - lat1)
  longdif = 53.0 * (long2 - long1)
  return (latdif ** 2 + longdif ** 2) ** .5

def loadNumerical():
  oldRows = loadMatch('matchmaker.csv')
  newRows = []
  for row in oldRows:
    d = row.data
    data = [float(d[0]), yesNo(d[1]), yesNo(d[2]),
            float(d[5]), yesNo(d[6]), yesNo(d[7]),
            matchCount(d[3],d[8]),
            milesDistance(d[4],d[9]),
            row.match]
    newRows.append(matchRow(data))
  return newRows

def scaleData(rows):
    low = [99999999999999999999.9] * len(rows[0].data)
    high = [-99999999999999999999.9] * len(rows[1].data)

    for row in rows:
        d = row.data
        for i in range(len(d)):
            if d[i] < low[i]: low[i] = d[i]
            if d[i] > high[i]: high[i] = d[i]

    def scaleInput(d):
        return [(d.data[i] - low[i]) / (high[i] - low[i])
                for i in range(len(row))]

    newRows = [matchRow(scaleInput(row.data) + [row.match])
               for row in rows]

    return newRows, scaleInput
def vecLength(v):
  return sum([p**2 for p in v])

def rbf(v1, v2, gamma = 20):
    dv = [v1[i] - v2[i] for i in range(len(v1))]
    l = vecLength(dv)
    return math.e ** (-gamma * l)

def nlClassify(point, rows, offset, gamma = 10):
    sum0 = 0.0
    sum1 = 0.0
    count0 = 0
    count1 = 0

    for row in rows:
        if row.match == 0:
            sum0 += rbf(point, row.data, gamma)
            count0 += 1
        else:
            sum1 += rbf(point, row.data, gamma)
            count1 += 1
    y = (1.0 / count0) * sum0 - (1.0 / count1) * sum1 + offset

    if y < 0: return 0
    else: return 1

def getOffset(rows, gamma = 10):
    l0 = list()
    l1 = list()

    for row in rows:
        if row.match == 0: l0.append(row.data)
        else: l1.append(row.data)
    sum0 = sum([sum([rbf(v1, v2, gamma) for v1 in l0]) for v2 in l0])
    sum1 = sum([sum([rbf(v1, v2, gamma) for v1 in l1]) for v2 in l1])

    return (1.0 / (len(l1) ** 2)) * sum1 - (1.0 / (len(l0) ** 2)) * sum0

