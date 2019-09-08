# -*- coding: UTF-8 -*-
from django.http import HttpResponse
from django.shortcuts import render
import jieba
import sqlite3

cnamed = {'rockets': '休斯顿火箭（Houston Rockets）', 'grizzlies': '孟菲斯灰熊（Memphis Grizzlies）', 'pelicans': '新奥尔良鹈鹕（New Orleans Pelicans）', 'spurs': '圣安东尼奥马刺（San Antonio Spurs）', 'mavericks': '达拉斯独行侠（Dallas Mavericks）', 'clippers': '洛杉矶快船（Los Angeles Clippers）', 'lakers': '洛杉矶湖人（Los Angeles Lakers）', 'suns': '菲尼克斯太阳（Phoenix Suns）', 'warriors': '金州勇士（Golden State Warriors）', 'kings': '萨克拉门托国王（Sacramento Kings）', 'blazers': '波特兰开拓者（Portland Trail Blazers）', 'timberwolves': '明尼苏达森林狼（Minnesota Timberwolves）', 'thunder': '俄克拉荷马城雷霆（Oklahoma City Thunder）', 'jazz': '犹他爵士（Utah Jazz）', 'nuggets': '丹佛掘金（Denver Nuggets）', 'celtics': '波士顿凯尔特人（Boston Celtics）', 'knicks': '纽约尼克斯（New York Knicks）', 'raptors': '多伦多猛龙（Toronto Raptors）', 'nets': '布鲁克林篮网（Brooklyn Nets）', '76ers': '费城76人（Philadelphia 76ers）', 'hawks': '亚特兰大老鹰（Atlanta Hawks）', 'heat': '迈阿密热火（Miami Heat）', 'hornets': '夏洛特黄蜂（Charlotte Hornets）', 'magic': '奥兰多魔术（Orlando Magic）', 'wizards': '华盛顿奇才（Washington Wizards）', 'bucks': '密尔沃基雄鹿（Milwaukee Bucks）', 'bulls': '芝加哥公牛（Chicago Bulls）', 'cavaliers': '克利夫兰骑士（Cleveland Cavaliers）', 'pistons': '底特律活塞（Detroit Pistons）', 'pacers': '印第安纳步行者（Indiana Pacers）'}

def news(request):
    request.encoding = 'utf-8'
    name = request.path.split('/')[-1]
    with open('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/Data/'+name+'.html','r') as f:
        text = f.read().replace('<html>','').replace('</html>','')
    arg = {'newstext': text}
    return render(request, 'News.html', arg)

def teamlist(request):
    global cnamed
    request.encoding = 'utf-8'
    conn = sqlite3.connect('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/data.db')
    c = conn.cursor()
    c.execute('select * from teamtoplayer')
    dic = {}
    for row in c:
        teamname = row[0]
        score = int(row[2])
        dic[teamname] = score
    sortedlist = sorted(dic.items(), key=lambda d: d[1], reverse=True)
    #print(sortedlist)
    arg = {}
    arg['teamlist'] = sortedlist
    arg['cnamed'] = cnamed
    c.close()
    return render(request, 'TeamList.html',arg)

def team(request):
    global cnamed
    teamname = request.path.split('/')[-1]
    with open('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/TeamData/'+teamname+'.html','r') as f:
        text = f.read()
        arg = {'text': text, 'teamname':teamname}
    newslist = []
    searchtext=cnamed[teamname]
    dbpath = '/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/data.db'
    conn = sqlite3.connect(dbpath)
    c = conn.cursor()
    target = searchtext
    for row in c.execute('select * from teamtoplayer where team=?', (teamname,)):
        target+=row[1]
    seggen = jieba.cut_for_search(target)
    score = {}
    for word in seggen:
        print('得到查询词：', word)
        if len(word)<2:
            continue
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
    for each in sortedlist:
        c.execute('select headline from doc where name=?', (each[0],))
        newslist.append((each[0],c.fetchone()[0]))
    conn.close()
    #print(newslist)
    page = request.GET.get('page',1)
    if page=='':
        page=1
    page = int(page)
    npage = page
    if page*20 < len(newslist):
        mowei = page*20
        npage = page+1
    else:
        mowei=len(newslist)
        npage = page
    arg['newslist'] = newslist[(page-1)*20:mowei]
    if page==1:
        arg['beforelink']='/team/'+teamname
    else:
        arg['beforelink'] = '/team/'+teamname+'?page='+str(page-1)

    arg['nextlink'] = '/team/'+teamname+'?page='+str(npage)
    arg['num'] = str(len(newslist))
    return render(request, "TeamMain.html", arg)

def allnews(request):
    return render(request, 'AllNews.html')

import sys
sys.path.append('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/Mysite')
from Spy import Begin
import threading
import G

def spy(request):
    global t
    request.encoding = 'utf-8'
    arg = {}
    G.flag = False
    if request.GET.get('status')=='b':
        G.flag = True
        arg['zt']='正在爬虫...'
        t = threading.Thread(target=Begin)
        t.start()
    elif request.GET.get('status') == 's':
        G.flag = False
        arg['zt']='暂停爬虫'
    
    return render(request, 'spy.html',arg)

# conn = sqlite3.connect('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/data.db')
# c = conn.cursor()
# c.execute('select * from teamtoplayer')
# dic = {}
# for row in c:
#     teamname = row[0]
#     score = int(row[2])
#     dic[teamname] = score
# sortedlist = sorted(dic.items(), key=lambda d: d[1], reverse=True)
# print(sortedlist)
# c.close()

