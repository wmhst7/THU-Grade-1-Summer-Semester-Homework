#ifndef KERNEL_H
#define KERNEL_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QRegExp>
#include <QSound>
#include <QTimer>
#include <QPoint>


struct Position{
public:
    int X;
    int Y;

    Position(int x, int y);
    Position(){X = 0; Y = 0;}
    ~Position(){}

    bool operator==(Position& p){
        if(X == p.X && Y == p.Y) return true;
        else return false;
    }
};

class Cube{
public:
    Position Other = Position(0, 0);
    int kind = -1;//now drop kind
    bool Expanding = false;//now ecllipse
    bool blocked = false;
    bool Washing = false;
    QVector<int> PolluteKinds;//Polluted Drop Kinds

public:
    Cube() {}
    ~Cube() {}
    void Initial();

};

class Kernel : public QObject
{
    Q_OBJECT
public:
    explicit Kernel(QObject *parent = nullptr);

    bool onBorder(Position&);

    void UpdateMix();

    bool Moveable(int , int);

signals:
    void TimeChanged(int);

    void Error(QString);

    void End();

public slots:
    void NextStep();

    void Before();

    void Move(int ,int, int, int, int);

    void Split(int ,int, int, int, int, int, int);

    void Merge(int , int, int, int, int);

    void Input(int , int, int);

    void Output(int ,int ,int);

    void BMove(int ,int, int, int, int);

    void BSplit(int ,int, int, int, int, int, int);

    void BMerge(int , int, int, int, int);

    void BInput(int , int, int);

    void BOutput(int ,int ,int);

    void Reset();

    bool CheckConstraint(int, int);

    void WashMove(int, int, int, int);

public:
    Cube CubeData[14][14];
    int Width = 8;
    int Height = 8;
    QVector<Position> InputPositions;
    Position OutputPosition;
    QStringList Orders;
    int Time = -1;
    int Interval = 700; //间隔时间
    int KindNumber = 0;
    bool Sound = true;
    bool Wash = false;
    QVector<QString> ColorName;
    QString GetColorName(int);
};

#endif // KERNEL_H
