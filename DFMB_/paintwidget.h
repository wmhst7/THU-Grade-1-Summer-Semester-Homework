#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include "kernel.h"
#include <QMouseEvent>


class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent* );

signals:

public slots:
    void UpdateIOput();



public:
    Kernel * kernel = nullptr;
    QVector<Position> InputPositions_draw;
    Position OutputPosition_draw = Position(0,1);
    qreal len = 1;

    bool end = false;

};

#endif // PAINTWIDGET_H
