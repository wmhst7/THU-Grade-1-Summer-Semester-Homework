#include "kernel.h"
#include <QDebug>

Kernel::Kernel(QObject *parent) : QObject(parent)
{
    OutputPosition = Position(8, 1);
    InputPositions << Position(1,1);

    ColorName<<"black"<<"deepskyblue"<<"tomato"<<"yellow"<<"violet"
            <<"royalblue"<<"olive"<<"peru"<<"darkorchid"<<"seagreen"
         <<"midnightblue"<<"gold"<<"skyblue"<<"lightslategray"<<"cadetblue"<<"steelblue"
        <<"mediumseagreen"<<"darkgoldenrod"<<"peachpuff"<<"orangered"<<"palegreen";

}

Position::Position(int x, int y) : X(x), Y(y){}

bool Kernel::onBorder(Position& p){
    if(p.X == 1 || p.X == Width || p.Y == 1 || p.Y == Height)
        return true;
    else
        return false;
}

void Kernel::UpdateMix(){
    //chu li Mix
    QVector<QString> temV;
    for(auto it : Orders){
        if(it.indexOf("Mix") != -1){
            QVector<int> intV;
            QString tem = it.mid(it.indexOf("Mix") + 4, it.size()-5);
            qDebug()<<tem;
            QStringList t = tem.split(",");
            for(auto item : t){
                int i = item.toInt();
                intV<<i;
                //qDebug()<<i;
            }
            int time = intV[0];
            for(int a = 1;a<intV.size()-2;a+=2){
                QString order = "Move " + QString::number(time + (a-1)/2)
                        +","+ QString::number(intV[a])
                        +","+ QString::number(intV[a+1])
                        +","+ QString::number(intV[a+2])
                        +","+ QString::number(intV[a+3])+";";
                temV<<order;
                qDebug()<<order;
            }
        }
    }
    for(auto each : temV)
        Orders<<each;
    for(auto each : Orders)
        qDebug()<<each;
}


QString Kernel::GetColorName(int i){
    return ColorName[i % ColorName.size()];
}

void Kernel::Input(int t, int x, int y){
    if(Time != t) return;
    for(auto it : InputPositions){
        if(x == it.X && y == it.Y) {
            CheckConstraint(x,y);
            KindNumber++;
            CubeData[x][y].kind = KindNumber;
            CubeData[x][y].PolluteKinds << KindNumber;
            CubeData[x][y].Expanding = false;
            return;
        }
    }
    emit(Error("Input"));
}

void Kernel::BInput(int t, int x, int y){
    if(Time != t) return;
    CubeData[x][y].kind = -1;
    CubeData[x][y].PolluteKinds.remove(CubeData[x][y].PolluteKinds.size()-1);
    CubeData[x][y].Expanding = false;
    KindNumber--;

}

void Kernel::Output(int t, int x, int y){
    if(Time != t) return;
    if(x != OutputPosition.X || y != OutputPosition.Y){
        emit(Error("Output"));
        return;
    }
    CubeData[x][y].kind = -1;
    CubeData[x][y].Expanding = false;
    if(!CubeData[x][y].PolluteKinds.contains(CubeData[x][y].kind) && CubeData[x][y].kind!=-1)
        CubeData[x][y].PolluteKinds<<CubeData[x][y].kind;
}

void Kernel::BOutput(int t, int x, int y){
    if(Time != t) return;
    CubeData[x][y].kind = CubeData[x][y].PolluteKinds[CubeData[x][y].PolluteKinds.size()-1];
    CubeData[x][y].Expanding = false;
    CubeData[x][y].PolluteKinds.remove(CubeData[x][y].PolluteKinds.size()-1);
}

void Kernel::Move(int t, int x1, int y1, int x2, int y2){
    if(Time == t){
        Cube& c1 = CubeData[x1][y1];
        Cube& c2 = CubeData[x2][y2];
        CheckConstraint(x2,y2);
        CheckConstraint(x1,y1);
        c2.kind = c1.kind;
        c1.kind = -1;
        if(!c2.PolluteKinds.contains(c2.kind) && c2.kind != -1)
            c2.PolluteKinds<<c2.kind;
        if(Sound)
            QSound::play(":/Sound/Move.wav");
    }
}

void Kernel::BMove(int t, int x1, int y1, int x2, int y2){
    if(Time == t){
        Cube& c1 = CubeData[x1][y1];
        Cube& c2 = CubeData[x2][y2];
        c1.kind = c2.kind;
        c2.kind = -1;
        if(c2.PolluteKinds.size() > 0 && c2.PolluteKinds[c2.PolluteKinds.size()-1] == c1.kind){
            c2.PolluteKinds.remove(c2.PolluteKinds.size()-1);
        }
    }
}

void Kernel::Split(int t, int x1, int y1, int x2, int y2, int x3, int y3){
    if(Time == t){
        Cube& c1 = CubeData[x1][y1];
        Cube& c2 = CubeData[x2][y2];
        Cube& c3 = CubeData[x3][y3];
        c1.Expanding = true;
        c1.Other = Position(0, 0);
        c2.kind = c1.kind;
        c3.kind = c1.kind;
        c2.Expanding = true;
        c3.Expanding = true;
        c2.Other = Position(x3, y3);
        c3.Other = Position(x2, y2);
//        if(!c2.PolluteKinds.contains(c1.kind) && c1.kind != -1)
//            c2.PolluteKinds<<c1.kind;
//        if(!c3.PolluteKinds.contains(c1.kind) && c1.kind != -1)
//            c3.PolluteKinds<<c1.kind;
        if(Sound)
            QSound::play(":/Sound/1.wav");
        CheckConstraint(x1,y1);
        CheckConstraint(x2,y2);
        CheckConstraint(x3,y3);
    }else if(Time == t + 1){
        Cube& c1 = CubeData[x1][y1];
        Cube& c2 = CubeData[x2][y2];
        Cube& c3 = CubeData[x3][y3];
        c1.Expanding = false;
        c1.kind = -1;
        KindNumber++;
        c2.kind = KindNumber;
        c2.PolluteKinds<<c2.kind;
        KindNumber++;
        c3.kind = KindNumber;
        c3.PolluteKinds<<c3.kind;
        c2.Expanding = false;
        c3.Expanding = false;
        c2.Other = Position(0,0);
        c3.Other = Position(0,0);
        if(Sound)
            QSound::play(":/Sound/2.wav");
        CheckConstraint(x2,y2);
        CheckConstraint(x3,y3);
    }
}

void Kernel::BSplit(int t, int x1, int y1, int x2, int y2, int x3, int y3){
    if(Time == t){
        Cube& c1 = CubeData[x1][y1];
        Cube& c2 = CubeData[x2][y2];
        Cube& c3 = CubeData[x3][y3];

        c2.kind = -1;
        c3.kind = -1;

        c1.Expanding = false;
        c1.Other = Position(0, 0);
        c2.Expanding = false;
        c3.Expanding = false;
        c2.Other = Position(0, 0);
        c3.Other = Position(0, 0);

    }else if(Time == t + 1){
        Cube& c1 = CubeData[x1][y1];
        Cube& c2 = CubeData[x2][y2];
        Cube& c3 = CubeData[x3][y3];
        c1.kind = c1.PolluteKinds[c1.PolluteKinds.size()-1];
        c2.kind = c1.kind;
        c3.kind = c1.kind;
        c1.Expanding = true;
        c2.Expanding = true;
        c3.Expanding = true;
        KindNumber-=2;
        if(c2.PolluteKinds.size() > 0)
            c2.PolluteKinds.remove(c2.PolluteKinds.size()-1);
        if(c3.PolluteKinds.size() > 0)
            c3.PolluteKinds.remove(c3.PolluteKinds.size()-1);
        c2.Other = Position(x3, y3);
        c3.Other = Position(x2, y2);

    }
}


void Kernel::Merge(int t, int x1, int y1, int x2, int y2){
    int x3 = (x1 + x2)/2;
    int y3 = (y1 + y2)/2;
    if(Time == t){
        Cube& c1 = CubeData[x1][y1];
        Cube& c2 = CubeData[x2][y2];
        Cube& c3 = CubeData[x3][y3];
        c1.Expanding = true;
        c2.Expanding = true;
        c3.Expanding = true;
        c3.Other = Position(0, 0);
        c1.Other = Position(x2, y2);
        c2.Other = Position(x1, y1);
        KindNumber++;
        c1.kind = KindNumber;
        c2.kind = KindNumber;
        c3.kind = KindNumber;
        //c1.PolluteKinds<<c1.kind;
        //c2.PolluteKinds<<c2.kind;
        c3.PolluteKinds<<c3.kind;
        CheckConstraint(x1,y1);
        CheckConstraint(x2,y2);
        CheckConstraint(x2,y3);
    }else if(Time == t + 1){
        Cube& c1 = CubeData[x1][y1];
        Cube& c2 = CubeData[x2][y2];
        Cube& c3 = CubeData[x3][y3];
        c1.Expanding = false;
        c2.Expanding = false;
        c3.Expanding = false;
        c1.Other = Position(0, 0);
        c2.Other = Position(0, 0);
        c3.Other = Position(0, 0);
        c1.kind = -1;
        c2.kind = -1;
        if(Sound)
            QSound::play(":/Sound/Merge.wav");
        CheckConstraint(x3,y3);
    }
}

void Kernel::BMerge(int t, int x1, int y1, int x2, int y2){
    int x3 = (x1 + x2)/2;
    int y3 = (y1 + y2)/2;
    if(Time == t){
        Cube& c1 = CubeData[x1][y1];
        Cube& c2 = CubeData[x2][y2];
        Cube& c3 = CubeData[x3][y3];
        c1.Expanding = false;
        c2.Expanding = false;
        c3.Expanding = false;
        c3.Other = Position(0, 0);
        c1.Other = Position(0, 0);
        c2.Other = Position(0, 0);

        if(c1.PolluteKinds.size() > 0)
            c1.kind = c1.PolluteKinds[c1.PolluteKinds.size()-1];
        if(c2.PolluteKinds.size() > 0)
            c2.kind = c2.PolluteKinds[c2.PolluteKinds.size()-1];
        c3.kind = -1;
        if(c3.PolluteKinds.size() > 0)
            c3.PolluteKinds.remove(c3.PolluteKinds.size()-1);

    }else if(Time == t + 1){
        Cube& c1 = CubeData[x1][y1];
        Cube& c2 = CubeData[x2][y2];
        Cube& c3 = CubeData[x3][y3];
        c1.Expanding = true;
        c2.Expanding = true;
        c3.Expanding = true;
        c1.Other = Position(x2, y2);
        c2.Other = Position(x1, y1);
        c3.Other = Position(0, 0);
        c1.kind = c3.kind;
        c2.kind = c3.kind;
//        c1.PolluteKinds.remove(c1.PolluteKinds.size()-1);
//        c2.PolluteKinds.remove(c2.PolluteKinds.size()-1);

    }
}


void Kernel::NextStep(){
    Time++;
    emit TimeChanged(Time);

    QRegExp RInput( "Input\\s*(\\d+),\\s*(\\d+),\\s*(\\d+)\\s*;\\s*$" );
    QRegExp RMerge("Merge\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+)\\s*;\\s*$");
    QRegExp RSplit("Split\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+)\\s*;\\s*$");
    QRegExp ROutput("Output\\s*(\\d+),\\s*(\\d+),\\s*(\\d+)\\s*;\\s*$");
    QRegExp RMove("Move\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+)\\s*;\\s*$");

    bool end = true;
    for(auto it : Orders)
    {
        if(it.indexOf(RInput) >= 0){
            if(RInput.cap(1).toInt() >= Time){
                Input(RInput.cap(1).toInt(),RInput.cap(2).toInt(),RInput.cap(3).toInt());
                end = false;
            }
            //qDebug()<<"Input";
        }
        else if(it.indexOf(ROutput) >= 0){
            if(ROutput.cap(1).toInt() >= Time){
                Output(ROutput.cap(1).toInt(),ROutput.cap(2).toInt(),ROutput.cap(3).toInt());
                end = false;
            }
            //qDebug()<<"Output";
        }
        else if(it.indexOf(RMove) >= 0){
            if(RMove.cap(1).toInt() >= Time){
                Move(RMove.cap(1).toInt(),RMove.cap(2).toInt(),RMove.cap(3).toInt(),RMove.cap(4).toInt(),RMove.cap(5).toInt());
                end = false;
            }
            //qDebug()<<"Move";
        }
        else if(it.indexOf(RMerge) >= 0){
            if(RMerge.cap(1).toInt() >= Time || RMerge.cap(1).toInt() + 1 == Time){
                Merge(RMerge.cap(1).toInt(),RMerge.cap(2).toInt(),RMerge.cap(3).toInt(),RMerge.cap(4).toInt(),RMerge.cap(5).toInt());
                end = false;
            }
            //qDebug()<<"Merge";
        }
        else if(it.indexOf(RSplit) >= 0){
            if(RSplit.cap(1).toInt() >= Time || RSplit.cap(1).toInt() + 1 == Time){
                Split(RSplit.cap(1).toInt(),RSplit.cap(2).toInt(),RSplit.cap(3).toInt(),RSplit.cap(4).toInt(),RSplit.cap(5).toInt(),RSplit.cap(6).toInt(),RSplit.cap(7).toInt());
                end = false;
            }
            //qDebug()<<"Split";
        }
    }
    if(end) emit(End());


}

void Kernel::Before(){


    QRegExp RInput( "Input\\s*(\\d+),\\s*(\\d+),\\s*(\\d+)\\s*;\\s*$" );
    QRegExp RMerge("Merge\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+)\\s*;\\s*$");
    QRegExp RSplit("Split\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+)\\s*;\\s*$");
    QRegExp ROutput("Output\\s*(\\d+),\\s*(\\d+),\\s*(\\d+)\\s*;\\s*$");
    QRegExp RMove("Move\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+),\\s*(\\d+)\\s*;\\s*$");

    bool begin = true;
    for(auto it : Orders)
    {
        if(it.indexOf(RInput) >= 0){
            if(RInput.cap(1).toInt() == Time){
                BInput(RInput.cap(1).toInt(),RInput.cap(2).toInt(),RInput.cap(3).toInt());
                begin = false;
            }
            //qDebug()<<"Input";
        }
        else if(it.indexOf(ROutput) >= 0){
            if(ROutput.cap(1).toInt() == Time){
                BOutput(ROutput.cap(1).toInt(),ROutput.cap(2).toInt(),ROutput.cap(3).toInt());
                begin = false;
            }
            //qDebug()<<"Output";
        }
        else if(it.indexOf(RMove) >= 0){
            if(RMove.cap(1).toInt() == Time){
                BMove(RMove.cap(1).toInt(),RMove.cap(2).toInt(),RMove.cap(3).toInt(),RMove.cap(4).toInt(),RMove.cap(5).toInt());
                begin = false;
            }
            //qDebug()<<"Move";
        }
        else if(it.indexOf(RMerge) >= 0){
            if(RMerge.cap(1).toInt() == Time || RMerge.cap(1).toInt() + 1 == Time){
                BMerge(RMerge.cap(1).toInt(),RMerge.cap(2).toInt(),RMerge.cap(3).toInt(),RMerge.cap(4).toInt(),RMerge.cap(5).toInt());
                begin = false;
            }
            //qDebug()<<"Merge";
        }
        else if(it.indexOf(RSplit) >= 0){
            if(RSplit.cap(1).toInt() == Time || RSplit.cap(1).toInt() + 1 == Time){
                BSplit(RSplit.cap(1).toInt(),RSplit.cap(2).toInt(),RSplit.cap(3).toInt(),RSplit.cap(4).toInt(),RSplit.cap(5).toInt(),RSplit.cap(6).toInt(),RSplit.cap(7).toInt());
                begin = false;
            }
            //qDebug()<<"Split";
        }
    }
    Time--;
    emit TimeChanged(Time);

}

void Kernel::Reset(){
    Time = -1;
    KindNumber = 0;
    for(int i = 1;i<=Width;i++)
        for(int j = 1;j <= Height;j++)
            CubeData[i][j].Initial();
}

void Cube::Initial(){
    Position Other = Position(0, 0);
    kind = -1;
    Expanding = false;
    PolluteKinds.clear();
    Washing = false;
    blocked = false;
}

bool Kernel::CheckConstraint(int x, int y){//check x,y is moveable
    for(int i = 1;i<=Width && i != x;i++){
        for(int j = 1;j <= Height && j != y;j++){
            Cube& c = CubeData[i][j];
            Cube& d = CubeData[x][y];
            if(((c.kind != -1 && d.kind != c.kind) || c.Washing == true)&& qAbs(x - i)<=1&& qAbs(y - j)<=1){
                emit(Error("Constraint"));
                return false;
            }
        }
    }
    return true;
}

bool Kernel::Moveable(int x, int y){//check x,y is moveable
    if(CubeData[x][y].blocked == true) return false;
    Cube & a = CubeData[x+1][y];
    Cube & s = CubeData[x-1][y];
    Cube & d = CubeData[x][y+1];
    Cube & f = CubeData[x][y-1];
    if((a.kind > -1 ) && x+1 <= Width){
        return false;
    }
    if((s.kind > -1 ) && x-1 >= 1){
        return false;
    }
    if((d.kind > -1 ) && y+1 <= Height){
        return false;
    }
    if((f.kind > -1 ) && y-1 >= 1){
        return false;
    }
    return true;
}

void Kernel::WashMove(int x, int y, int xx, int yy){
    Cube & c = CubeData[xx][yy];
    c.PolluteKinds.clear();
    c.Washing = true;
    CubeData[x][y].Washing = false;
    CubeData[x][y].PolluteKinds.clear();
    if(Sound)
        QSound::play(":/Sound/1.wav");
}




