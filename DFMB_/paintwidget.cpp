#include "paintwidget.h"
#include <QPainter>
#include <QDebug>
#include <QtWidgets/qframe.h>
#include <QWidget>
#include <sys/time.h>
#define DISTANCE 20


PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{

}


void PaintWidget::paintEvent(QPaintEvent * event) {
    //Paint Rectangles
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing);
    p.setPen(QPen(Qt::black, 2));
    p.drawRect(0, 0, this->width(), this->height());
    p.setPen(QPen(Qt::gray,1));

    len = qMin((this->width() - 2 * DISTANCE)/(kernel->Width + 2), (this->height() - 2 * DISTANCE)/(kernel->Height + 2));
    for(int i=1;i <= kernel->Height + 1;i++)
        p.drawLine(len + DISTANCE ,i * len + DISTANCE,len * (kernel->Width + 1) + DISTANCE,i * len + DISTANCE);
    for(int i=1;i<=kernel->Width + 1;i++)
        p.drawLine(i * len+ DISTANCE, len+ DISTANCE, i * len+ DISTANCE, len * (kernel->Height + 1)+ DISTANCE);

    p.translate(DISTANCE, DISTANCE);
    p.setBrush(QColor("dodgerblue"));
    p.drawRect(len * OutputPosition_draw.X, len * OutputPosition_draw.Y, len, len);
    p.setBrush(QColor("salmon"));
    for(auto it : InputPositions_draw){
        p.drawRect(len * it.X, len * it.Y, len, len);
    }
    //Draw Blocked
     p.setBrush(QColor("darkgray"));
     for(int i = 1;i<=kernel->Width;i++){
        for(int j = 1;j <= kernel->Height;j++){
            Cube & c = kernel->CubeData[i][j];
            if(c.blocked == true){
                p.drawRect(len * i, len * j, len, len);
            }
        }
    }
    //Draw Washing io Rect
    if(kernel->Wash){
        p.setPen(QPen(QColor("gray"), 0.5));
        p.setBrush(QColor("salmon"));
        p.drawRect(len, 0, len, len);

        p.setBrush(QColor("dodgerblue"));
        p.drawRect(len * kernel->Width, len * kernel->Height + len, len, len);

        p.setPen(QPen(QColor("white"), 0.5));
        p.drawText(len+len*0.1, len/2, "WashIn");
        p.drawText(len * kernel->Width, len * kernel->Height + len*1.5, "WashOut");
    }
    //if it is over
    if(end){
        qDebug()<<"Number";
        QFont font;
        font.setPointSize(24);
        font.setLetterSpacing(QFont::AbsoluteSpacing, 20);
        p.setFont(font);
        p.setPen(QPen(Qt::gray,1));
         for(int i = 1;i<=kernel->Width;i++){
            for(int j = 1;j <= kernel->Height;j++){
                //直接在这paint
                Cube & c = kernel->CubeData[i][j];
                p.drawText(i*len+len/2, j*len+len/2, QString::number(c.PolluteKinds.size()));
            }
        }
         return;
    }

    //qDebug()<<"Paint w = "<<kernel->Width<<" h = "<<kernel->Height;
    for(int i = 1;i<=kernel->Width;i++){
        for(int j = 1;j <= kernel->Height;j++){
            //直接在这paint
            Cube & c = kernel->CubeData[i][j];

            //先画正常drop
            if(c.kind != -1 && c.Expanding == false && c.Washing == false){
                p.setPen(QColor(kernel->GetColorName(c.kind)));;
                p.setBrush(QColor(kernel->GetColorName(c.kind)));
                p.drawEllipse(i*len, j*len, len, len);
            }
            //再画Split/Merge
            else if(c.kind != -1 && c.Expanding == true && c.Washing == false){
                if(c.Other.X != 0 && c.Other.Y != 0){
                    p.setPen(QColor(kernel->GetColorName(c.kind)));
                    p.setBrush(QColor(kernel->GetColorName(c.kind)));
                    qreal x = qMin(i, c.Other.X);
                    qreal y = qMin(j, c.Other.Y);
                    qreal dx = len * ( qMax(i-c.Other.X, c.Other.X-i)+1 );
                    qreal dy = len * (qMax(j-c.Other.Y, c.Other.Y - j) + 1);
                    p.drawEllipse(x*len,y*len,dx,dy);
                    //qDebug()<<x<<" Ecllipse "<<y;
                }
            }
            //再画残液
            else if(c.kind == -1 && c.PolluteKinds.size()!=0 && c.Washing == false){
                p.setPen(QColor(kernel->GetColorName(c.PolluteKinds[c.PolluteKinds.size()-1])));
                p.setBrush(QColor(kernel->GetColorName(c.PolluteKinds[c.PolluteKinds.size()-1])));
                p.drawEllipse(i*len + len/4, j*len + len/4, len/2, len/2);
            }

            //Paint Washing Drop
            if(kernel->Wash && c.Washing == true){
                p.setPen(QColor("skyblue"));;
                p.setBrush(QColor("skyblue"));
                p.drawEllipse(i*len, j*len, len, len);
                p.setBrush(QColor("dodgerblue"));
                p.drawEllipse(i*len + len/4, j*len + len/4, len/2, len/2);
                //qDebug()<<"Washing Point:"<<i<<" "<<j;
            }
        }
    }
}

void PaintWidget::UpdateIOput(){
    if(kernel->OutputPosition.X == 1){
        OutputPosition_draw = Position(0, kernel->OutputPosition.Y);
    }else if(kernel->OutputPosition.X == kernel->Width){
        OutputPosition_draw = Position(kernel->Width + 1, kernel->OutputPosition.Y);
    }else if(kernel->OutputPosition.Y == 1){
        OutputPosition_draw = Position(kernel->OutputPosition.X, 0);
    }else if(kernel->OutputPosition.Y == kernel->Height){
        OutputPosition_draw = Position(kernel->OutputPosition.X, kernel->Height+1);
    }
    InputPositions_draw.clear();
    for(auto it : kernel->InputPositions){
        if(it.X == 1){
            InputPositions_draw << Position(0, it.Y);
        }else if(it.X == kernel->Width){
            InputPositions_draw << Position(kernel->Width + 1, it.Y);
        }else if(it.Y == 1){
            InputPositions_draw << Position(it.X, 0);
        }else if(it.Y == kernel->Height){
            InputPositions_draw << Position(it.X, kernel->Height+1);
        }
    }
}


void PaintWidget::mousePressEvent(QMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        QPoint p = event->pos();
        int x = (p.x() - DISTANCE)/len;
        int y = (p.y() - DISTANCE)/len;
        kernel->CubeData[x][y].blocked ? kernel->CubeData[x][y].blocked = false : kernel->CubeData[x][y].blocked = true;
        this->update();
    }
}

