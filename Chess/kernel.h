#ifndef KERNEL_H
#define KERNEL_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QPoint>
#include <QDebug>

enum chess{
    null, king, queen, rook, knight, bishop, pawn
};

class Cube{
public:
    chess kind = null;

    bool black = true;

    bool moved = false;

    Cube();

    Cube & operator=(const Cube& c);

    void Init();



};

class Kernel : public QObject
{
    Q_OBJECT
public:
    explicit Kernel(QObject *parent = nullptr);

    inline bool In(int x, int y){
        if(x > 0 && x < 9 && y > 0 && y < 9) return true;
        else return false;
    }

signals:

    void Win(bool);

    void MoveSignal(QPoint , QPoint);

public slots:
    void UpdateAblePoints();

    bool Move(QPoint, QPoint);



public:
    Cube Cubes[10][10];

    bool black = false;

    QVector<QPoint> AblePoints;

    QVector<QPoint> MovePoints;

    QPoint NowPoint = QPoint(0, 0);

    chess ToKind = queen;

    int status = 4;//0 Draw 1 black win 2 white win 3 black/server turn 4 white/client turn

    Cube& GetCube(QPoint);
    void Initial();





};

#endif // KERNEL_H




