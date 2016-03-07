import re
import math

def getWords(doc):
    splitter = re.compile('\\W*')
    words = [word.lower() for word in splitter.split(doc)
             if len(word) > 2 and len(word) < 20]
    return dict([(word, 1) for word in words])

class classifier:
    def __init__(self, getFeatures, fileName = None):
        self.fc = dict()
        self.cc = dict()
        self.getFeatures = getFeatures
