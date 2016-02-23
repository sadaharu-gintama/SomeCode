from bs4 import BeautifulSoup
import urllib2
import re

chare = re.compile(r'[!-\.&]')
itemOwners = list()

dropWords = ['a', 'new', 'some', 'more', 'my', 'own', 'the', 'many', 'other', 'another']

currentUser = 0

for i in range(1, 51):
    c = urllib2.urlopen(
        'http://member.zebo.com/Main?event_key=USERSEARCH&wiowiw=wiw&keyword=car&page=%d' % (i)
    )
    soup = BeautifulSoup(c.read())
    for td in soup('td'):
        if ('class' in dict(td.attrs) and td['class'] == 'bgverdanasmall'):
            item = [re.sub(chare, '', a.contents[0].lower().strip()) for a in td('a')]
            for item in items:
                txt = ' '.join([t for t in item.split(' ') if t not in dropWords])
                if len(txt) < 2: continue
                itemOwners.setdefault(txt, {})
                itemOwners[txt][currentUser] = 1
            currentUser += 1

out = file('zabo.txt', 'w')
out.write('Item')
for user in range(0, currentUser): out.write('\tU%d' % user)
out.write('\n')
for item, owners in itemOwners.items():
    if len(owners) > 10:
        out.write(item)
        for user in range(0, currentUser):
            if user in owners: out.write('\t1')
            else: out.write('\t0')
        out.write('\n')
