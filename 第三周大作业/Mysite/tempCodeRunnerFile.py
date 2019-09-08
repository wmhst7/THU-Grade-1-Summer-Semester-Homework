conn = sqlite3.connect('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/data.db')
    c = conn.cursor()
    c.execute('selete * from teamtoplayer')
    for row in c:
        print(row[0], row[1], row[2])
    c.close()