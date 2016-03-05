import time
import urllib2
import xml.dom.minidom as minidom

kayakKey = 'abc'

def getKayakSession():
    url = 'http://www.kayak.com/k/ident/apisession?token=%s&version=1' % kayakKey
    doc = minidom.parseString(urllib2.urlopen(url).read())
    sId = doc.getElementsByTagName('sid')[0].firstChild.data
    return sId

def flightSearch(sId, origin, destination, departDate):
      url = 'http://www.kayak.com/s/apisearch?basicmode=true&oneway=y'
      url += '&origin=%s&destination=%s' % (origin, destination)
      url += '&depart_date=%s&return_date=none&depart_time=a' % departDate
      url += '&return_time=a&travelers=1&cabin=e&action=doFlights&apimode=1'
      url += '&_sid_=%s&version=1' % sId

      doc = minidom.parseString(urllib2.urlopen(url).read())
      searchId = doc.getElementsByTagName('searchid')[0].firstChild.data
      return searchId

def flightSearchResults(sId, searchId):
    def parsePrice(p):
        return float(p[1:].replace(',',''))

    while 1:
        time.sleep(2)
        url = 'http://www.kayak.com/s/basic/flight?'
        url += 'searchid=%s&c=5&apimode=1&_sid_=%s&version=1' % (searchId, sId)
        doc = minidom.parseString(urllib2.urlopen(url).read())

        morePending = doc.getElementsByTagName('morepending')[0].firstChild
        if morePending == None or morePending.data == 'false': break

    url = 'http://www.kayak.com/s/basic/flight?'
    url += 'searchid=%s&c=999&apimode=1&_sid_=%s&version=1' % (searchId, sId)
    doc = minidom.parseString(urllib2.urlopen(url).read())

    prices = doc.getElementsByTagName('price')
    departures = doc.getElementsByTagName('depart')
    arrivals = doc.getElementsByTagName('arrive')

    return zip([p.firstChild.data.split(' ')[1] for p in departures],
               [p.firstChild.data.split(' ')[1] for p in arrivals],
               [parsePrice(p.firstChild.data) for p in prices])

def createSchedule(people, dest, dep, ret):
    sId = getKayakSession()
    flights = dict()

    for p in people:
        name, origin = p
        searchId = flightSearch(sId, origin, dest, dep)
        flights[(origin, dest)] = flightSearchResults(sId, searchId)

        searchId = flightSearch(sId, dest, origin, ret)
        flights[(dest, origin)] = flightSearchResults(sId, searchId)

    return flights
