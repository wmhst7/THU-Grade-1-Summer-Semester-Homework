#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "kernel.h"
#include "newdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool Found =false;

    void BFS(QVector<Position>&,Position,Position );


private slots:
    void on_actionOpen_2_triggered();

    void on_actionNext_2_triggered();

    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_actionNext_triggered();

    void on_actionBefore_triggered();

    void on_actionReset_triggered();

    void on_actionAudio_toggled(bool arg1);

    void PrintError(QString);

    void End();

    void Wash();

    void on_actionGo_toggled(bool arg1);

    void on_pushButton_clicked(bool checked);



private:
    Ui::MainWindow *ui;
    NewDialog * newdialog = nullptr;
    Kernel * kernel = nullptr;
    QTimer * timer = nullptr;
};

#endif // MAINWINDOW_H
