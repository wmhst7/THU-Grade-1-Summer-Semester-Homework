#include "dialog.h"
#include "ui_dialog.h"
#include <QHostAddress>
#include <QHostInfo>
#include <QRegExp>
#include <QDebug>
#include <QMessageBox>



Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_accepted()
{
    QRegExp rx("^((?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))*$");
    QString str = ui->lineEdit->text();
    qDebug()<<str;
    int pos = str.indexOf(rx);
    if(pos >= 0){
        ServerAddress = str;
    }else{
        QMessageBox::information(this, tr("Error!"), tr("IP Address Format Error!\nPlease Type in Again!"));
    }
}
