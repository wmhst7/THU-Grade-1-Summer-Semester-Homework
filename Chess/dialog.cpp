#include "dialog.h"
#include "ui_dialog.h"
#include <QHostAddress>
#include <QHostInfo>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QHostInfo info=QHostInfo::fromName(QHostInfo::localHostName());
    QHostAddress address=info.addresses().first();
    QString str=address.toString();
    ui->lineEdit->setText(str);
}

Dialog::~Dialog()
{
    delete ui;
}
