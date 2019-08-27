#include "newdialog.h"
#include "ui_newdialog.h"
#include <QFileDialog>
#include <QMessageBox>


NewDialog::NewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDialog)
{
    ui->setupUi(this);
    OutputPos = Position(8, 1);
}

NewDialog::~NewDialog()
{
    delete ui;
}

void NewDialog::on_spinBox_2_valueChanged(int arg1)
{
    height = arg1;
}

void NewDialog::on_spinBox_3_valueChanged(int arg1)
{
    OutputPos.X = arg1;
}

void NewDialog::on_spinBox_4_valueChanged(int arg1)
{
    OutputPos.Y = arg1;
}

void NewDialog::on_pushButton_2_clicked()
{
    Position pos(ui->spinBox_5->value(),ui->spinBox_6->value());
    InputPosVec << pos;
    ui->spinBox_5->setValue(1);
    ui->spinBox_6->setValue(1);
    ui->labelnum->setText("Number: " + QString::number(InputPosVec.size()));
}

void NewDialog::on_spinBox_valueChanged(int arg1)
{
    width = arg1;
}
