import math
import random

dorms = ['Zeus', 'Athena', 'Hercules', 'Bacchus', 'Pluto']

prefs=[('Toby', ('Bacchus', 'Hercules')),
       ('Steve', ('Zeus', 'Pluto')),
       ('Karen', ('Athena', 'Zeus')),
       ('Sarah', ('Zeus', 'Pluto')),
       ('Dave', ('Athena', 'Bacchus')),
       ('Jeff', ('Hercules', 'Pluto')),
       ('Fred', ('Pluto', 'Athena')),
       ('Suzie', ('Bacchus', 'Hercules')),
       ('Laura', ('Bacchus', 'Hercules')),
       ('James', ('Hercules', 'Athena'))]

domain = [(0, (len(dorms) * 2) - i - 1) for i in range(0, len(dorms) * 2)]

def printSolution(vec):
    slots = list()
    for i in range(len(dorms)): slots += [i, i]

    for i in range(len(vec)):
        x = int(vec[i])

        dm = dorms[slots[x]]
        print prefs[i][0], dm

        del slots[x]

def dormCost(vec):
    cost = 0
    slots = [0, 0, 1, 1, 2, 2, 3, 3, 4, 4]

    for i in range(len(vec)):
        x = int(vec[i])
        dm = dorms[slots[x]]
        pref = prefs[i][1]

        if pref[0] == dm: cost += 0
        elif pref[1] == dm: cost += 1
        else: cost += 3

        del slots[x]

    return cost
