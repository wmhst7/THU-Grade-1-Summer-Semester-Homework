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
