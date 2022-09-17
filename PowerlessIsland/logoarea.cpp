#include "logoarea.h"
#include "ui_logoarea.h"

logoArea::logoArea(QWidget *parent,QPoint p) :
    QWidget(parent),
    ui(new Ui::logoArea)
{
    ui->setupUi(this);

    setParent(parent);
    parentPos = p;

    this->setWindowFlags(Qt::FramelessWindowHint);

    this->setMouseTracking(true);
    this->installEventFilter(this); //安装事件过滤器

    ui->label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

logoArea::~logoArea()
{
    delete ui;
}

void logoArea::mousePressEvent(QMouseEvent *event)
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

    qDebug() << "globalPosition" <<event->globalPos() << "pos" << pos() << "parentpos" << parentPos;
    movePoint = event->globalPos() - pos();

    parentPos = QPoint(11,11);
    mousePressPos = mapFromGlobal(QCursor().pos()) + pos() + 2 * parentPos;
}

void logoArea::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    mousePress = false;
    this->setCursor(QCursor(Qt::SizeAllCursor));
}

void logoArea::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePress)
    {
        qDebug() << "press";
        QPoint movePos = event->globalPos();//窗口初始位置
        emit(move_sig(movePos, mousePressPos));
        //move(movePos - movePoint);
    }
}
