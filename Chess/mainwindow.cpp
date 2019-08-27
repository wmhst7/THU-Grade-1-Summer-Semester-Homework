#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPainter>
#define DISTANCE 85

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置背景图片
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap pixmap(":/Res_/ChessPan.jpg");
    QPixmap pmap = pixmap.scaled(800,800,Qt::KeepAspectRatio);
    palette.setBrush(backgroundRole(),QBrush(pmap));
    this->setPalette(palette);




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent* event){
     qDebug()<<"Painting";
     QPainter p(this);
     p.setBrush(QColor("yellow"));
     p.drawRect(85,85,10,10);
     p.drawRect(715,85,10,10);
     p.drawRect(715,715,10,10);
     p.drawRect(85,715,10,10);

}











