#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTimer>
#include <QFileDialog>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QRegExp>
#include <QTcpServer>
#include <QTcpSocket>
#include "dialoghe.h"
#include "kernel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionServer_triggered();

    void ReadData();

    void StartClock();

    void InitClock();

    void Check();

    void ObserveTime();

    void on_Startbtn_clicked();

    void on_canjubtn_clicked();

    void on_hejubtn_clicked();

    void SendMove(QPoint , QPoint);

    void SendLost();

    void SendPawnTo(QString);

    void ReceiveHe();

    void ReplyHe(bool);

    void InitAll();

    void on_actionExit_triggered();

    void on_pushButton_clicked();

    void LoadChess();

    chess Getkind(QString);

    void ClearAll();

private:
    Ui::MainWindow *ui;

    void paintEvent(QPaintEvent*);

    void mousePressEvent(QMouseEvent* );

    Kernel * kernel;

    QTcpSocket * socket;

    QTimer * timer = nullptr;

    int Time = 30;

    QString LoadPath;


};

#endif // MAINWINDOW_H
