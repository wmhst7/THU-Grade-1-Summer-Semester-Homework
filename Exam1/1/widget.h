#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QObject>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QRegExp>
#include <QMouseEvent>
#include <QTimer>
#include <QPoint>

namespace Ui {
class Widget;
}

class Cube{
public:
    bool black = true;

};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;

public:
    Cube Data[10][10];

    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent* );



};

#endif // WIDGET_H
