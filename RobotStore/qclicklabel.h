#ifndef QCLICKLABEL_H
#define QCLICKLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QDebug>

class QClickLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QClickLabel(QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void leavEvent(QEvent *);

    int x, y;

signals:
    void mousePressed();
    void mouseReleased();
    void mousePos();
    void mouseLeft();
};

#endif // QCLICKLABEL_H
