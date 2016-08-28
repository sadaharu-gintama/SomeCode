from numpy import *

def loadDataSet(fileName, delim = '\t'):
    fr = open(fileName)
    stringArr = [line.strip().split(delim) for line in fr.readlines()]
    dataArr = [map(float, line) for line in stringArr]
    return mat(dataArr)

def pca(dataMat, topNfeat = 999999):
    meanVals = mean(dataMat, axis = 0)
    meanRemove = dataMat - meanVals
    covMat = cov(meanRemove, rowvar = 0)
    eigVals, eigVectors = linalg.eig(mat(covMat))
    eigValInd = argsort(eigVals)
    eigValInd = eigValInd[: - (topNfeat + 1) : -1]
    redEigVects = eigVectors[:, eigValInd]
    lowDDataMat = meanRemove * redEigVects
    reconMat = (lowDDataMat * redEigVects.T) + meanVals
    return lowDDataMat, reconMat

def replaceNanWithMean():
    dataMat = loadDataSet('secom.data', ' ')
    numFeat = shape(dataMat)[1]
    for i in range(numFeat):
        meanVal = mean(dataMat[nonzero(~isnan(dataMat[:, i].A))[0], i])
        dataMat[nonzero(isnan(dataMat[:, i].A))[0], i] = meanVal

    return dataMat
