import requests
import re
from bs4 import BeautifulSoup
import time, random
import G

def spynews(url):
    if G.flag==False:
        return
    headers = {'User-agent': 'Mozilla/5.0'}
    try:
        page = requests.get(url, headers=headers, timeout=3)
    except:
        return 'False', ''
    content = page.text
    soup = BeautifulSoup(content, 'lxml')
    title = soup.find("div", class_ = "artical-title")
    body = soup.find("div", class_="artical-content")
    loadpath = "/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/Data/"+url.split("/")[-1].replace('html','txt')
    with open(loadpath, "w") as f:
        f.write("<html><body>\n")
        f.write(title.prettify())
        f.write(body.prettify())
        f.write("</body></html>\n")
        print(url)
    time.sleep(0.1)
    
def spynewslist():
    count = 0
    for i in range(2):
        listurl = "https://voice.hupu.com/nba/"+str(i)
        print(i)
        headers = {'User-agent': 'Mozilla/5.0'}
        try:
            page = requests.get(listurl, headers=headers, timeout=3)
        except:
            return 'False', ''
        content = page.text
        soup = BeautifulSoup(content, 'lxml')
        res = soup.find_all("div", class_ = "list-hd")
        for each in res:
            if G.flag == False:
                return
            url = each.find("a")["href"]
            count += 1
            spynews(url)

def spyteam(c, u):
    global di
    name = u.split('/')[-1]
    path = u
    headers = {'User-agent': 'Mozilla/5.0'}
    try:
        page = requests.get(path, headers=headers, timeout=3)
    except:
        return 'False', ''
    content = page.text
    soup = BeautifulSoup(content, 'lxml')
    with open('/Users/wmhst7/Desktop/SS/1Week_3-Python/Mysite/TeamData/'+name+'.html','w') as f:
        f.write('<h1>')
        f.write(soup.find('div', class_='team_data').find('span', class_='title-text').prettify())
        f.write('</h1>')
        f.write(soup.find('div', class_='team_data').find('div', class_='content_a').find('div', class_='clearfix').prettify())
        f.write(soup.find('div', class_='team_data').find('div', class_='content_a').find('div', class_='txt').prettify())
        f.write(soup.find('div', class_='team_list_data').find('div', class_='a').prettify())
    cname = soup.find('span',class_='title-text').get_text().strip()
    di[name] = cname


di={}
def spyteams():
    global di
    path = 'https://nba.hupu.com/teams'
    headers = {'User-agent': 'Mozilla/5.0'}
    try:
        page = requests.get(path, headers=headers, timeout=3)
    except:
        return 'False', ''
    content = page.text
    soup = BeautifulSoup(content, 'lxml')
    soup = soup.find("div", class_='gamecenter_content')
    urls = [x['href'] for x in soup.find_all('a')]
    c=0
    for u in urls:
        c+=1
        spyteam(c, u)
    print(di)

import Preload
def Begin():
    spynewslist()
    Preload.main()
    print('结束爬虫')


