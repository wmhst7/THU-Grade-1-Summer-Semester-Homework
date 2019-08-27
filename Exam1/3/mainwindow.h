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
#include <QTextStream>
#include <QFileDialog>

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
    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;

public:
    QString txt;
    QString path;
};

#endif // MAINWINDOW_H
