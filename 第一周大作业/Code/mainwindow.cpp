#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileDevice>
#include <QIODevice>
#include <QDebug>
#include <QObject>
#include <QTime>
#include <QQueue>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    kernel = new Kernel(this);
    ui->widget->kernel = this->kernel;
    QObject::connect(kernel, SIGNAL(TimeChanged(int)), ui->lcdNumber, SLOT(display(int)));
    QObject::connect(kernel, SIGNAL(Error(QString)), this, SLOT(PrintError(QString)));
    timer = new QTimer(this);
    QObject::connect( timer,SIGNAL(timeout()), this, SLOT(on_actionNext_triggered()) );
    QObject::connect(kernel, SIGNAL(End()), this, SLOT(End()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete kernel;
}

void MainWindow::on_actionOpen_2_triggered() //Open
{
    newdialog = new NewDialog(this);
    if( newdialog->exec() ==  QDialog::Accepted) {
        //Refresh Data...
        kernel->Width = newdialog->width;
        kernel->Height = newdialog->height;
        kernel->InputPositions.clear();
        for(auto it : newdialog->InputPosVec)
            kernel->InputPositions << it;
        kernel->OutputPosition = newdialog->OutputPos;
         if(kernel->Width < 4 && kernel -> Height < 4)
             QMessageBox::information(this, QString("Warning"), QString("Width <= 3 && Height <= 3 !"));
         if(!kernel->onBorder(kernel->OutputPosition)){
             QMessageBox::information(this, QString("Warning"), QString("Output端口不在边界上！"));
         }
         for(auto it : kernel->InputPositions){
             if(!kernel->onBorder(it))
                 QMessageBox::information(this, QString("Warning"), QString("Input端口不在边界上！"));
         }
         //QObject::connect(newdialog,SIGNAL(accepted()), ui->widget, SLOT(UpdateIOput()));
    }
    ui->widget->UpdateIOput();
    ui->widget->update();
}

void MainWindow::on_actionNext_2_triggered()
{

}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionOpen_triggered()
{

    QString OrderName = QFileDialog::getOpenFileName(nullptr, tr("Open Order File"));
    if(OrderName.length() == 0)
        QMessageBox::information(nullptr, tr("Order File"), tr("You didn't select any files."));
     QFile file(OrderName);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
         QMessageBox::information(this, QString("Warning"), QString("Cannot Read from: %1").arg(OrderName),QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
         return;
     }
     QTextStream in(&file);
     QString line;
     kernel->Orders.clear();
     while(!file.atEnd()){
         QByteArray line = file.readLine();
         QString str(line);
         //qDebug() << str;
         kernel->Orders << str.trimmed();
     }
     file.close();
     QFile f(OrderName);
     f.open(QIODevice::ReadOnly | QIODevice::Text);
     ui->textBrowser->setText(f.readAll());
     f.close();
     kernel->UpdateMix();
}

void MainWindow::on_actionNext_triggered()
{
    kernel->NextStep();
    ui->widget->update();
    if(kernel->Wash)
        Wash();
}

void MainWindow::on_actionBefore_triggered()
{
    kernel->Before();
    ui->lcdNumber->display(kernel->Time);
    ui->widget->update();
}

void MainWindow::on_actionReset_triggered()
{
    kernel->Reset();
    ui->lcdNumber->display(kernel->Time);
    ui->widget->end = false;
    ui->widget->update();
}

void MainWindow::on_actionAudio_toggled(bool arg1)
{
    arg1 ==  false ? kernel->Sound = true : kernel->Sound = false;
}

void MainWindow::PrintError(QString error){
    if(error == "Input"){
        QMessageBox::information(this,tr("Warning"), QString("Input Position Error!"));

    }else if(error == "Output"){
        QMessageBox::information(this,tr("Warning"), QString("Output Position Error!"));

    }else if(error == "Constraint"){
        QMessageBox::information(this,tr("Warning"), QString("Constraint Error!"));
    }
}

void MainWindow::on_actionGo_toggled(bool a)
{
    if(a == true)
        timer->start(kernel->Interval);
    else
        timer -> stop();
}

void MainWindow::End(){
    timer->stop();
    ui->widget->end = true;
}

void MainWindow::on_pushButton_clicked(bool checked)
{
    if(checked){
        ui->actionBefore->setEnabled(false);
        ui->actionBefore_2->setEnabled(false);
        ui->actionGo->setEnabled(false);
        ui->widget->UpdateIOput();
        kernel->Wash = true;
        ui->widget->update();
    }else{
        ui->actionBefore->setEnabled(true);
        ui->actionBefore_2->setEnabled(true);
        ui->actionGo->setEnabled(true);
        kernel->Wash = false;
        ui->widget->update();
    }
}

void MainWindow::BFS(QVector<Position>& Path, Position now, Position end) {
    if(now == end) return;
    QQueue<Position> Q;
    Position Vn;

    bool visited[kernel->Width + 1][kernel->Height + 1];
    for(int i = 0;i <= kernel->Width;i++){
        for(int j = 0; j <= kernel->Height;j++){
            visited[i][j] = false;
        }
    }
    Position pre[kernel->Width+1][kernel->Height+1];
    for(int i = 0;i <= kernel->Width;i++){
        for(int j = 0; j <= kernel->Height;j++){
            pre[i][j] = Position(0,0);
        }
    }

    Q.enqueue(now);
    visited[now.X][now.Y] = true;
    while(!Q.isEmpty()){
        Vn = Q.head();
        Q.dequeue();
        //qDebug()<<"BFS: "<<Vn.X<<" "<<Vn.Y<<" "<<end.X<<" "<<end.Y;
        if(kernel->Moveable(Vn.X+1, Vn.Y) && Vn.X+1 <= kernel->Width){
            if(Position(Vn.X+1, Vn.Y) == end){
                pre[Vn.X+1][Vn.Y] = Vn;
                Found = true;
                break;
            }
            if(visited[Vn.X+1][Vn.Y] == false){
                pre[Vn.X+1][Vn.Y] = Vn;
                Q.enqueue(Position(Vn.X+1, Vn.Y));
                visited[Vn.X+1][Vn.Y] = true;
            }
        }

        if(kernel->Moveable(Vn.X-1, Vn.Y) && Vn.X-1 >= 1){
            if(Position(Vn.X-1, Vn.Y) == end){
                pre[Vn.X-1][Vn.Y] = Vn;
                Found = true;
                break;
            }
            if(visited[Vn.X-1][Vn.Y] == false){
                pre[Vn.X-1][Vn.Y] = Vn;
                Q.enqueue(Position(Vn.X-1, Vn.Y));
                visited[Vn.X-1][Vn.Y] = true;
            }
        }

        if(kernel->Moveable(Vn.X, Vn.Y+1) && Vn.Y+1 <= kernel->Height){
            if(Position(Vn.X, Vn.Y+1) == end){
                pre[Vn.X][Vn.Y+1] = Vn;
                Found = true;
                break;
            }
            if(visited[Vn.X][Vn.Y+1] == false){
                pre[Vn.X][Vn.Y+1] = Vn;
                Q.enqueue(Position(Vn.X, Vn.Y+1));
                visited[Vn.X][Vn.Y+1] = true;
            }
        }

        if(kernel->Moveable(Vn.X, Vn.Y-1) && Vn.Y-1 >= 1){
            if(Position(Vn.X, Vn.Y-1) == end){
                pre[Vn.X][Vn.Y-1] = Vn;
                Found = true;
                break;
            }
            if(visited[Vn.X][Vn.Y-1] == false){
                pre[Vn.X][Vn.Y-1] = Vn;
                Q.enqueue(Position(Vn.X, Vn.Y-1));
                visited[Vn.X][Vn.Y-1] = true;
            }
        }
    }
    if(Found){
        Position p = end;
        QVector<Position> Tem;
        Tem<<p;
        while(pre[p.X][p.Y].X != 0){
            p = pre[p.X][p.Y];
            Tem<<p;
        }
        for(int i = Tem.size()-1;i>=0;i--){
            Path<<Tem[i];
            //qDebug()<<"Path += "<<Tem[i].X<<" "<<Tem[i].Y;
        }
    }
    return;
}

void MainWindow::Wash(){
    if(kernel->Moveable(1,1) == false)
        return;
    //BFS to get a path(Wash every cube as much as possible)
    QVector<Position> Path;
    QVector<Position> PollutedCube;
    PollutedCube<<Position(1,1);
    for(int i = 1;i<=kernel->Width;i++){
        for(int j = 1;j<=kernel->Height;j++){
            if(kernel->CubeData[i][j].PolluteKinds.size() > 0 && kernel->CubeData[i][j].kind == -1){
                PollutedCube<<Position(i, j);
            }
        }
    }
    PollutedCube << Position(kernel->Width, kernel->Height);
    Found = false;
    Position Pos = Position(1, 1);
    if(PollutedCube.size() == 2) return;
    if(PollutedCube.size() > 1){
        for(int i = 0;i < PollutedCube.size() -1 ;i ++){
            if(Found){
                Pos = PollutedCube[i];
                Found = false;
                qDebug()<<"finding: "<<Pos.X<<" "<<Pos.Y<<" "<<PollutedCube[i+1].X<<" "<<PollutedCube[i+1].Y;
                BFS(Path, Pos, PollutedCube[i+1]);
                if(Found)
                    qDebug()<<"Found: "<<Pos.X<<" "<<Pos.Y<<" "<<PollutedCube[i+1].X<<" "<<PollutedCube[i+1].Y;
            }
            else{
                Found = false;
                qDebug()<<"finding: "<<Pos.X<<" "<<Pos.Y<<" "<<PollutedCube[i+1].X<<" "<<PollutedCube[i+1].Y;
                BFS(Path, Pos, PollutedCube[i+1]);
                if(Found)
                    qDebug()<<"Found: "<<Pos.X<<" "<<Pos.Y<<" "<<PollutedCube[i+1].X<<" "<<PollutedCube[i+1].Y;

            }
        }
    }
    //Apply this path to the real CubeData and Print it
    QTime time;
    for(int i = 0; i < Path.size()-1; i++){
        kernel->WashMove(Path[i].X, Path[i].Y, Path[i+1].X, Path[i+1].Y);
        ui->widget->update();
        time.start();
        ui->actionNext->setEnabled(false);
        while(time.elapsed() < 150)
            QCoreApplication::processEvents();
    }
    kernel->CubeData[kernel->Width][kernel->Height].Washing = false;
    ui->actionNext->setEnabled(true);
    ui->widget->update();
}




