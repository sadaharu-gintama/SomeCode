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
