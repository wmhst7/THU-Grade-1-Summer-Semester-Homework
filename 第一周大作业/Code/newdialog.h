#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include <QVector>
#include <QString>
#include "kernel.h"

namespace Ui {
class NewDialog;
}

class NewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDialog(QWidget *parent = nullptr);
    ~NewDialog();

private slots:

    void on_spinBox_2_valueChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);

    void on_spinBox_4_valueChanged(int arg1);

    void on_pushButton_2_clicked();

    //void on_pushButton_clicked();

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::NewDialog *ui;

public:
    QVector<Position> InputPosVec;
    Position OutputPos;
    int width, height;
    //QString OrderName;

};

#endif // NEWDIALOG_H
