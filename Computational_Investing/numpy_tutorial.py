import numpy as np

# Create Arrays
zeroArray = np.zeros((2,3))
onesArray = np.ones((2,3))
randomArray = np.random.random((2,3))

foo = [ [1,2,3],
        [4,5,6]]

rangeArray = np.arange(6,12)

rangeArray = rangeArray.reshape((2,3))

# quickly manipulate matrices
squareArray = np.arage(1,10).reshape((3,3))
# [0,2)
# slcie matrices
squareArray[0, 0:2]

squareArray[:, 1:2]

randomRow = np.random.random((10,1))
fibIndices = np.array([1,1,2,3])
randomArray[fibIndices]

# use boolean values for masking
boolIndices = np.array([[True, False, True],
                        [False, True, False],
                        [True, False, True]])

squareArray[boolIndices]

sqAverage = np.average(squareArray)
np.median(squareArray)
np.sum(squareArray)

betterThanAverage = squareArray > sqAverage
squareArray[betterThanAverage]

# standard deviation
sqStdDev = np.std(squareArray)
# python is by-reference, not by-value, so use copy here to have
# two arrays
clampedSqArray = np.array(squareArray.copy(), dtype = float)
clampedSqArray[(squareArray - sqAverage) > sqStdDev] = sqAverage + sqStdDev
clampedSqArray[(squareArray - sqAverage) < -sqStdDev] = sqAverage - sqStdDev

squareArray * 2.0
squareArray / 2.0

squareArray + np.ones((3,3))
squareArray + 1

squareArray * np.arange(1,10).reshape((3,3))

matA = np.array([[1,2],[3,4]])
matB = np.array([[5,6],[7,8]])

matA.dot(matB)
