from PIL import Image, ImageDraw
my_data = [line.split('\t') for line in file('decision_tree_example.txt')]

def divideSet(rows, column, value):
    split_function = None
    if isinstance(value, int) or isinstance(value, float):
        split_function = lambda row: row[column] >= value
    else:
        split_function = lambda row: row[column] == value

    set1 = [row for row in rows if split_function(row)]
    set2 = [row for row in rows if not split_function(row)]

    return (set1, set2)

def uniqueCounts(rows):
    results = dict()
    for row in rows:
        r = row[len(row) - 1]
        if r not in results: results[r] = 0
        results[r] += 1
    return results

def giniImpurity(rows):
    total = len(rows)
    counts = uniqueCounts(rows)
    imp = 0.0
    for k1 in counts:
        p1 = float(counts[k1]) / total
        for k2 in counts:
            if k1 == k2: continue
            p2 = float(counts[k2]) / total
            imp += p1 * p2
    return imp

def entropy(rows):
    from math import log
    log2 = lambda x: log(x) / log(2)
    results = uniqueCounts(rows)

    ent = 0.0
    for r in results.keys():
        p = float(results[r]) / len(rows)
        ent = ent - p * log2(p)
    return ent

def variance(rows):
    if len(rows) == 0: return 0
    data = [float(row[len(row) - 1]) for row in rows]
    mean = sum(data) / len(data)
    variance = sum([(d - mean) ** 2 for d in data]) / len(data)
    return variance

class decisionNode:
    def __init__(self, col = -1, value = None, results = None, tb = None, fb = None):
        self.col = col
        self.value = value
        self.results = results
        self.tb = tb
        self.fb = fb

def buildTree(rows, scoref = entropy):
    if len(rows) == 0: return dicisionNode()
    current_score = scoref(rows)

    best_gain = 0.0
    best_criteria = None
    best_sets = None

    column_count = len(rows[0]) - 1
    for col in range(0, column_count):
        column_values = dict()
        for row in rows:
            column_values[row[col]] = 1

        for value in column_values.keys():
            (set1, set2) = divideSet(rows, col, value)

            p = float(len(set1)) / len(rows)
            gain = current_score - p * scoref(set1) - (1.0 - p) * scoref(set2)

            if gain > best_gain and len(set1) > 0 and len(set2) > 0:
                best_gain = gain
                best_criteria = (col, value)
                best_sets = (set1, set2)

    if best_gain > 0:
        trueBranch = buildTree(best_sets[0])
        falseBranch = buildTree(best_sets[1])
        return decisionNode(col = best_criteria[0], value = best_criteria[1],
                            tb = trueBranch, fb = falseBranch)
    else:
        return decisionNode(results = uniqueCounts(rows))

def printTree(tree, indent = ' '):
    if tree.results != None:
        print str(tree.results)
    else:
        print str(tree.col) + ':' + str(tree.value) + '?'
        print indent + 'T->',
        printTree(tree.tb, indent + ' ')
        print indent + 'F->',
        printTree(tree.fb, indent + ' ')

def getWidth(tree):
    if tree.tb == None and tree.fb == None: return 1
    return getWidth(tree.fb) + getWidth(tree.tb)

def getDepth(tree):
    if tree.tb == None and tree.fb == None: return 0
    return max(getDepth(tree.tb), getDepth(tree.fb)) + 1

def drawTree(tree, jpeg = 'tree.jpg'):
    w = getWidth(tree) * 100
    h = getDepth(tree) * 100 + 120

    img = Image.new('RGB', (w, h), (255, 255, 255))
    draw = ImageDraw.Draw(img)

    drawNode(draw, tree, w / 2, 20)
    img.save(jpeg, 'JPEG')

def drawNode(draw, tree, x, y):
    if tree.results == None:
        w1 = getWidth(tree.fb) * 100
        w2 = getWidth(tree.tb) * 100
        left = x - (w1 + w2) / 2
        right = x + (w1 + w2) / 2
        draw.text((x - 20, y - 10),
                  str(tree.col) + ':' + str(tree.value),
                  (0, 0, 0))

        draw.line((x, y, left + w1 / 2, y + 100), fill = (255, 0, 0))
        draw.line((x, y, right - w2 / 2, y + 100), fill = (255, 0, 0))

        drawNode(draw, tree.fb, left + w1 / 2, y + 100)
        drawNode(draw, tree.tb, right - w2 / 2, y + 100)
    else:
        txt = ' \n'.join(['%s:%d' % v for v in tree.results.items()])
        draw.text((x - 20, y), txt, (0, 0, 0))

def classify(observation, tree):
    if tree.results != None:
        return tree.results
    else:
        v = observation[tree.col]
        branch = None
        if isinstance(v, int) or isinstance(v, float):
            if v >= tree.value: branch = tree.tb
            else: branch = tree.fb
        else:
            if v == tree.value: branch = tree.tb
            else: branch = tree.fb
        return classify(observation, branch)

def prune(tree, minGain):
    if tree.tb.results == None:
        prune(tree.tb, minGain)
    if tree.fb.results == None:
        prune(tree.fb, minGain)

    if tree.tb.results != None and tree.fb.results != None:
        tb, fb = list(), list()
        for v, c in tree.tb.results.items():
            tb += [[v]] * c
        for v, c in tree.fb.results.items():
            fb += [[v]] * c

        delta = entropy(tb + fb) - (entropy(tb) + entropy(fb) / 2)
        if delta < minGain:
            tree.tb, tree.fb = None, None
            tree.results = uniqueCounts(tb + fb)

def mdClassify(observation, tree):
    if tree.results != None: return tree.results

    v = observation[tree.col]
    if v == None:
        tr, fr = mdClassify(observation, tree.tb), mdClassify(observation, tree.fb)
        tCount = sum(tr.values())
        fCount = sum(fr.values())
        tw = float(tCount) / (tCount + fCount)
        fw = float(fCount) / (tCount + fCount)
        result = dict()
        for k, v in tr.items(): result[k] = v * tw
        for k, v in fr.items():
            if k not in result: result[k] = 0
            result[k] += v * fw
        return result
    else:
        if isinstance(v, int) or isinstance(v, float):
            if v >= tree.value: branch = tree.tb
            else: branch = tree.fb
        else:
            if v == tree.value: branch = tree.tb
            else: branch = tree.fb
        return mdClassify(observation, branch)
