#include "movearea.h"
#include "qevent.h"
#include "ui_movearea.h"

moveArea::moveArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::moveArea)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->setMouseTracking(true);
    this->installEventFilter(this); //安装事件过滤器
}

moveArea::~moveArea()
{
    delete ui;
}

void moveArea::mousePressEvent(QMouseEvent *event)
{
    this->setCursor(QCursor(Qt::ClosedHandCursor));

    if(event->button() == Qt::LeftButton)
    {
        mousePress = true;
    }
    //窗口移动距离
    /*
    qDebug() << "globalPosition" <<event->globalPosition().toPoint() << "pos" << pos();
    movePoint = event->globalPosition().toPoint() - pos();*/

    qDebug() << "globalPosition" <<event->globalPos() << "pos" << pos();
    movePoint = event->globalPos() - pos();

    mousePressPos = mapFromGlobal(QCursor().pos()) + pos();
}

void moveArea::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    mousePress = false;
    this->setCursor(QCursor(Qt::SizeAllCursor));
}

void moveArea::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePress)
    {
        QPoint movePos = event->globalPos();//窗口初始位置
        emit(move_sig(movePos, mousePressPos));
        //move(movePos - movePoint);
    }
}

void moveArea::self_castrate()
{
    delete ui->label;
    delete ui->verticalLayout;
    this->setMinimumSize(0,0);
    this->setMaximumSize(114514,114514);
}

void moveArea::setInOpacity(int o)
{
    double d = (double)o;
    //qDebug() << "opaAll = " << d;
    d = d /10;
    QString opa = QString::number(d);
    QString qss1 = "QLabel{background-color: rgba(255, 255, 255, ";
    QString qss2 = ");border-radius:0px;}";
    ui->label->setStyleSheet(qss1 + opa + qss2);
    //qDebug() << qss1 + opa + qss2;
}
