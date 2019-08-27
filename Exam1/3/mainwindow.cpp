#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionOpen_triggered()
{
   path = QFileDialog::getOpenFileName(this, tr("Open File"), ".");
   QFile file(path);

   if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
       //QMessageBox::information(this, QString("Warning"), QString("Cannot Read from: %1").arg(OrderName),QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       return;
   }
   QTextStream in(&file);
   txt = in.readAll();
   ui->textEdit->setText(txt);
}

void MainWindow::on_actionNew_triggered()
{
    path = "";
    txt.clear();
    ui->textEdit->setText("");
}

void MainWindow::on_actionSave_triggered()
{
    txt = ui->textEdit->toPlainText();
    if(path == ""){
        path =	QFileDialog::getSaveFileName ( this,    tr("Save File"), "." );
    }
    QFile file(path);
    if (!file.open(QFile::WriteOnly | QIODevice::Truncate)){
        QMessageBox::information(this, QString("Warning"), QString("Cannot write"));
        return;
    }
    QTextStream out(&file);
    out<<txt;
    out.flush();
    file.close();
    }








