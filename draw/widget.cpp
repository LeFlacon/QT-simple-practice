#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    this->isPressed=false;

    scale=1;
    btn=new QPushButton(this);
    btn->setText(tr("放大"));
    btn->move(10,10);
    connect(btn,SIGNAL(clicked()),this,SLOT(fangda()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *p)
{

    QPainter ptr(this);
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(5);
    ptr.setPen(pen);
    if(scale!=1){
        scale =1; //scale重新置1
    }
    for(int i=0;i<lines.size();i++){
        myLine *pline=lines[i];
        ptr.drawLine(pline->startp,pline->endp);
    }
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    setCursor(Qt::PointingHandCursor);
    startp=e->pos();
    endp=e->pos();
    this->isPressed=true;
}
void Widget::mouseMoveEvent(QMouseEvent *e)
{
    if(this->isPressed){
        endp=e->pos();
        myLine *line=new myLine;
        line->startp=startp;
        line->endp=endp;
        this->lines.push_back(line);
        update();
        startp=endp;
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    setCursor(Qt::ArrowCursor);
    this->isPressed=false;
}

void Widget::fangda()
{
    QPainter ptr(this);
    scale*=2;
    for(int i=0;i<lines.size();i++){
        myLine *pline=lines[i];
        pline->startp=pline->startp*scale;//放大了，所以之前的坐标每个都乘scale
        pline->endp=pline->endp*scale;
    }
    repaint();//立即显示
}











