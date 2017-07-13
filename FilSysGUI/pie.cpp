#include "pie.h"
#include <QPainter>


pie::pie(QWidget *parent) : QWidget(parent)
{

}

void pie::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QRectF size= QRectF(30,30,this->width()-40,this->height()-40);
    QRectF size2= QRectF(0,0,20,20);
    QRectF size3= QRectF(0,30,20,20);
    painter.setBrush(Qt::darkBlue);
    painter.drawPie(size,0,360*16);
    painter.setBrush(Qt::darkMagenta);
    painter.drawPie(size,0,end_One*360*16);
    painter.setBrush(Qt::darkBlue);
    painter.drawRect(size2);
    painter.setBrush(Qt::darkMagenta);
    painter.drawRect(size3);


}

void pie::SetEnd(float end_one)
{
 end_One=end_one;
 repaint();
 update();
}
