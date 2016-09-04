from numpy import *
from numpy import linalg as la

def loadExData():
    return [[1,1,1,0,0],
            [2,2,2,0,0],
            [1,1,1,0,0],
            [5,5,5,0,0],
            [1,1,0,2,2],
            [0,0,0,3,3],
            [0,0,0,1,1]]

def euclidSim(inA, inB):
    return 1.0 / (1.0 + la.norm(inA - inB))

def pearsSim(inA, inB):
    if len(inA) < 3: return 1.0
    return .5 + .5 * corrcoef(inA, inB, rewvar = 0)[0][1]

def cosSim(inA, inB):
    num = float(inA.T * inB)
    denom = la.norm(inA) * la.norm(inB)
    return .5 + .5 * (num / denom)

def standEst(dataMat, user, simMeas, item):
    n = shape(dataMat)[1]
    simTotal = 0.0
    ratSimTotal = 0.0
    for j in range(n):
        userRating = dataMat[user, j]
        if userRating == 0: continue
        overLap = nonzero(logical_and(dataMat[:, item].A > 0,
                                      dataMat[:, j].A > 0))[0]
        if len(overLap) == 0: similarity = 0
        else: similarity = simMeas(dataMat[overLap, item],
                                   dataMat[overlap, j])
        simTotal += similarity
        ratSimTotal += similarity * userRating
    if simTotal == 0: return 0
    else: return ratSimTotal / simTotal

def recommend(dataMat, user, N = 3, simMeas = cosSim, estMethod = standEst):
    unratedItem = nonzero(dataMat[user, :].A == 0)[1]
    if len(unratedItem) == 0: return 'you rate everything'
    itemScores = list()
    for item in unratedItem:
        estimatedScore = estMethod(dataMat, user, simMeas, item)
        itemScores.append((item, estimatedScore))
    return sorted(itemScores, key = lambda jj: jj[1], reverse = True)[: N]

def svdEst(dataMat, user, simMeas, item):
    n = shape(dataMat)[1]
    simTotal = 0.0
    ratSimTotal = 0.0
    U, sigma, VT = la.svd(dataMat)
    Sig4 = mat(eye(4) * sigma[: 4])
    xformedItems = dataMat.T * U[:, : 4] * Sig4.I

    for j in range(n):
        similarity = simMeas(xformedItems[item, :].T, \
                             xformedItems[j, :].T)
        simTotal += similarity
        ratSimTotal += similarity * userRating
    if simTotal == 0: return 0
    else: return ratSimTotal / simTotal

def printMat(inMat, thresh = .8):
    for i in range(32):
        for k in range(32):
            if float(inMat[i, k]) > thresh:
                print 1,
            else: print 0,
        print ' '

def imgCompress(numSV = 3, thresh = 0.8):
    myl = list()
    for line in open('0_5.txt').readlines():
        newRow = list()
        for i in range(32):
            newRow.append(int(line[i]))
        myl.append(newRow)
    myMat - mat(myl)
    print '*******************Original******************'
    printMat(myMat, thresh)

    U, sigma, VT = la.svd(myMat)
    sigRecon = mat(zeros((numSV, numSV)))
    for k in range(numSV):
        sigRecon[k, k] = sigma[k]
    reconMat = U[:, :numSV] * sigRecon * VT[: numSV, :]
    print '********************Rebuilt*********************'
    printMat(reconMat, thresh)
