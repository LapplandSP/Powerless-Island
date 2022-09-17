#ifndef LOGOAREA_H
#define LOGOAREA_H

#include <QWidget>
#include <QMouseEvent>
#include "movearea.h"

namespace Ui {
class logoArea;
}

class logoArea : public QWidget
{
    Q_OBJECT

public:
    explicit logoArea(QWidget *parent = nullptr, QPoint parentPos = QPoint(0,0));
    ~logoArea();

private:
    Ui::logoArea *ui;

    QPoint m_mousePoint;
    QPoint movePoint;
    bool mousePress = false;
    QPoint mousePressPos;

    QPoint parentPos;

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void move_sig(QPoint, QPoint);
};

#endif // LOGOAREA_H
