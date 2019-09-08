# -*- coding: utf-8 -*-

from django.http import HttpResponse
from django.shortcuts import render_to_response
from django.shortcuts import render
from bs4 import BeautifulSoup

import sqlite3
import jieba
import math
from time import process_time
import re


def search_form(request):
    return render_to_response('mainview.html')

# 接收请求数据


def search(request):
    request.encoding = 'utf-8'
    q = request.GET.get('q')
    print(q)
    start = process_time()
    dbpath = '/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/data.db'
    conn = sqlite3.connect(dbpath)
    c = conn.cursor()
    target =  q
    seggen = jieba.cut_for_search(target)
    score = {}
    for word in seggen:
        if len(word)<2:
            continue
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
    arg = {}
    arg['second'] = passtime
    arg['newsnumber'] = str(len(resname))
    arg['searchtext'] = q
    newreshl = []
    newslist = []
    for line in resheadline:
        temstr = '<font color=mediumaquamarine>' + q + '</font>'
        newreshl.append(line.replace(q,temstr))
    for i, name in enumerate(resname):
        with open('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/Data/'+name+'.html','r') as fin:
            soup = BeautifulSoup(fin.read())
            temtext = soup.find('div',class_='artical-main-content').get_text()[:300]
            temstr = '<font color=mediumaquamarine>' + q + '</font>'
            ttext = temtext.replace(q, temstr)
            ttext+='...'
            newslist.append([name,newreshl[i],ttext])
    #newslist = [x for x in zip(resname, newreshl)]
    #print(newslist)
    page = request.GET.get('page', 1)
    if page == '':
        page = 1
    page = int(page)
    npage = page
    if page*20 < len(newslist):
        mowei = page*20
        npage = page+1
    else:
        mowei = len(newslist)
        npage = page
    arg['newslist'] = newslist[(page-1)*20:mowei]
    if page == 1:
        arg['beforelink'] = '/find/?q='+q+'&page='+str(page)
    else:
        arg['beforelink'] = '/find/?q='+q+'&page='+str(page-1)
    arg['nextlink'] = '/find/?q='+q+'&page='+str(npage)
    return render(request, 'results.html', arg)

