import sqlite3
import jieba
import math
from time import process_time

dbpath = '/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/data.db'


def Bsearch(searchtext):
    start = process_time()
    dbpath = '/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/data.db'
    conn = sqlite3.connect(dbpath)
    c = conn.cursor()
    target = searchtext
    seggen = jieba.cut_for_search(target)
    score = {}  
    for word in seggen:
        print('得到查询词：', word)
        c.execute('select namelist from word where term=?', (word,))
        result = c.fetchall()
        if len(result) > 0:
            doclist = result[0][0]
            getlist = doclist.split(' ')
            #print(getlist)
            for each in getlist:
                if each in score:
                    score[str(each)] += 1
                else:
                    score[str(each)] = 1
    sortedlist = sorted(score.items(), key=lambda d: d[1], reverse=True)
    #很牛逼，这个表达式。
    #print('得分列表',sortedlist)
    
    resname = []
    resheadline = []
    for name, scoreint in sortedlist:
        
        c.execute('select headline from doc where name=?', (name,))
        resheadline.append(c.fetchall()[0][0])
        resname.append(name)
        #print("当前关联度排名：", cnt)
        #print('新闻：', headline, '匹配度：', scoreint)
    conn.close()
    end = process_time()
    passtime = str(end - start)
    with open('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/templates/results.html','w') as f:
        htmltext = '''
<html>
<head>
<meta charset="utf-8"> 
<title>搜索结果</title> 
<style>
body
{
	background-color:tomato;
}
</style>
</head>
<body>
<h1>搜索结果</h1>
<p>搜索用时：'''+passtime+'''</p>
<p>搜索条数：'''+str(len(resname))+'''</p>
<br>
<br>
<br>'''
        for i, n in enumerate(resname):
            hline = resheadline[i]
            htmltext += '<p><a href="/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/Data/>'
            htmltext+=n
            htmltext+='.html">'
            htmltext+=hline
            htmltext+='</a></p>'
        htmltext+='''</body></html>
        '''
        f.write(htmltext)
    

def main(searchtext):
    global dbpath
    Bsearch(searchtext)


