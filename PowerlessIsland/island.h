#ifndef ISLAND_H
#define ISLAND_H

#include <QWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QCursor>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPalette>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include "movearea.h"

namespace Ui {
class island;
}

class island : public QWidget
{
    Q_OBJECT

public:
    explicit island(QWidget *parent = nullptr,
                    bool showAbove = false,
                    QFont font = QFont("Microsoft JingHei UI"),
                    QSize size = QSize(250,36),
                    QString textDisplay = "Powerless Island",
                    int opaAll = 10, int opaIn = 6);
    ~island();

    QRect annimation_rect;
    bool old_flag;
    bool new_flag;

private:
    Ui::island *ui;
    moveArea *area;

    void playExpandAnimation();
    void playReduceAnimation();
    QPropertyAnimation *animation;
    QTimer *aniTimer;
    bool animation_end = true;

    bool eventFilter(QObject * obj, QEvent * event);

    int minHeight;
    int maxHeight;
    int minWidth;

private slots:
    void move_slot(QPoint, QPoint);

    void set_animation_ended();

};

#endif // ISLAND_H
