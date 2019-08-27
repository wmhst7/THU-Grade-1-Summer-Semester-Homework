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


#include <QTimer>

#include <QLabel>
#include <QProgressBar>
#include <QTextBrowser>



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

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void ShowNext();

    void dis();

    QString GetItem(int);

private:
    Ui::MainWindow *ui;



public:
    int now = 0;//0-3
    QTimer * timer = nullptr;




};

#endif // MAINWINDOW_H
