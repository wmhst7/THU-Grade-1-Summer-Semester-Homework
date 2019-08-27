#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#define DISTANCE 0

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    for(int i = 1;i<=8;i++){
        for(int j = 1;j <= 8;j++){
            Data[i][j].black = true;
            this->update();
        }

        }
}

void Widget::paintEvent(QPaintEvent * event) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing);
    qreal len = 600 / 8;
    p.setPen(QPen(Qt::black,3));
    p.setBrush(QColor("gold"));
    p.drawRect(len-0.2*len,len-0.2*len,8.4*len,len*8.4);

    p.setPen(QPen(Qt::gray,1));
    for(int i=1;i <=8 + 1;i++)
        p.drawLine(len + DISTANCE ,i * len + DISTANCE,len * (8 + 1) + DISTANCE,i * len + DISTANCE);
    for(int i=1;i<=8 + 1;i++)
        p.drawLine(i * len+ DISTANCE, len+ DISTANCE, i * len+ DISTANCE, len * (8 + 1)+ DISTANCE);
    for(int i = 1;i<=8;i++){
        for(int j = 1;j <= 8;j++){
            Cube& c = Data[i][j];
            //先画正常drop
            if(c.black){
                p.setPen(QColor("black"));;
                p.setBrush(QColor("black"));

            }else{
                p.setPen(QColor("white"));;
                p.setBrush(QColor("white"));
            }
            p.drawEllipse(i*len+0.1*len, j*len+0.1*len, len*0.8, len*0.8);
        }
    }
}

void Widget::mousePressEvent(QMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        QPoint p = event->pos();
        qreal len = 600 / 8;
        int x = (p.x() - DISTANCE)/len;
        int y = (p.y() - DISTANCE)/len;
        if(x > 8 || x < 1 || y > 8 || y < 1) return;
        if(Data[x][y].black){
            for(int i=1;i<=8;i++){
                Data[x][i].black == true ? Data[x][i].black = false : Data[x][i].black = true;
            }
            for(int i=1;i<=8 ;i++){
                Data[i][y].black == true ? Data[i][y].black = false : Data[i][y].black = true;
            }
            Data[x][y].black == true ? Data[x][y].black = false : Data[x][y].black = true;


        }else if(!Data[x][y].black){
            QMessageBox::information(this, QString("Warning"), QString("Clicked White Cube!"));

        }
        this->update();
    }
}


