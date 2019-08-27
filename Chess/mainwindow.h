#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <QPoint>
#include <QDebug>

#include <QFileDialog>

#include <QTimer>

#include <QLabel>
#include <QProgressBar>
#include <QTextBrowser>

#include <QMouseEvent>
#include<QKeyEvent>
#include<QEvent>

#include <QRegExp>
#include <QSound>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void paintEvent(QPaintEvent*);
};

#endif // MAINWINDOW_H
