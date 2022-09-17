#include "island.h"
#include "ui_island.h"

island::island(QWidget *parent, bool showAbove, QFont font, QSize size, QString textDisplay, int opaAll, int opaIn) :
    QWidget(parent),
    ui(new Ui::island)
{
    ui->setupUi(this);

    if(showAbove)
    {
        this->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint);
    }
    else
    {
        this->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);
    }
    this->setFont(font);
    ui->widget->setFont(font);
    minWidth = size.width();
    if(minWidth >= 250)
    {
        this->setMinimumWidth(minWidth);
        this->setMaximumWidth(minWidth);
    }

    maxHeight = size.height();


    //this->setAttribute(Qt::WA_TranslucentBackground);
    /*
    QPalette pal = palette();
    pal.setColor(QPalette::Background, qRgba(255,255,255,1));
    this->setPalette(pal);*/

    double d = (double)opaAll;
    //qDebug() << "opaAll = " << d;
    d = d /10;
    this->setWindowOpacity(d);
    this->setStyleSheet("border-Radius:16px;border:0px;background-color:green;color:white;");



    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover, true); //开启悬停事件
    this->installEventFilter(this); //安装事件过滤器

    /*add widgets*/
    area = new moveArea();
    ui->widget->layout()->addWidget(area);
    connect(area,SIGNAL(move_sig(QPoint, QPoint)),this,SLOT(move_slot(QPoint, QPoint)));
    area->setInOpacity(opaIn);

    QLabel *text = new QLabel(ui->widget);
    text->setText(textDisplay);
    ui->widget->layout()->addWidget(text);
    text->setAlignment(Qt::AlignCenter);
    text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    text->setFont(ui->widget->font());
    text->setStyleSheet("background-color:rgba(255,255,255,0);border:0px;");
    text->setWordWrap(true);
    text->setMinimumWidth(200);
/*
    QSpacerItem *spacer;
    spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    ui->widget->layout()->addItem(spacer);*/
    /*add widgets*/

    animation = new QPropertyAnimation(this,"geometry");
    aniTimer = new QTimer(this);
    aniTimer->setSingleShot(true);
    //connect(aniTimer,SIGNAL(timeout()),this,SLOT(set_animation_ended()));
    connect(animation,SIGNAL(finished()),this,SLOT(set_animation_ended()));
}

island::~island()
{
    delete ui;
}

bool island::eventFilter(QObject * obj, QEvent * event)
{
    annimation_rect = QRect(this->pos().x(),this->pos().y(),this->width(),this->height());

    QPoint mousePos = QCursor().pos();

    if(annimation_rect.contains(mousePos))
    {
        old_flag = new_flag;
        new_flag = true;
    }
    else
    {
       old_flag = new_flag;
       new_flag = false;
    }


    if((old_flag != new_flag) && animation_end)
    {
        if(old_flag && !new_flag && height() >= maxHeight - 1)   //鼠标在外面
        {
            qDebug() << "playreduce";
            playReduceAnimation();
        }

        if(!old_flag && new_flag && height() != maxHeight)   //鼠标进入不灵动岛
        {
            qDebug() << "playexpand";
            playExpandAnimation();
        }
    }
    return true;
}

void island::move_slot(QPoint p, QPoint mousePos)
{
    move(p - mousePos);
}

void island::playExpandAnimation()
{
    if(animation_end)
    {
        aniTimer->setSingleShot(true);
        animation->setDuration(600);

        this->setGeometry(QRect(pos().x(),pos().y(), this->width() , 32));
        animation->setEndValue(QRect(pos().x(),pos().y(), this->width() , maxHeight));
        animation->setStartValue(QRect(pos().x(),pos().y(), this->width() , 32));

        animation->setEasingCurve(QEasingCurve::OutQuart);

        animation->start();
        aniTimer->start(600);
        //qDebug() << "started";
        animation_end = false;
    }
}

void island::playReduceAnimation()
{
    if(animation_end)
    {
        aniTimer->setSingleShot(true);

        animation->setDuration(600);
        this->setGeometry(QRect(pos().x(),pos().y(), this->width() , maxHeight));
        animation->setEndValue(QRect(pos().x(),pos().y(), this->width() , 32));
        animation->setStartValue(QRect(pos().x(),pos().y(), this->width() , maxHeight));
        animation->setEasingCurve(QEasingCurve::OutQuart);

        animation->start();
        aniTimer->start(600);
        animation_end = false;
    }
}

void island::set_animation_ended()
{
    //this->setGeometry(QRect(pos().x(),pos().y(), this->width() , 250));

    animation_end = true;

    aniTimer->disconnect();
    aniTimer->deleteLater();

    aniTimer = new QTimer(this);
    aniTimer->setSingleShot(true);
    connect(aniTimer,SIGNAL(timeout()),this,SLOT(set_animation_ended()));

    annimation_rect = QRect(this->pos().x(),this->pos().y(),this->width(),this->height());
    QPoint mousePos = QCursor().pos();

    if(!annimation_rect.contains(mousePos) && (height() == maxHeight))
    {
        playReduceAnimation();
    }
}
