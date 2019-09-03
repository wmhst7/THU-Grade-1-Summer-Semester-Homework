#ifndef DIALOG1_H
#define DIALOG1_H

#include <QDialog>
#include "kernel.h"

namespace Ui {
class Dialog1;
}

class Dialog1 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog1(QWidget *parent = nullptr);
    ~Dialog1();

    chess ToKind = queen;


private slots:
    void on_QBtn_clicked();

    void on_RBtn_clicked();

    void on_KBtn_clicked();

    void on_BBtn_clicked();

private:
    Ui::Dialog1 *ui;
};

#endif // DIALOG1_H
