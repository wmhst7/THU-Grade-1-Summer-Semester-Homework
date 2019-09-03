from bs4 import BeautifulSoup as bs
import re

def get(str):
    if str.find("(") != -1:
        return str[:str.find("(")]
    return str

dict1 = {}
with open("exam/2017/2/csrank1.html", "r") as f:
    soup = bs(f.read(), "html.parser")

count = 0
a = 0
for each in soup.find_all(class_ = "s1"):
    if each.get_text():
        count+=1
        if count%4 == 1 and count > 4:
            a = int(each.get_text())
        elif count%4 == 2 and count > 4:
            dict1[each.get_text().strip()] = a


dict2 = {}
with open("exam/2017/2/csrank2.html", "r") as f:
    soup = bs(f.read(), "html.parser")
count = 0
a = 0
ming = []
xiao = []
for each in soup.find_all(class_ = "s2"):
    if each.get_text() != "Tie":
        ming.append((int)(each.get_text()[1:]))
for each in soup.find_all(class_ = "s3"):
    xiao.append(each.get_text().strip())
for index, item in enumerate(xiao):
    dict2[item] = ming[index]

dict2 = {}
with open("exam/2017/2/csrank2.html", "r") as f:
    soup = bs(f.read(), "html.parser")
count = 0
a = 0
ming = []
xiao = []
for each in soup.find_all(class_="s2"):
    if each.get_text() != "Tie":
        ming.append((int)(each.get_text()[1:]))
for each in soup.find_all(class_="s3"):
    xiao.append(each.get_text().strip())
for index, item in enumerate(xiao):
    dict2[item] = ming[index]

dict3 = {}
with open("exam/2017/2/csrank3.html", "r") as f:
    soup = bs(f.read(), "html.parser")
name = []
for each in soup.find_all("a", target = "_blank"):
    name.append(" ".join(each.get_text().split()).strip())
for a, each in enumerate(name):
    i = a+1
    if i < 51:
        dict3[each] = i
    elif i > 50 and i < 76:
        dict3[each] = (51+75)/2
    elif 75 > i and i < 101:
        dict3[each] = (75+101)/2
    elif 100 > i and i < 151:
        dict3[each] = (150+101)/2
    elif 150 > i and i < 201:
        dict3[each] = (151+200)/2

dict4 = {}
with open("exam/2017/2/csrank4.html", "r") as f:
    soup = bs(f.read(), "html.parser")

for i, each in enumerate(soup.find_all("a")):
    if(i > 21 and i < 524):
        na = get(" ".join(each.get_text().split())).strip()
        tem = each
        for a in range(12):
            tem = tem.parent
        str = ' '.join(tem.get_text().split())
        str = str.split(" ")[0].replace("=", "").replace("-", "+")
        if(str.find("+") == -1):
            r = int(str)
        else:
            r = eval(str)/2
        dict4[na] = r 

def merge(d1, d2):
    for j in d2.keys():
        for i in d1.keys():
            ok = False
            if i == j:
                #print(i,j)
                d1[i] = (d1[i]*1.0 + d2[j]*1.0)/2
                ok = True
                #print("Modify: {}:{}".format(i, d1[i]))
        if ok == False:
            d1[j] = float(d2[j])

merge(dict1, dict2)
merge(dict1, dict3)
merge(dict1, dict4)
res = sorted(dict1.items(), key=lambda d:d[1], reverse = False)

with open("exam/2017/UniversityRanks.txt","w") as f:
    for key, value in res:
        f.write("{}: {}\n".format(key, value))
