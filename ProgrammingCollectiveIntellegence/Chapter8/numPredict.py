from __future__ import division
import math
from random import random, randint
import matplotlib.pylab as plt
import numpy as np
weightDomain = [(0, 20)] * 4

def winePrice(rating, age):
    peakAge = rating - 50

    price = rating / 2.0
    if age > peakAge:
        price = price * (5 - (age - peakAge))
    else:
        price = price * (5 * (age + 1) / peakAge)

    if price < 0: price = 0
    return price

def wineSet1():
    rows = list()
    for i in range(300):
        rating = random() * 50 + 50
        age = random() * 50

        price = winePrice(rating, age)
        price *= (random() * 0.4 + 0.8)

        rows.append({'input': (rating, age),
                     'result': price})
    return rows

def euclidean(v1, v2):
    d = 0.0
    for i in range(len(v1)):
        d += (v1[i] - v2[i]) ** 2.0

    return math.sqrt(d)

def getDistances(data, vec1):
    distanceList = list()
    for i in range(len(data)):
        vec2 = data[i]['input']
        distanceList.append((euclidean(vec1, vec2), i))
    distanceList.sort()
    return distanceList

def kNNEstimate(data, vec1, k = 5):
    distanceList = getDistances(data, vec1)
    avg = 0.0

    for i in range(k):
        idx = distanceList[i][1]
        avg += data[idx]['result']

    return avg / k

def inverseWeight(dist, num = 1.0, const = 0.1):
    return num / (dist + const)

def substraceWeight(dist, const = 1.0):
    if dist > const:
        return 0
    else:
        return const - dist

def gaussianWeight(dist, sigma = 10.0):
    return math.e ** (-dist ** 2 / (2 * sigma ** 2))

def weightedKNN(data, vec1, k = 5, weightf = gaussianWeight):
    distanceList = getDistances(data, vec1)
    avg = 0.0
    totalWeight = 0.0

    for i in range(k):
        idx = distanceList[i][1]
        dist = distanceList[i][0]
        weight = weightf(dist)
        avg += weight * data[idx]['result']
        totalWeight += weight

    return avg / totalWeight

def divideData(data, test = 0.05):
    trainSet = list()
    testSet = list()
    for row in data:
        if random() < test:
            testSet.append(row)
        else:
            trainSet.append(row)
    return trainSet, testSet

def testAlgorithm(algf, trainSet, testSet):
    error = 0.0
    for row in testSet:
        guess = algf(trainSet, row['input'])
        error += (row['result'] - guess) ** 2
    return error / len(testSet)

def crossValidate(algf, data, trials = 100, test = 0.05):
    error = 0.0
    for i in range(trials):
        trainSet, testSet = divideData(data, test)
        error += testAlgorithm(algf, trainSet, testSet)
    return error / trials

def wineSet2():
    rows = list()
    for i in range(300):
        rating = random() * 50 + 50
        age = random() * 50
        aisle = float(randint(1, 20))
        bottleSize = [375.0, 750, 1500.0, 3000.0][randint(0, 3)]
        price = winePrice(rating, age)
        price *= (bottleSize / 750.0)
        price *= (random() * 0.9 + 0.2)
        rows.append({'input': (rating, age, aisle, bottleSize),
                     'result': price})
    return rows

def rescale(data, scale):
    scaledData = list()
    for row in data:
        scaled = [scale[i] * row['input'][i]
                  for i in range(len(scale))]
        scaledData.append({'input': scaled, 'result': row['result']})
    return scaledData

def createCostFunction(algf, data):
    def costf(scale):
        sdata = rescale(data, scale)
        return crossValidate(algf, sdata, trials = 10)
    return costf

def wineSet3():
    rows = wineSet1()
    for row in rows:
        if random() < 0.5:
            row['result'] *= 0.5
    return rows

def probGuess(data, vec1, low, high, k = 5, weightf = gaussianWeight):
    dList = getDistances(data, vec1)
    nWeight = 0.0
    tWeight = 0.0

    for i in range(k):
        dist = dList[i][0]
        idx = dList[i][1]
        weight = weightf(dist)
        v = data[idx]['result']

        if v >= low and v <= high:
            nWeight += weight
        tWeight += weight
    if tWeight == 0: return 0
    return nWeight / tWeight

def cummulativeGraph(data, vec1, high, k = 5, weightf = gaussianWeight):
    t1 = np.arange(0.0, high, 0.1)
    cProb = np.array([probGuess(data, vec1, 0, v, k, weightf) for v in t1])
    plt.plot(t1, cProb)
    plt.show()

def probabilityGraph(data, vec1, high, k = 5, weightf = gaussianWeight, ss = 5.0):
    t1 = np.arange(0.0, high, 0.1)
    probs = np.array([probGuess(data, vec1, v, v + 0.1, k, weightf) for v in t1])

    smoothed = []
    for i in range(len(probs)):
        sv = 0.0
        for j in range(0, len(probs)):
            dist = abs(i - j) * 0.1
            weight = gaussianWeight(dist, sigma = ss)
            sv += weight * probs[j]
        smoothed.append(sv)
    smoothed = np.array(smoothed)

    plt.plot(t1, smoothed)
    plt.show()
