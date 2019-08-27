#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    QObject::connect( timer,SIGNAL(timeout()), this, SLOT(ShowNext()) );
    timer->start(2000);

}

void MainWindow::ShowNext(){
    now++;
    if(now == 4) now = 0;
    this->dis();
    timer->start(2000);
}

QString MainWindow::GetItem(int i){
    if(i == 0){
        return "biscuit";
    }
    if(i == 1) return "steak";
    if(i == 2) return "noodle";
    if(i == 3) return "cola";
}

void MainWindow::dis(){
    QString item = GetItem(now);
    QString pic = ":/2/"+item+".jpg";
    QString sound = ":/2/"+item+".wav";
    QSound::play(sound);

    QImage *img=new QImage; //新建一个image对象

    img->load(pic); //将图像资源载入对象img，注意路径，可点进图片右键复制路径
    ui->label->setPixmap(QPixmap::fromImage(*img));
//    QPixmap * pixmap = new QPixmap;
//    pixmap->load(pic);
//    int w = ui->label->width();
//    int h = ui->label->height();
//    ui->label->setPixmap(pixmap->scaled(w,h,Qt::KeepAspectRatio));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    now = 0;
    timer->start(2000);
    dis();
}

void MainWindow::on_pushButton_2_clicked()
{
    now = 1;
    timer->start(2000);
    dis();
}

void MainWindow::on_pushButton_3_clicked()
{
    now = 2;
    timer->start(2000);
    dis();

}

void MainWindow::on_pushButton_4_clicked()
{
    now = 3;
    timer->start(2000);
    dis();

}
