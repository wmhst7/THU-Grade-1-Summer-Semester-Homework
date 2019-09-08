#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "dialog1.h"
#include <QPixmap>
#include <QPainter>
#include <QTime>
#define DISTANCE 85

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置背景图片
    setAutoFillBackground(true);
    QPalette palette;
    QPixmap *pixmap = new QPixmap(":/Res_/ChessPan.jpg");
    QPixmap pmap = pixmap->scaled(800,800,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    palette.setBrush(backgroundRole(),QBrush(pmap));
    this->setPalette(palette);
    kernel = new Kernel();

    //QTimer
    timer = new QTimer(this);
    connect(timer , SIGNAL(timeout()), this, SLOT(ObserveTime()));
    connect(kernel, SIGNAL(MoveSignal(QPoint, QPoint)), this, SLOT(SendMove(QPoint, QPoint)));


}

void MainWindow::Dis(){
    QMessageBox::information(this, "QTcpSocket","Sucess!");
    kernel->status = 4;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent* event){
     //qDebug()<<"Painting";
     QPainter p(this);
     p.setRenderHint(QPainter::Antialiasing, true);
     p.setRenderHint(QPainter::TextAntialiasing);
     p.setBrush(QColor("black"));
     p.drawRect(800, 0, 200, 800);
     p.setBrush(QColor("gray"));
     p.drawRect(0,0,1000,30);
     qreal len = 78.75;
     p.translate(85,85);
     //Draw Other things
     p.setBrush(Qt::NoBrush);
     p.setPen(QPen(QColor("deepskyblue"), 4));
     for(auto it : kernel->AblePoints){
         p.drawRect(it.x()*len-len, it.y()*len-len, len, len);
     }
     p.setPen(QPen(QColor("orangered"), 4));
     bool now;
     if(kernel->In(kernel->NowPoint.x(), kernel->NowPoint.y())){
        now = kernel->GetCube(kernel->NowPoint).black;
     }
     for(auto it : kernel->MovePoints){
         if(kernel->GetCube(it).black != now)
            p.drawRect(it.x()*len-len, it.y()*len-len, len, len);
     }
     p.setPen(QPen(QColor("gold"), 4));
     if(kernel->NowPoint.x() != 0)
        p.drawRect(kernel->NowPoint.x()*len-len, kernel->NowPoint.y()*len-len, len, len);

     //Draw Chess Piece
     QPixmap *pixmap;
     QPixmap *pmap ;
     for(int i = 1; i <= 8;i++){
         for(int j = 1;j <= 8;j++){
             Cube & c = kernel->Cubes[i][j];
             if(c.kind != null){
                 if(c.black){
                     switch (c.kind) {
                     case king:
                         pixmap = new QPixmap(":/Material/Normal/black_king.png");
                         break;
                     case queen:
                         pixmap = new QPixmap(":/Material/Normal/black_queen.png");
                         break;
                     case knight:
                         pixmap = new QPixmap(":/Material/Normal/black_knight.png");
                         break;
                     case bishop:
                         pixmap = new QPixmap(":/Material/Normal/black_bishop.png");
                         break;
                     case pawn:
                         pixmap = new QPixmap(":/Material/Normal/black_pawn.png");
                         break;
                     case rook:
                         pixmap = new QPixmap(":/Material/Normal/black_rook.png");
                         break;
                     }
                 }else{
                     switch (c.kind) {
                     case king:
                         pixmap = new QPixmap(":/Material/Normal/white_king.png");
                         break;
                     case queen:
                         pixmap = new QPixmap(":/Material/Normal/white_queen.png");
                         break;
                     case knight:
                         pixmap = new QPixmap(":/Material/Normal/white_knight.png");
                         break;
                     case bishop:
                         pixmap = new QPixmap(":/Material/Normal/white_bishop.png");
                         break;
                     case pawn:
                         pixmap = new QPixmap(":/Material/Normal/white_pawn.png");
                         break;
                     case rook:
                         pixmap = new QPixmap(":/Material/Normal/white_rook.png");
                         break;
                     }
                 }
                 p.drawPixmap(i*len-len*0.8, j*len-len*0.8, pixmap->scaled(len*0.6,len*0.6,Qt::KeepAspectRatio, Qt::SmoothTransformation));

             }
         }
     }
}


void MainWindow::on_actionServer_triggered()
{
    Dialog * dialog = new Dialog();
    if(dialog->exec() ==  QDialog::Accepted) {
        socket = new QTcpSocket();

        if(dialog->ServerAddress == ""){
            QMessageBox::information(this, "QTcpSocket","Falied!");

            return;
        }
        kernel->status = 0;
        socket->connectToHost(dialog->ServerAddress, 9898);
        connect(socket, SIGNAL(connected()), this, SLOT(Dis()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(ReadData()));
        //QMessageBox::information(this, "QTcpSocket","Success!");
    }
}

void MainWindow::ReadData(){
    QByteArray buffer = socket->readAll();
    qDebug()<<"Client Receive: "<<buffer;
    if(buffer == "L"){
        kernel->status = 2;
        Check();
        InitAll();
        this->update();

    }else if(buffer == "H"){
        ReceiveHe();
    }else if(buffer[0] == 'H'){
        if(buffer[1] == 'A'){
            ReplyHe(true);
        }else if(buffer[1] == 'R'){
            ReplyHe(false);
        }
    }else if(buffer[0] == 'P'){
        int x = buffer[1] - '0';
        int y = buffer[2] - '0';
        chess c;
        switch (buffer[3]) {
        case 'Q':
            c = queen;
            break;
        case 'R':
            c = rook;
            break;
        case 'K':
            c = knight;
            break;
        case 'B':
            c = bishop;
            break;
        }
        kernel->Cubes[x][y].kind = c;
        Check();
        this->update();

    }else if(buffer[0] == 'b' || buffer[0] == 'w'){
        ClearAll();
        QTextStream in(buffer);
        bool blacking = true;
        int pos; int num;
        QStringList ls;
        chess kind;
        while(!in.atEnd()){
            QString str = in.readLine();
            str.simplified();
            //qDebug()<<str;
            if(str.indexOf("black") > -1){
                blacking = true;
                kernel->black = false;
                kernel->status = 4;
                continue;
            }else if(str.indexOf("white") > -1){
                kernel->status = 3;
                blacking = false;
                kernel->black = true;
                continue;
            }
            ls = str.split(" ");
            kind = Getkind(ls[0]);
            for(int i = 2;i<ls.size();i++){
                if(ls[i] == "\n") break;
                int x = ls[i][0].toLatin1() - 'a' + 1;
                int y = 9 - ls[i][1].toLatin1() + '0';
                kernel->Cubes[x][y].kind = kind;
                kernel->Cubes[x][y].black = blacking;
            }
        }
        this->update();
    }else{
        int x = buffer[0] - '0';
        int y = buffer[1] - '0';
        int xx = buffer[2] - '0';
        int yy = buffer[3] - '0';
        kernel->NowPoint = QPoint(x, y);
        kernel->UpdateAblePoints();
        kernel->Move(QPoint(x,y), QPoint(xx,yy));
        kernel->NowPoint = QPoint(0,0);
        kernel->UpdateAblePoints();
        Check();
        this->update();
        kernel->status = 4;
        StartClock();
    }

}

void MainWindow::mousePressEvent(QMouseEvent* event){
    if(kernel->status == 3){
        kernel->black = true;
    }else if(kernel->status == 4){
        kernel->black = false;
    }
    if(kernel->status != 4) return;
    if(event->button() == Qt::LeftButton){
        QPoint p = event->pos();
        int x = (p.x() -85) / 78.75 +1;
        int y = (p.y() -85) / 78.75 +1;
        if(!kernel->In(x, y)) return;
        kernel->UpdateAblePoints();
        bool Swi = false;
        if(kernel->Move(kernel->NowPoint, QPoint(x, y))){
            Swi = true;
            emit(kernel->MoveSignal(kernel->NowPoint, QPoint(x, y)));
            kernel->NowPoint = QPoint(0,0);
        }else{
            kernel->NowPoint.rx() = x;
            kernel->NowPoint.ry() = y;
        }
        kernel->UpdateAblePoints();
        Check();
        if(Swi) kernel->status = 3;
        this->update();
    }
}


void MainWindow::Check(){

    //Check Winning
    if(kernel->status == 1){
        QMessageBox::information(this, tr("Game Over!"), "Black Win!");
        timer->stop();
        return;
    }else if(kernel->status == 2){
        QMessageBox::information(this, tr("Game Over!"), "White Win!");
        timer->stop();
        return;
    }
    bool b1 = false, b2 = false;
    for(int i=1;i<9;i++){
        for(int j=1;j<9;j++){
            if(kernel->Cubes[i][j].kind == king){
                if(kernel->Cubes[i][j].black){
                    b1 = true;
                }else{
                    b2 = true;
                }
            }
        }
    }
    if(!b1){//black lose
        kernel-> status = 2;
        QMessageBox::information(this, tr("Game Over!"), "White Win!");
        timer->stop();
        return;
    }
    if(!b2){
        kernel->status = 1;
        QMessageBox::information(this, tr("Game Over!"), "Black Win!");
        timer->stop();
        return;
    }
    //Check Pawn
    if(kernel->status != 4) return;
    for(int i = 1; i < 9; i++){
        if(kernel->Cubes[i][1].kind == pawn && kernel->Cubes[i][1].black == false) {
            Dialog1 * Dialog = new Dialog1();
            Dialog -> exec();
            kernel->ToKind = Dialog->ToKind;
            kernel->Cubes[i][1].kind = kernel->ToKind;
            QString str = "P"+QString::number(i)+"1";
            switch (kernel->ToKind) {
            case queen:
                str += "Q";
                break;
            case rook:
                str += "R";
                break;
            case knight:
                str += "K";
                break;
            case bishop:
                str += "B";
                break;
            }
            SendPawnTo(str);
        }
        if(kernel->Cubes[i][8].kind == pawn && kernel->Cubes[i][8].black == true) {
            Dialog1 * Dialog = new Dialog1();
            Dialog -> exec();
            kernel->ToKind = Dialog->ToKind;
            kernel->Cubes[i][8].kind = kernel->ToKind;
            QString str = "P"+QString::number(i)+"8";
            switch (kernel->ToKind) {
            case queen:
                str += "Q";
                break;
            case rook:
                str += "R";
                break;
            case knight:
                str += "K";
                break;
            case bishop:
                str += "B";
                break;
            }
            SendPawnTo(str);
        }
    }
}

void MainWindow::StartClock(){//收到对方信号开始本方回合
    Time = 30;
    timer->start(1000);
}

void MainWindow::InitClock(){
    Time = 30;
    timer->stop();
    ui->lcdNumber->display(Time);
}

void MainWindow::ObserveTime(){
    Time--;
    ui->lcdNumber->display(Time);
    if(Time == 0){
        QMessageBox::information(this, tr("Time Out!"),tr("You Lose!"));
        kernel->status = 1;
        timer->stop();
        SendLost();
    }
}

void MainWindow::on_Startbtn_clicked()
{
    SendLost();
    InitAll();
}



void MainWindow::InitAll(){
    for(int i = 1;i <9;i++){
        for(int j = 1;j<9;j++){
            kernel->Cubes[i][j].kind = null;
            kernel->Cubes[i][j].black = true;
            kernel->Cubes[i][j].moved = false;
        }

    }
    kernel->Initial();
}


void MainWindow::on_hejubtn_clicked()
{
    QByteArray buff;
    QTextStream out (&buff, QIODevice::ReadWrite);
    out << "H" << flush;
    int len = socket->write(buff);
    if(len == -1){
        QMessageBox::information(this, "QT网络通信", "向服务端发送数据失败！");
        return;
    }
    timer->stop();

}
void MainWindow::SendMove(QPoint p, QPoint pp){
    QByteArray buff;
    QTextStream out (&buff, QIODevice::ReadWrite);
    out << p.x()<<p.y()<<pp.x()<<pp.y() << flush;
    int len = socket->write(buff);
    if(len == -1){
        QMessageBox::information(this, "QT网络通信", "向服务端发送数据失败！");
        return;
    }

    InitClock();

}

void MainWindow::SendLost(){
    QByteArray buff;
    QTextStream out (&buff, QIODevice::ReadWrite);
    out <<"L"<< flush;
    int len = socket->write(buff);
    if(len == -1){
        QMessageBox::information(this, "QT网络通信", "向服务端发送数据失败！");
        return;
    }
    kernel->status = 1;
    Check();
    InitAll();
}

void MainWindow::SendPawnTo(QString str){

    QByteArray buff;
    QTextStream out (&buff, QIODevice::ReadWrite);
    out <<str<< flush;
    int len = socket->write(buff);
    if(len == -1){
        QMessageBox::information(this, "QT网络通信", "向服务端发送数据失败！");
        return;
    }

}

void MainWindow::ReplyHe(bool b){
    if(b){
        kernel->status = 0;
        QMessageBox::information(this, tr("Game Over!"),tr("The game ends in a draw!"));
        timer->stop();

    }else{
        QMessageBox::information(this, tr("Information"),tr("The other side has refused your drawn-game request!"));
        timer->start();
    }
}


void MainWindow::ReceiveHe(){
    QByteArray buff;
    QTextStream out (&buff, QIODevice::ReadWrite);
    DialogHe * d = new DialogHe();
    d->exec();
    if(d->Accept){
        out <<"HA"<< flush;
        kernel->status = 0;
        QMessageBox::information(this, tr("Game Over!"),tr("The game ends in a draw!"));
    }else{
        out <<"HR"<< flush;
        QMessageBox::information(this, tr("Information"),tr("You have refused the other side's drawn-game request!"));
    }
    int len = socket->write(buff);
    if(len == -1){
        QMessageBox::information(this, "QT网络通信", "向服务端发送数据失败！");
        return;
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_pushButton_clicked()
{

    QString path =	QFileDialog::getSaveFileName ( this,    tr("Save File"), "." );

    QFile file(path);
    if (!file.open(QFile::WriteOnly | QIODevice::Truncate)){
        QMessageBox::information(this, QString("Warning"), QString("Cannot write"));
        return;
    }
    QTextStream out(&file);
    QVector<QPoint> wking;
    QVector<QPoint> wqueen;
    QVector<QPoint> wrook;
    QVector<QPoint> wknight;
    QVector<QPoint> wbishop;
    QVector<QPoint> wpawn;
    QVector<QPoint> bking;
    QVector<QPoint> bqueen;
    QVector<QPoint> brook;
    QVector<QPoint> bknight;
    QVector<QPoint> bbishop;
    QVector<QPoint> bpawn;
    for(int i=1;i<9;i++){
        for(int j=1;j<9;j++){
            Cube& c = kernel->Cubes[i][j];
            if(c.black){
                switch (c.kind) {
                case king:
                    bking<<QPoint(i,j);
                    break;
                case queen:
                    bqueen<<QPoint(i,j);
                    break;
                case rook:
                    brook<<QPoint(i,j);
                    break;
                case bishop:
                    bbishop<<QPoint(i,j);
                    break;
                case knight:
                    bknight<<QPoint(i,j);
                    break;
                case pawn:
                    bpawn<<QPoint(i,j);
                    break;
                }
            }else{
                switch (c.kind) {
                case king:
                    wking<<QPoint(i,j);
                    break;
                case queen:
                    wqueen<<QPoint(i,j);
                    break;
                case rook:
                    wrook<<QPoint(i,j);
                    break;
                case bishop:
                    wbishop<<QPoint(i,j);
                    break;
                case knight:
                    wknight<<QPoint(i,j);
                    break;
                case pawn:
                    wpawn<<QPoint(i,j);
                    break;
                }
            }
        }
    }
    QString t;
    if(kernel->status == 3)
    {
        out<<"black\n";
        if(bking.size()>0){
            t = "king ";
            t += QString::number(bking.size());
            t +=" ";
            for(auto p : bking){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }
        if(bqueen.size()>0){
            t = "queen ";
            t += QString::number(bqueen.size());
            t +=" ";
            for(auto p : bqueen){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(brook.size()>0){
            t = "rook ";
            t += QString::number(brook.size());
            t +=" ";
            for(auto p : brook){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(bbishop.size()>0){
            t = "bishop ";
            t += QString::number(bbishop.size());
            t +=" ";
            for(auto p : bbishop){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(bknight.size()>0){
            t = "knight ";
            t += QString::number(bknight.size());
            t +=" ";
            for(auto p : bknight){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(bpawn.size()>0){
            t = "pawn ";
            t += QString::number(bpawn.size());
            t +=" ";
            for(auto p : bpawn){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        out<<"white\n";
        if(wking.size()>0){
            t = "king ";
            t += QString::number(wking.size());
            t +=" ";
            for(auto p : wking){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }
        if(wqueen.size()>0){
            t = "queen ";
            t += QString::number(wqueen.size());
            t +=" ";
            for(auto p : wqueen){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(wrook.size()>0){
            t = "rook ";
            t += QString::number(wrook.size());
            t +=" ";
            for(auto p : wrook){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(wbishop.size()>0){
            t = "bishop ";
            t += QString::number(wbishop.size());
            t +=" ";
            for(auto p : wbishop){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(wknight.size()>0){
            t = "knight ";
            t += QString::number(wknight.size());
            t +=" ";
            for(auto p : wknight){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(wpawn.size()>0){
            t = "pawn ";
            t += QString::number(wpawn.size());
            t +=" ";
            for(auto p : wpawn){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }
    }
    else if(kernel->status == 4)
    {
        out<<"white\n";
        if(wking.size()>0){
            t = "king ";
            t += QString::number(wking.size());
            t +=" ";
            for(auto p : wking){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }
        if(wqueen.size()>0){
            t = "queen ";
            t += QString::number(wqueen.size());
            t +=" ";
            for(auto p : wqueen){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(wrook.size()>0){
            t = "rook ";
            t += QString::number(wrook.size());
            t +=" ";
            for(auto p : wrook){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(wbishop.size()>0){
            t = "bishop ";
            t += QString::number(wbishop.size());
            t +=" ";
            for(auto p : wbishop){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(wknight.size()>0){
            t = "knight ";
            t += QString::number(wknight.size());
            t +=" ";
            for(auto p : wknight){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(wpawn.size()>0){
            t = "pawn ";
            t += QString::number(wpawn.size());
            t +=" ";
            for(auto p : wpawn){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }
        out<<"black\n";
        if(bking.size()>0){
            t = "king ";
            t += QString::number(bking.size());
            t +=" ";
            for(auto p : bking){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }
        if(bqueen.size()>0){
            t = "queen ";
            t += QString::number(bqueen.size());
            t +=" ";
            for(auto p : bqueen){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(brook.size()>0){
            t = "rook ";
            t += QString::number(brook.size());
            t +=" ";
            for(auto p : brook){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(bbishop.size()>0){
            t = "bishop ";
            t += QString::number(bbishop.size());
            t +=" ";
            for(auto p : bbishop){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(bknight.size()>0){
            t = "knight ";
            t += QString::number(bknight.size());
            t +=" ";
            for(auto p : bknight){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }

        if(bpawn.size()>0){
            t = "pawn ";
            t += QString::number(bpawn.size());
            t +=" ";
            for(auto p : bpawn){
                char x = static_cast<char>('a' - 1 + p.x());
                char y = static_cast<char>('0' + 9 - p.y());
                t += x; t += y; t += " ";
            }
            t += "\n";
            //qDebug()<<t;
            out<<t;
        }
    }
    out.flush();
    file.close();
}


void MainWindow::on_canjubtn_clicked()
{
    LoadChess();
    //Send ChessData
    QByteArray buff;
    QTextStream out (&buff, QIODevice::ReadWrite);

    QFile file(LoadPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(this, QString("Warning"), QString("Cannot Read"),QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    QTextStream in(&file);
    QString txt = in.readAll();
    out<<txt<<flush;
    int len = socket->write(buff);
    if(len == -1){
        QMessageBox::information(this, "QT网络通信", "向服务端发送数据失败！");
        return;
    }
}

void MainWindow::LoadChess(){
    ClearAll();
    LoadPath = QFileDialog::getOpenFileName(this, tr("Open Chess Data"));
    QFile file(LoadPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(this, QString("Warning"), QString("Cannot Read"),QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    QTextStream in(&file);
    QString line;
    bool blacking = true;
    int pos; int num;
    QStringList ls;
    chess kind;

    while(!file.atEnd()){
        QByteArray line = file.readLine();
        QString str(line);
        str.simplified();
        //qDebug()<<str;
        if(str.indexOf("black") > -1){
            blacking = true;
            kernel->status = 4;
            kernel->black = false;
            continue;
        }else if(str.indexOf("white") > -1){
            kernel->status = 3;
            blacking = false;
            kernel->black = true;
            continue;
        }
        ls = str.split(" ");
        kind = Getkind(ls[0]);
        for(int i = 2;i<ls.size();i++){
            if(ls[i] == "\n") break;
            //qDebug()<<ls[i];
            int x = ls[i][0].toLatin1() - 'a' + 1;
            int y = 9 - ls[i][1].toLatin1() + '0';
            kernel->Cubes[x][y].kind = kind;
            kernel->Cubes[x][y].black = blacking;
        }
    }
    this->update();
}

chess MainWindow::Getkind(QString str){
    if(str == "king") return king;
    if(str == "queen") return queen;
    if(str == "rook") return rook;
    if(str == "knight") return knight;
    if(str == "bishop") return bishop;
    if(str == "pawn") return pawn;
    return null;
}

void MainWindow::ClearAll(){
    kernel->NowPoint = QPoint(0, 0);
    kernel->AblePoints.clear();
    kernel->MovePoints.clear();
    for(int i = 1;i <9;i++){
        for(int j = 1;j<9;j++){
            kernel->Cubes[i][j].kind = null;
            kernel->Cubes[i][j].black = true;
            kernel->Cubes[i][j].moved = false;
        }
    }
}



