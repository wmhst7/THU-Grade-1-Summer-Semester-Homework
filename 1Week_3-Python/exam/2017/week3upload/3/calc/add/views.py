from django.shortcuts import render
from django.http import HttpResponse

# Create your views here.

token=r'{% csrf_token %}'
zc='''
<form method="post" action="/">
    %s
    <table>
    <tr><td>用户名: </td><td><input type="text" name="name" value=""></td></tr>
    <tr><td>密码: </td><td><input type="password" name="pwd" value=""></td></tr>
    <tr><td>确认密码: </td><td><input type="password" name="pwd2" value=""></td></tr>
    <tr><select name="qx">
    <option value="gly">管理员</option>
    <option value="pt">普通用户</option>
    </select></tr>
    <tr><td><input type="submit" value="提交" name="zc"></td></tr>
    <tr><td>%s</td></tr>
    <table>
</form>
'''
dl='''
<form method="post" action="/">
    %s
    <table>
    <tr><td>用户名: </td><td><input type="text" name="name" value=""></td></tr>
    <tr><td>密码: </td><td><input type="password" name="pwd" value=""></td></tr>
    <tr><select name="qx">
    <option value="gly">管理员</option>
    <option value="pt">普通用户</option>
    </select></tr>
    <tr><td><input type="submit" value="登录" name="dl"></td></tr>
    <tr><td><input type="submit" value="注册" name="zc1"></td></tr>
    <tr><td>%s</td></tr>
    <table>
</form>
'''

def wr(zc,ss):
    with open("try.html","w") as f:
        f.write(zc%(token,ss))

user=[]

def calc(request):
    print(request.POST)
    print(user)
    try:
        c=request.POST['zc']
        name=request.POST['name']
        pwd=request.POST['pwd']
        pwd2=request.POST['pwd2']
        qx=request.POST['qx']
        print(name,pwd,qx)
        if name and pwd==pwd2:
            user.append([name,pwd,qx])
            wr(dl,"Success.")
        else:
            wr(zc,"Invalid!")
    except:
        try:
            c=request.POST['zc1']
            wr(zc,"")
        except:
            try:
                c=request.POST['dl']
                name=request.POST['name']
                pwd=request.POST['pwd']
                qx=request.POST['qx']
                flag=0
                for i in user:
                    if i[0]==name and i[1]==pwd and i[2]==qx:
                        wr(dl,"Login Success!")
                        flag=1
                if flag==0:
                    wr(dl,"Login Failure")
            except:
                print("qwq")
                wr(dl,"")
    return render(request,"try.html") #HttpResponse(calculator%(0))
    #if request.POST.has_key('input'):

