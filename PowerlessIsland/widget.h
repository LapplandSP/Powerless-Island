#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFontMetrics>
#include <QSystemTrayIcon>
#include <QAction>
#include <qmenu.h>
#include <QListView>
#include <QAbstractItemView>
#include <QScrollBar>
#include "island.h"
#include "movearea.h"
#include "logoarea.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    void initWidget();
    island *land = NULL;
    moveArea *area;
    logoArea *logo;

    void CreatTrayMenu();
    void CreatTrayIcon();

    QSystemTrayIcon *myTrayIcon;
    QMenu *myMenu;
    QAction *restoreWinAction;
    QAction *quitAction;


private slots:
    void closeBtn_clicked();
    void show_island();
    void slot_quit();
    void move_slot(QPoint, QPoint);

    void examTextSize();

public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void SwitchTrayIcon();
/*
protected:
    void closeEvent(QCloseEvent *event);*/
};
#endif // WIDGET_H
