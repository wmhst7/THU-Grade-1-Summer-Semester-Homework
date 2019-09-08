#include "dialoghe.h"
#include "ui_dialoghe.h"

DialogHe::DialogHe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogHe)
{
    ui->setupUi(this);
}

DialogHe::~DialogHe()
{
    delete ui;
}

void DialogHe::on_pushButton_2_clicked()
{
    Accept = true;
    this->close();
}

void DialogHe::on_pushButton_clicked()
{
    Accept = false;
    this->close();
}
