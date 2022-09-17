#ifndef MOVEAREA_H
#define MOVEAREA_H

#include <QWidget>
#include <QMouseEvent>
#include "qdebug.h"

namespace Ui {
class moveArea;
}

class moveArea : public QWidget
{
    Q_OBJECT

public:
    explicit moveArea(QWidget *parent = nullptr);
    ~moveArea();

    void self_castrate();
    void setInOpacity(int);

private:
    Ui::moveArea *ui;

    QPoint m_mousePoint;
    QPoint movePoint;
    bool mousePress = false;
    QPoint mousePressPos;

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void move_sig(QPoint, QPoint);
};

#endif // MOVEAREA_H
