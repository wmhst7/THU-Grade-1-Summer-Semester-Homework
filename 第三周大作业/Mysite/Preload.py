import os
import sys
import re
import chardet
import jieba
import sqlite3
import importlib
from time import process_time
from bs4 import BeautifulSoup
import G

cnnamed = {'rockets': '休斯顿火箭（Houston Rockets）', 'grizzlies': '孟菲斯灰熊（Memphis Grizzlies）', 'pelicans': '新奥尔良鹈鹕（New Orleans Pelicans）', 'spurs': '圣安东尼奥马刺（San Antonio Spurs）', 'mavericks': '达拉斯独行侠（Dallas Mavericks）', 'clippers': '洛杉矶快船（Los Angeles Clippers）', 'lakers': '洛杉矶湖人（Los Angeles Lakers）', 'suns': '菲尼克斯太阳（Phoenix Suns）', 'warriors': '金州勇士（Golden State Warriors）', 'kings': '萨克拉门托国王（Sacramento Kings）', 'blazers': '波特兰开拓者（Portland Trail Blazers）', 'timberwolves': '明尼苏达森林狼（Minnesota Timberwolves）', 'thunder': '俄克拉荷马城雷霆（Oklahoma City Thunder）', 'jazz': '犹他爵士（Utah Jazz）', 'nuggets': '丹佛掘金（Denver Nuggets）', 'celtics': '波士顿凯尔特人（Boston Celtics）', 'knicks': '纽约尼克斯（New York Knicks）', 'raptors': '多伦多猛龙（Toronto Raptors）', 'nets': '布鲁克林篮网（Brooklyn Nets）', '76ers': '费城76人（Philadelphia 76ers）', 'hawks': '亚特兰大老鹰（Atlanta Hawks）', 'heat': '迈阿密热火（Miami Heat）', 'hornets': '夏洛特黄蜂（Charlotte Hornets）', 'magic': '奥兰多魔术（Orlando Magic）', 'wizards': '华盛顿奇才（Washington Wizards）', 'bucks': '密尔沃基雄鹿（Milwaukee Bucks）', 'bulls': '芝加哥公牛（Chicago Bulls）', 'cavaliers': '克利夫兰骑士（Cleveland Cavaliers）', 'pistons': '底特律活塞（Detroit Pistons）', 'pacers': '印第安纳步行者（Indiana Pacers）'}
dic = {} #文件序号到新闻标题的映射

def getdic():
    path = "/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/Data" 
    files = os.listdir(path)
    global dic
    c = 0
    for file in files:
        if not os.path.isdir(file) and file[-3:]=='txt' and file[:3]!='.DS': 
            with open(path+"/"+file, "r") as f: 
                if G.flag == False:
                    return
                soup = BeautifulSoup(f.read(), 'lxml')
                name = file[:-4]
                print(name)
                str = soup.find('h1', class_='headline').get_text().strip()
                print(str)
                c+=1
                #print(c)
                dic[name] = str

def HeadlineSplit():
    global dic
    #num = 0
    conn = sqlite3.connect('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/data.db')
    c = conn.cursor()
    for name, headline in dic.items():
        if G.flag == False:
            return
        print(name, headline)
        seglist = list(jieba.cut_for_search(headline))
        #print(seglist)
        rows = c.execute('select * from doc where name=?',(name,))
        flag=0
        for row in rows:
            flag=1
        if flag==0:
            c.execute('insert into doc values(?,?)', (name, headline))
        for word in seglist:
            c.execute('select namelist from word where term=?', (word,))
            #print('select namelist from word where term={}'.format(word))
            result = c.fetchall()
            if len(result) == 0:
                c.execute('insert into word values(?,?)', (word, name))
                #print('insert into word value({},{})'.format(word, name))
            else:
                temstr = result[0][0]
                temstr += ' ' + name
                c.execute('update word set namelist=? where term=?', (temstr, word))
                #print('update word set namelist={} where term={}'.format(temstr, word))
        conn.commit()
    conn.close()


def process():
    global dic
    conn = sqlite3.connect('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/data.db')
    c = conn.cursor()
    #c.execute('drop table doc')
    #c.execute('create table doc (name text primary key,headline text)')
    #c.execute('drop table word')
    #c.execute('create table word (term varchar(25) primary key, namelist text)')
    conn.commit()
    conn.close()
    HeadlineSplit()


def Searchnum(searchtext):
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
    conn.close()
    return len(sortedlist)

def preloadteam():
    path = "/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/TeamData"
    files = os.listdir(path)
    conn = sqlite3.connect('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/data.db')
    c = conn.cursor()
    c.execute('create table teamtoplayer (team varchar(25) primary key, players text, score int,cname text)')
    for file in files:
        if not os.path.isdir(file):
            with open(path+"/"+file, "r") as f:
                teamname = file[:-5]
                #print(path+"/"+file)
                soup = BeautifulSoup(f.read(), 'lxml')
                players = [x.get_text().strip() for x in soup.find_all('span', class_='c2')]
                print(teamname, players)
                for p in players:
                    if p == '球员':
                        continue
                    c.execute('select players from teamtoplayer where team=?', (teamname,))
                    #print('select players from teamtoplayer where team={}'.format(teamname))
                    result = c.fetchall()
                    if len(result) == 0:
                        c.execute('insert into teamtoplayer values(?,?,?,?)', (teamname, p,0,cnnamed[teamname]))
                        #print('insert into teamtoplayer values({},{})'.format(teamname, p))
                    else:
                        temstr = result[0][0]
                        temstr += ' ' + p
                        c.execute('update teamtoplayer set players=? where team=?',(temstr, teamname))
                        #print('update teamtoplayer set players={} where team={}'.format(temstr, teamname))
                    conn.commit()
                #球队
                num = Searchnum(cnnamed[teamname])*800
                c.execute('update teamtoplayer set score=? where team=?', (num, teamname))
                for p in players:
                    if p == '球员':
                        continue
                    num = Searchnum(p)
                    c.execute('select team, score from teamtoplayer where team=?',(teamname,))
                    for row in c:
                        num+=int(row[1])
                    c.execute('update teamtoplayer set score=? where team=?', (num, teamname))
                    print(teamname, num)
                    conn.commit()

def jiachaolianjie():
    conn = sqlite3.connect('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/data.db')
    c = conn.cursor()
    rows = c.execute('select * from teamtoplayer')
    d = {}
    for row in rows:
        if G.flag == False:
            return
        tname = row[3].split('（')[0]
        tem = 0
        for each in jieba.cut(tname):
            if tem>0:
                d[each] = '<a href="/team/'+row[0]+'">'+each+'</a>'
            tem += 1
        players = re.split(' |-',row[1])
        for p in players:
            if p=='科比' or p=='詹姆斯' or p=='库里' or len(p)>1:
                strt='<a href="/team/'+row[0]+'">'+p+'</a>'
                d[p] = strt
    
    print(d)
    c.close()
    #return
    path = "/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/Data"
    files = os.listdir(path)
    pattern = re.compile('|'.join(d.keys()))
    for file in files:
        if not os.path.isdir(file) and file[-3:] == 'txt' and file[:3] != '.DS':
            with open(path+"/"+file, "r") as f:
                with open(path+'/'+file[:-4]+'.html', 'w') as out:
                    #print(file)
                    for line in f:
                        if line.find('img')>-1 or line.find('href')>-1:
                            out.write(line)
                            continue
                        out.write(pattern.sub(lambda x: d[x.group()], line))


def main():
    #start = process_time()
    global dic
    getdic()
    process()
    jiachaolianjie()
    #end = process_time()
    #time = end - start
    #print (str(time)+"s")
#main()
#jiachaolianjie()

# conn = sqlite3.connect('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/data.db')
# c = conn.cursor()
# c.execute('drop table doc')
# c.execute('drop table word')
# c.execute('create table doc (name text primary key,headline text)')
# c.execute('create table word (term varchar(25) primary key, namelist text)')

# getdic()
# process()
# jiachaolianjie()
# main()

