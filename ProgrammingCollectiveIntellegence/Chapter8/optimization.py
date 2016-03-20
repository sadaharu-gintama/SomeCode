import time
import random
import math

people = [('Seymour', 'BOS'),
          ('Franny', 'DAL'),
          ('Zooey', 'CAK'),
          ('Walt', 'MIA'),
          ('Buddy', 'ORD'),
          ('Les', 'OMA')]

destination = 'LGA'

flights = dict()

for line in file('schedule.txt'):
    origin, dest, depart, arrive, price = line.strip().split(',')
    flights.setdefault((origin, dest), [])
    flights[(origin, dest)].append((depart, arrive, int(price)))


def getMinutes(t):
    x = time.strptime(t, '%H:%M')
    return x[3] * 60 + x[4]

def printSchedule(r):
    for d in range(len(r) / 2):
        name = people[d][0]
        origin = people[d][1]
        out = flights[(origin, destination)][r[2 * d]]
        ret = flights[(origin, destination)][r[2 * d + 1]]
        print "%10s%10s %5s-%5s $%3s %5s-%5s $%3s" % (name, origin,
                                                      out[0], out[1], out[2],
                                                      ret[0], ret[1], ret[2])
def scheduleCost(sol):
    totalPrice = 0
    totalWait = 0
    latestArrival = 0
    earliestDepart = 24 * 60

    for d in range(len(sol) / 2):
        origin = people[d][1]
        out = flights[(origin, destination)][int(sol[2 * d])]
        ret = flights[(origin, destination)][int(sol[2 * d + 1])]

        totalPrice += out[2] + ret[2]

        if latestArrival < getMinutes(out[1]): latestArrival = getMinutes(out[1])
        if earliestDepart > getMinutes(ret[0]): earliestDepart = getMinutes(ret[0])

    for d in range(len(sol) / 2):
        origin = people[d][1]
        out = flights[(origin, destination)][int(sol[2 * d])]
        ret = flights[(origin, destination)][int(sol[2 * d + 1])]
        totalWait += latestArrival - getMinutes(out[1])
        totalWait += getMinutes(ret[0]) - earliestDepart

    if latestArrival > earliestDepart: totalPrice += 50

    return totalWait + totalPrice

def randomOptimize(domain, costf = scheduleCost):
    best = 999999999999
    bestr = None

    for i in range(1000):
        r = [random.randint(domain[i][0], domain[i][1]) for i in range(len(domain))]
        cost = costf(r)
        if cost < best:
            best = cost
            bestr = r

    return r

def hillClimb(domain, costf = scheduleCost):
    sol = [random.randint(domain[i][0], domain[i][1]) for i in range(len(domain))]

    while 1:
        neighbors = list()
        for j in range(len(domain)):
            if sol[j] > domain[j][0]:
                neighbors.append(sol[0:j] + [sol[j] - 1] + sol[j + 1 :])
            if sol[j] < domain[j][1]:
                neighbors.append(sol[0:j] + [sol[j] + 1] + sol[j + 1 :])

        current = costf(sol)
        best = current
        for j in neighbors:
            cost = costf(j)
            if cost < best:
                best = cost
                sol = j

        if best == current:
            break

    return sol

def annealingOptimize(domain, costf = scheduleCost, T = 10000.0, cool = 0.95, step = 1):
    sol = [random.randint(domain[i][0], domain[i][1]) for i in range(len(domain))]

    while T > 0.1:
        i = random.randint(0, len(domain) - 1)
        dir = random.randint(-step, step)
        vec = sol[:]
        vec[i] += dir
        if vec[i] < domain[i][0]: vec[i] = domain[i][0]
        elif vec[i] > domain[i][1]: vec[i] = domain[i][1]

        ca = costf(sol)
        cb = costf(vec)

        if cb < ca or random.random() < pow(math.e, -(cb - ca) / T):
            sol = vec

        T = T * cool

    return sol

def geneticOptimize(domain, costf = scheduleCost, popSize = 50, step = 1,
                    mutProb = 0.2, elite = 0.2, maxIter = 100):
    def mutate(vec):
        i = random.randint(0, len(domain) - 1)
        if random.random < 0.5 and vec[i] > domain[i][0]:
            return vec[0 : i] + [vec[i] - step] + vec[i + 1 :]
        elif vec[i] < domain[i][1]:
            return vec[0 : i] + [vec[i] + step] + vec[i + 1 :]

    def crossOver(r1, r2):
        i = random.randint(1, len(domain) - 2)
        return r1[0 : i] + r2[i :]

    pop = list()
    for i in range(popSize):
        vec = [random.randint(domain[i][0], domain[i][1])
               for i in range(len(domain))]
        pop.append(vec)

    topElite = int(elite * popSize)
    for i in range(maxIter):
        scores = [(costf(v), v) for v in pop if v != None]
        scores.sort()
        ranked = [v for (s, v) in scores]
        pop = ranked[0 : topElite]
        while len(pop) < popSize:
            if random.random() < mutProb:
                pop.append(mutate(ranked[random.randint(0, topElite)]))
            else:
                c1 = random.randint(0, topElite)
                c2 = random.randint(0, topElite)
                pop.append(crossOver(ranked[c1], ranked[c2]))
        print scores[0][0]

    return scores[0][1]
