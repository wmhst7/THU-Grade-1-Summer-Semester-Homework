#include "dialog1.h"
#include "ui_dialog1.h"

Dialog1::Dialog1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog1)
{
    ui->setupUi(this);
}

Dialog1::~Dialog1()
{
    delete ui;
}

void Dialog1::on_QBtn_clicked()
{
    ToKind = queen;
}

void Dialog1::on_RBtn_clicked()
{
    ToKind = rook;
}

void Dialog1::on_KBtn_clicked()
{
    ToKind = knight;
}

void Dialog1::on_BBtn_clicked()
{
    ToKind = bishop;
}
