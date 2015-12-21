import sqlite3
import re

conn = sqlite3.connect('test_db.sqlite')
cur = conn.cursor()

cur.execute('''
DROP TABLE IF EXISTS Counts''')

cur.execute('''
CREATE TABLE Counts(org TEXT, count INTEGER)''')

file = 'mbox.txt'

fn = open(file)

for line in fn:
    if not line.startswith('From: '):
        continue
    email = line.split(' ')[1]
    org = email.split('@')[1].strip()
    print org
    cur.execute('SELECT count FROM Counts WHERE org = ?', (org, ))
    row = cur.fetchone()
    if row is None:
        cur.execute('''
        INSERT INTO Counts (org, count) VALUES (?, 1)''', (org, ))
    else:
        cur.execute('''
        UPDATE Counts SET count = count + 1 WHERE org = ?''', (org, ))
    conn.commit()

sql_query = '''SELECT count FROM Counts'''
total = 0
for r in cur.execute(sql_query):
    total += r[0]

print total
fn.close()
cur.close()
