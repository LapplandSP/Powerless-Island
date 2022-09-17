#include "widget.h"
#include "./ui_widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    initWidget();

    //land = new island();

    logo = new logoArea(ui->widget_logo,ui->widget_logo->pos());
    logo->setMinimumSize(250,32);/*
    QWidget *tmp = ui->mainLayout->itemAt(0)->widget();
    ui->mainLayout->replaceWidget(ui->mainLayout->itemAt(0)->widget(),logo);
    tmp->deleteLater();*/
    ui->widget_logo->layout()->addWidget(logo);
    //ui->mainLayout->addWidget(logo);

    connect(logo,SIGNAL(move_sig(QPoint, QPoint)),this,SLOT(move_slot(QPoint, QPoint)));
/*
    area = new moveArea(this);
    area->self_castrate();
    area->setMinimumSize(250,32);
    area->setStyleSheet("background-color:black;border:100px solid yellow;");
    ui->mainLayout->addWidget(area);

    connect(area,SIGNAL(move_sig(QPoint, QPoint)),this,SLOT(move_slot(QPoint, QPoint)));*/

    connect(ui->closeBtn,SIGNAL(clicked()),this,SLOT(closeBtn_clicked()));
    connect(ui->showBtn,SIGNAL(clicked()),this,SLOT(show_island()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(slot_quit()));
    connect(ui->lineEdit,SIGNAL(editingFinished()),this,SLOT(examTextSize()));

    if (!QSystemTrayIcon::isSystemTrayAvailable())      //判断系统是否支持系统托盘图标
    {
        ui->closeBtn->hide();
    }
    CreatTrayIcon();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initWidget()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    /*
    QPalette pal = palette();
    pal.setColor(QPalette::Background, qRgba(255,255,255,1));
    this->setPalette(pal);*/
    this->setStyleSheet("border-Radius:0px;border:4px;background-color:white;color:black;");

    ui->label->hide();
    ui->boot->hide();

    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(0, 0);
    shadow_effect->setColor(Qt::gray);
    shadow_effect->setBlurRadius(11);
    ui->frame->setGraphicsEffect(shadow_effect);

    this->setWindowIcon(QIcon(":/icons/icons/mount_fuji_flat.svg"));

    ui->fontComboBox->view()->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    QFile file(":/qss/qss/scrollbar.qss");
    file.open(QFile::ReadOnly);
    ui->fontComboBox->view()->verticalScrollBar()->setStyleSheet(file.readAll());
}

void Widget::closeBtn_clicked()
{
    SwitchTrayIcon();
    //this->hide();
}

void Widget::show_island()
{
    QFontMetrics fm(ui->fontComboBox->currentFont());
    if(!(fm.boundingRect(ui->lineEdit->text()).width() >= ui->spinBox_width->value()))
    {

        //island(QWidget *parent, bool showAbove, QFont font, QSize size, QString text, int opaAll, int opaIn)
        if(land != NULL)
        {
            land->close();
            delete land;
            land = NULL;
            ui->showBtn->setText("显示「呆滞岛」");
        }
        else
        {
            QSize size;
            size.setHeight(ui->spinBox_height->value());
            size.setWidth(ui->spinBox_width->value());
            land = new island(NULL,ui->keepAbove->isChecked(),
                              ui->fontComboBox->currentFont(),
                              size,ui->lineEdit->text(),
                              ui->spinBox_opaAll->value(),
                              ui->spinBox_opaIn->value());

            land->show();
            qDebug() << mapFromGlobal(ui->showBtn->pos()).y();
            land->move(land->x(),land->y() - mapFromGlobal(ui->showBtn->pos()).y());

            ui->showBtn->setText("隐藏「呆滞岛」");
        }
    }
}

void Widget::slot_quit()
{
    this->close();
}

void Widget::move_slot(QPoint p, QPoint mousePos)
{
    //qDebug() << "slot p = " << p;
    move(p - mousePos);
}

void Widget::examTextSize()
{
    QFontMetrics fm(ui->fontComboBox->currentFont());
    if(fm.boundingRect(ui->lineEdit->text()).width() >= ui->spinBox_width->value())
    {
        ui->lineEdit->setText("文本过长，请加大控件宽度。");
    }
}

void Widget::CreatTrayMenu()
{
    restoreWinAction = new QAction("打开设置",this);
    quitAction = new QAction("退出",this);

    this->connect(restoreWinAction,SIGNAL(triggered()),this,SLOT(showNormal()));
    this->connect(quitAction,SIGNAL(triggered()),qApp,SLOT(quit()));

    myMenu = new QMenu((QWidget*)QApplication::desktop());

    myMenu->addAction(restoreWinAction);
    myMenu->addSeparator();     //加入一个分离符
    myMenu->addAction(quitAction);
}

void Widget::CreatTrayIcon()
{
    CreatTrayMenu();

    if (!QSystemTrayIcon::isSystemTrayAvailable())      //判断系统是否支持系统托盘图标
    {
        return;
    }

    myTrayIcon = new QSystemTrayIcon(this);

    myTrayIcon->setIcon(QIcon(":/icons/icons/mount_fuji_flat.svg"));   //设置图标图片
    setWindowIcon(QIcon(":/icons/icons/mount_fuji_flat.svg"));  //把图片设置到窗口上

    myTrayIcon->setToolTip("呆滞岛\n "
                           "v1.0");    //托盘时，鼠标放上去的提示信息

    //myTrayIcon->showMessage("","",QSystemTrayIcon::Information,10000);



    myTrayIcon->setContextMenu(myMenu);     //设置托盘上下文菜单

    myTrayIcon->show();
    this->connect(myTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:

    case QSystemTrayIcon::DoubleClick:
        showNormal();
        break;
    case QSystemTrayIcon::MiddleClick:
        myTrayIcon->showMessage("呆滞岛","您可以从系统托盘再次打开设置",QSystemTrayIcon::Information,10000);
        break;

    default:
        break;
    }

    /**/
}
/*
void Widget::closeEvent(QCloseEvent *event)
{
    //qDebug() << "event";
    if (myTrayIcon->isVisible())
    {
        myTrayIcon->showMessage(QString::fromLocal8Bit("呆滞岛"),QString::fromLocal8Bit("您可以从系统托盘再次打开设置"),QSystemTrayIcon::Information,5000);

        hide();     //最小化
        event->ignore();
    }
    else
        event->accept();
}
*/
void Widget::SwitchTrayIcon()
{
    qDebug() << "event";
    if (myTrayIcon->isVisible())
    {
        myTrayIcon->showMessage("呆滞岛","您可以从系统托盘再次打开设置",QSystemTrayIcon::Information,5000);

        this->hide();     //最小化
    }
}
