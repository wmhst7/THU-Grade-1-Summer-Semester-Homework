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
    void NewConnection();
    void ServerReadData();
    void StartClock();
    void Check();
    void ObserveTime();
    void on_Startbtn_clicked();
    void on_canjubtn_clicked();
    void on_hejubtn_clicked();
    void SendMove(QPoint , QPoint);
    void SendLost();
    void InitClock();
    void SendPawnTo(QString);
    void ReceiveHe();
    void InitAll();
    void ReplyHe(bool);
    void LoadChess();

    chess Getkind(QString);

    void ClearAll();

    void on_actionClose_triggered();

    void on_pushButton_clicked();

    void on_actionCloseServer_triggered();

private:
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent* );

    Kernel * kernel;
    QTcpServer * server;
    QTcpSocket * socket;
    QTimer * timer = nullptr;
    int Time = 30;
    QString LoadPath;



};

#endif // MAINWINDOW_H
