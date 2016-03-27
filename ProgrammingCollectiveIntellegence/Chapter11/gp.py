from random import random, randint, choice
from copy import deepcopy
from math import log

class fWrapper:
    def __init__(self, function, childCount, name):
        self.function = function
        self.childCount = childCount
        self.name = name

class node:
    def __init__(self, fw, children):
        self.function = fw.function
        self.name = fw.name
        self.children = children

    def evaluate(self, inp):
        results = [n.evaluate(inp) for n in self.children]
        return self.function(results)

    def display(self, indent = 0):
        print (' ' * indent) + self.name
        for c in self.children:
            c.display(indent + 1)

class paramNode:
    def __init__(self, idx):
        self.idx = idx

    def evaluate(self, inp):
        return inp[self.idx]

    def display(self, indent = 0):
        print '%sp%d' % (' ' * indent, self.idx)

class constNode:
    def __init__(self, v):
        self.v = v

    def evaluate(self, inp):
        return self.v

    def display(self, indent = 0):
        print '%s%d' % (' ' * indent, self.v)

addW = fWrapper(lambda l: l[0] + l[1], 2, 'add')
subW = fWrapper(lambda l: l[0] - l[1], 2, 'subtract')
mulW = fWrapper(lambda l: l[0] * l[1], 2, 'multiply')

def ifFunc(l):
    if l[0] > 0: return l[1]
    else: return l[2]
ifW = fWrapper(ifFunc, 3, 'if')

def isGreater(l):
    if l[0] > l[1]: return 1
    else: return 0
gtW = fWrapper(isGreater, 3, 'isGreater')

fList = [addW, mulW, ifW, gtW, subW]

def exampleTree():
    return node(ifW, [
        node(gtW, [paramNode(0), constNode(3)]),
        node(addW, [paramNode(1), constNode(5)]),
        node(subW, [paramNode(1), constNode(2)])
    ])

def makeRandomTree(pc, maxDepth = 4, fPr = 0.5, pPr = 0.6):
    if random() < fPr and maxDepth > 0:
        f = choice(fList)
        children = [makeRandomTree(pc, maxDepth - 1, fPr, pPr)
                    for i in range(f.childCount)]
        return node(f, children)
    elif random() < pPr:
        return paramNode(randint(0, pc - 1))
    else:
        return constNode(randint(0, 10))

def hiddenFunction(x, y):
    return x ** 2 + 2 * y + 3 * x + 5

def buildHiddenSet():
    rows = list()
    for i in range(200):
        x = randint(0, 40)
        y = randint(0, 40)
        rows.append([x, y, hiddenFunction(x, y)])
    return rows

def scoreFunction(tree, s):
    dif = 0
    for data in s:
        v = tree.evaluate([data[0], data[1]])
        dif += abs(v - data[2])
    return dif

def mutate(t, pc, probChange = 0.1):
    if random() < probChange:
        return makeRandomTree(pc)
    else:
        result = deepcopy(t)
        if isinstance(t, node):
            result.children = [mutate(c, pc, probChange) for c in t.children]
        return result

def crossOver(t1, t2, probSwap = 0.7, top = 1):
    if random() < probSwap and not top:
        return deepcopy(t2)
    else:
        result = deepcopy(t1)
        if isinstance(t1, node) and isinstance(t2, node):
            result.children = [crossOver(c, choice(t2.children), probSwap, 0)
                               for c in t1.children]
        return result

def evolve(pc, popSize, rankFunction, maxGen = 500, mutationRate = 0.1, breedingRate = 0.4,
           pExp = 0.7, pNew = 0.05):
    def selectIndex():
        return int(log(random()) / log(pExp))

    population = [makeRandomTree(pc) for i in range(popSize)]
    for i in range(maxGen):
        scores = rankFunction(population)
        print scores[0][0]
        if scores[0][0] == 0: break

        newPop = [scores[0][1], scores[1][1]]
        while len(newPop) < popSize:
            if random() > pNew:
                newPop.append(mutate(
                              crossOver(scores[selectIndex()][1],
                                        scores[selectIndex()][1],
                                        probSwap = breedingRate),
                               pc, probChange = mutationRate))

            else:
                newPop.append(makeRandomTree(pc))
        population = newPop
    scores[0][1].display()
    return scores[0][1]

def getRankFunction(dataSet):
    def rankFunction(population):
        scores = [(scoreFunction(t, dataSet), t) for t in population]
        scores.sort()
        return scores
    return rankFunction

def gridGame(p):
    max = (3, 3)
    lastMove = [-1, -1]
    location = [[randint(0, max[0]), randint(0, max[1])]]
    location.append([(location[0][0] + 2) % 4, (location[0][1] + 2) % 4])

    for o in range(50):
        for i in range(2):
            locs = location[i][:] + location[1 - i][:]
            locs.append(lastMove[i])
            move = p[i].evaluate(locs) % 4

            if lastMove[i] == move: return 1 - i
            lastMove[i] = move
            if move == 0:
                location[i][0] -= 1
                if location[i][0] < 0: location[i][0] = 0
            if move == 1:
                location[i][0] += 1
                if location[i][0] > max[0]: location[i][0] = max[0]
            if move == 2:
                location[i][1] -= 1
                if location[i][1] < 0: location[i][1] = 0
            if move == 3:
                location[i][1] += 1
                if location[i][1] > max[1]: location[i][1] = max[1]
            if location[i] == location[1 - i]: return i
    return -1

def tournament(p1):
    losses = [0 for p in p1]

    for i in range(len(p1)):
        for j in range(len(p1)):
            if i == j: continue

            winner = gridGame([p1[i], p1[j]])
            if winner == 0:
                losses[j] += 2
            elif winner == 1:
                losses[i] += 2
            elif winner == -1:
                losses[i] += 1
                losses[j] += 1
                pass
    z = zip(losses, p1)
    z.sort()
    return z

class humanPlayer:
    def evaluate(self, board):
        me = tuple(board[0 : 2])
        others = [tuple(board[x : x + 2]) for x in range(2, len(board) - 1, 2)]

        for i in range(4):
            for j in range(4):
                if (i, j) == me:
                    print 'O',
                elif (i, j) in others:
                    print 'X',
                else:
                    print '.',
            print

        print 'Your last move was %d' % board[len(board) - 1]
        print ' 0'
        print '2 3'
        print ' 1'
        print 'Enter move: ',

        move = int(raw_input())
        return move
