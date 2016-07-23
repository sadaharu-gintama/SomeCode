import numpy as np
import operator
################################################################################
# First classifier
def createDataSet():
    group = np.array([[1.0, 1.1], [1.0, 1.0], [0.0, 0.0], [0.0, 0.1]])
    labels = ['A', 'A', 'B', 'B']
    return group, labels

def classify0(inX, dataSet, labels, k):
    dataSetSize = dataSet.shape[0]
    diffMat = np.tile(inX, (dataSetSize, 1)) - dataSet
    sqDiffMat = diffMat ** 2
    sqDistance = sqDiffMat.sum(axis = 1)
    distances = sqDistance ** 0.5
    sortedDistIndices = distances.argsort()

    classCount = dict()
    for i in range(k):
        voteIlabel = labels[sortedDistIndices[i]]
        classCount[voteIlabel] = classCount.get(voteIlabel, 0) + 1
    sortedClassCount = sorted(classCount.iteritems(),
                              key = operator.itemgetter(1),
                              reverse = True)
    return sortedClassCount[0][0]
################################################################################
# dating mate classifier
def file2matrix(filename):
    with open(filename) as fr:
        arrayOLines = fr.readlines()
    numberOfLines = len(arrayOLines)
    returnMat = np.zeros((numberOfLines, 3))

    classLabelVector = list()
    index = 0
    for line in arrayOLines:
        line = line.strip()
        listFromLine = line.split('\t')
        returnMat[index, :] = listFromLine[0 : 3]
        classLabelVector.append(int(listFromLine[-1]))
        index += 1
    return returnMat, classLabelVector

# Matplotlib creat scatter plot
#if __name__ == '__main__':
#    import matplotlib
#    import matplotlib.pyplot as plt
#    datingDataMat, datingLabel = file2matrix('datingTestSet2.txt')
#    fig = plt.figure()
#    ax = fig.add_subplot(111)
#    ax.scatter(datingDataMat[:, 0], datingDataMat[:, 1],
#               15 * np.array(datingLabel), 15 * np.array(datingLabel))
#    plt.savefig('scatter.pdf')

################################################################################
# Prepare data and normalize them
def autoNorm(dataSet):
    minVals = dataSet.min(0)
    maxVals = dataSet.max(0)

    ranges = maxVals - minVals
    normDataSet = np.zeros(np.shape(dataSet))
    m = dataSet.shape[0]

    normDataSet = dataSet - np.tile(minVals, (m, 1))
    normDataSet = normDataSet / np.tile(ranges, (m, 1))

    return normDataSet, ranges, minVals

# Dating Classifier
def datingClassTest():
    hoRatio = 0.1
    datingDataMat, datingLabels = file2matrix('datingTestSet2.txt')
    normMat, ranges, minVals = autoNorm(datingDataMat)
    m = normMat.shape[0]
    numTestVecs = int(m * hoRatio)
    errorCount = 0.0
    for i in range(numTestVecs):
        classifierResult = classify0(normMat[i, :], normMat[numTestVecs: m, :],
                                     datingLabels[numTestVecs: m], 3)
        print 'the classifier came back with %d, the real value is %d' \
            % (classifierResult, datingLabels[i])

        if (classifierResult != datingLabels[i]): errorCount += 1
    print "total error rate is %f" % (errorCount / float(numTestVecs))

################################################################################
def classifyPerson():
    resultList = ['not at all', 'in small doses', 'in large doses']
    percentTats = float(raw_input("time spent playing video game:"))
    ffMiles = float(raw_input('frequent filer miles:'))
    iceCream = float(raw_input('Liters of ice cream:'))

    datingDataMat, datingLabels = file2matrix('datingTestSet2.txt')
    normMat, ranges, minVals = autoNorm(datingDataMat)
    inArr = np.array([ffMiles, percentTats, iceCream])
    classifierResult = classify0((inArr - minVals) / ranges,
                                 normMat, datingLabels, 3)
    print "you will probably like this person: " + \
          resultList[classifierResult - 1]

################################################################################
def img2vector(fileName):
    returnVect = np.zeros((1, 1024))
    with open(fileName) as fr:
        for i in range(32):
            linStr = fr.readline()
            for j in range(32):
                returnVect[0, 32 * i + j] = int(linStr[j])
    return returnVect


def handWritingClassifier():
    from os import listdir
    hwLabels = list()
    trainingFileList = listdir('trainingDigits')
    m = len(trainingFileList)
    trainingMat = np.zeros((m, 1024))
    for i in range(m):
        fileNameStr = trainingFileList[i]
        fileStr = fileNameStr.split('.')[0]
        classNumStr = fileStr.split('_')[0]
        hwLabels.append(int(classNumStr))
        trainingMat[i, :] = img2vector('trainingDigits/%s' % fileNameStr)

    testFileList = listdir('testDigits')
    errorCount = 0
    mTest = len(testFileList)
    for i in range(mTest):
        fileNameStr = testFileList[i]
        fileStr = fileNameStr.split('.')[0]
        classNumStr = int(fileStr.split('_')[0])
        vectorUnderTest = img2vector('testDigits/%s' % fileNameStr)
        classifierResult = classify0(vectorUnderTest, trainingMat, hwLabels, 3)
        print 'the classifier comes back with %d, the real answer is %d' % \
            (classifierResult, classNumStr)
        if (classifierResult != classNumStr): errorCount += 1
    print "the total numer of errors is %d" % errorCount
    print "the total error rate is %f" % (errorCount / float(mTest))
