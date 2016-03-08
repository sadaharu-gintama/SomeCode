import feedparser
import re

def read(feed, classifier):
    f = feedparser.parse(feed)
    for entry in f['entries']:
        print '-----'
        print 'Title:\t' + entry['title'].encode('utf-8')
        print 'Publisher:\t' + entry['publisher'].encode('utf-8')
        print ''
        print entry['summary'].encode('utf-8')

        fullText = '%s\n%s\n%s' % (entry['title'], entry['publisher'],
                                   entry['summary'])
        #print 'Guess:\t' + str(classifier.classify(fullText))
        print 'Guess:\t' + str(classifier.classify(entry))
        cl = raw_input('Enter Category:')
        #classifier.train(fullText, cl)
        classifier.train(entry, cl)

def entryFeatures(entry):
    splitter = re.compile('\\W*')
    f = dict()

    titleWords = [s.lower() for s in splitter.split(entry['title'])
                  if len(s) > 2 and len(s) < 20]
    for w in titleWords: f['Title:' + w] = 1

    summaryWords = [s.lower() for s in splitter.split(entry['summary'])
                    if len(s) > 2 and len(s) < 20]

    uc = 0
    for i in range(len(summaryWords)):
        w = summaryWords[i]
        f[w] = 1
        if w.isupper(): uc += 1

        if i < len(summaryWords) - 1:
            twoWords = ' '.join(summaryWords[i : i + 1])
            f[twoWords] = 1

    f['Publisher:' + entry['publisher']] = 1
    if float(uc) / len(summaryWords) > 0.3: f['UPPERCASE'] = 1
    return f
