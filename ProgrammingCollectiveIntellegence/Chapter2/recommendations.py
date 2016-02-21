from math import sqrt

critics={'Lisa Rose': {'Lady in the Water': 2.5, 'Snakes on a Plane': 3.5,
'Just My Luck': 3.0, 'Superman Returns': 3.5, 'You, Me and Dupree': 2.5,
'The Night Listener': 3.0},
'Gene Seymour': {'Lady in the Water': 3.0, 'Snakes on a Plane': 3.5,
'Just My Luck': 1.5, 'Superman Returns': 5.0, 'The Night Listener': 3.0,
'You, Me and Dupree': 3.5},
'Michael Phillips': {'Lady in the Water': 2.5, 'Snakes on a Plane': 3.0,
'Superman Returns': 3.5, 'The Night Listener': 4.0},
'Claudia Puig': {'Snakes on a Plane': 3.5, 'Just My Luck': 3.0,
'The Night Listener': 4.5, 'Superman Returns': 4.0,
'You, Me and Dupree': 2.5},
'Mick LaSalle': {'Lady in the Water': 3.0, 'Snakes on a Plane': 4.0,
'Just My Luck': 2.0, 'Superman Returns': 3.0, 'The Night Listener': 3.0,
'You, Me and Dupree': 2.0},
'Jack Matthews': {'Lady in the Water': 3.0, 'Snakes on a Plane': 4.0,
'The Night Listener': 3.0, 'Superman Returns': 5.0, 'You, Me and Dupree': 3.5},
'Toby': {'Snakes on a Plane':4.5,'You, Me and Dupree':1.0,'Superman Returns':4.0}}

# return the similarity critics
def sim_distance(prefs, person1, person2):
    si = {}
    for item in prefs[person1]:
        if item  in prefs[person2]:
            si[item] = 1

    if len(si) == 0: return 0

    sum_sqrt = sum([pow(prefs[person1][item] - prefs[person2][item], 2.0)
                    for item in prefs[person1]
                    if item in prefs[person2]])

    return 1.0 / (1.0 + sqrt(sum_sqrt))

# Pearson correlation
def sim_pearson(prefs, person1, person2):
    si = {}
    for item in prefs[person1]:
        if item  in prefs[person2]:
            si[item] = 1

    if len(si) == 0: return 1

    sum1 = sum(prefs[person1][it] for it in si)
    sum2 = sum(prefs[person2][it] for it in si)

    sum1sq = sum([pow(prefs[person1][it], 2.0) for it in si])
    sum2sq = sum([pow(prefs[person2][it], 2.0) for it in si])

    psum = sum([prefs[person1][it] * prefs[person2][it] for it in si])

    num = psum - (sum1 * sum2 / len(si))
    den = sqrt((sum1sq - pow(sum1, 2) / len(si)) * (sum2sq - pow(sum2, 2) / len(si)))
    if den == 0: return 0

    return num / den

# k neareast neighbors
def topMatches(prefs, person, n = 5, similarity = sim_pearson):
    scores = [(similarity(prefs, person, other), other) for other in prefs if other != person]
    return sorted(scores, reverse = True)[0 : n]

# Recommendation
def getRecommendation(prefs, person, similarity = sim_pearson):
    totals = {}
    simSum = {}
    for other in prefs:
        if other == person: continue
        sim = similarity(prefs, person, other)
        if sim <= 0: continue

        for item in prefs[other]:
            if item not in prefs[person] or prefs[person][item] == 0:
                totals.setdefault(item, 0)
                totals[item] += prefs[other][item] * sim
                simSum.setdefault(item, 0)
                simSum[item] += sim

    rankings = [(total / simSum[item], item) for item, total in totals.items()]

    return sorted(rankings, reverse = True)

def transformPrefs(prefs):
    result = {}
    for person in prefs:
        for item in prefs[person]:
            result.setdefault(item, {})
            result[item][person] = prefs[person][item]
    return result

def calculateSimilarItems(prefs, n = 10):
    result = dict()
    itemPrefs = transformPrefs(prefs)
    c = 0
    for item in itemPrefs:
        c += 1
        if c % 100 == 0: print "%d / %d" % (c, len(itemPrefs))
        scores = topMatches(itemPrefs, item, n = n, similarity = sim_distance)
        result[item] = scores
    return result

def getRecommendedItems(prefs, itemMatch, user):
    userRating = prefs[user]
    scores = dict()
    totalSim = dict()

    for (item, rating) in userRating.items():

        for (similarity, item2) in itemMatch[item]:
            if item2 in userRating: continue
            scores.setdefault(item2, 0)
            scores[item2] += similarity * rating

            totalSim.setdefault(item2, 0)
            totalSim[item2] += similarity

    rankings = [(score / totalSim[item], item) for item, score in scores.items()]

    return sorted(rankings, reverse = True)

def loadMovieLens(path = '/home/yi/workspace/ProgrammingCollectiveIntellegence/Chapter2/ml-100k'):
    movies = {}

    with open(path + '/u.item') as f:
        for line in f:
            (id, title) = line.split('|')[0:2]
            movies[id] = title

    prefs = {}
    with open(path + '/u.data') as f:
        for line in f:
            (user, movieid, rating, ts) = line.split('\t')
            prefs.setdefault(user, {})
            prefs[user][movies[movieid]] = float(rating)
    return prefs
