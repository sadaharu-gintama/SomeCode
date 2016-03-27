import nmf
import urllib2
from numpy import *

tickers = ['YHOO', 'AVP', 'BIIB', 'BP', 'CL', 'CVX',
           'DNA', 'EXPE', 'GOOG', 'PG', 'XOM', 'AMGN']

shortest = 300
prices = dict()
dates = None

for t in tickers:
    rows=urllib2.urlopen('http://ichart.finance.yahoo.com/table.csv?'+\
                         's=%s&d=11&e=26&f=2006&g=d&a=3&b=12&c=1996'%t +\
                         '&ignore=.csv').readlines()

    prices[t] = [float(r.split(',')[5]) for r in rows[1: ] if r.strip() != '']
    if len(prices[t]) < shortest: shortest = len(prices[t])

    if not dates:
        dates = [r.split(',')[0] for r in rows[1: ] if r.strip() != '']

ll = [[prices[tickers[i]][j]
       for i in range(len(tickers))]
       for j in range(shortest)]

w, h = nmf.factorize(matrix(ll), pc = 5)

print h
print w

for i in range(shape(h)[0]):
    print 'Feature %d' % i
    ol = [(h[i, j], tickers[j]) for j in range(shape(h)[1])]
    ol.sort()
    ol.reverse()

    for i in range(12):
        print ol[j]

    print
    porder = [(w[d, i], d) for d in range(300)]
    porder.sort()
    porder.reverse()
    print [(p[0], dates[p[1]]) for p in porder[0 : 3]]
    print
