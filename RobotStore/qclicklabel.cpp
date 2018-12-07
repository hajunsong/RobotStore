#include "qclicklabel.h"

QClickLabel::QClickLabel(QWidget *parent) : QLabel(parent)
{

}

void QClickLabel::mouseMoveEvent(QMouseEvent *ev)
{
    this->x = ev->x();
    this->y = ev->y();
    emit mousePos();
}

void QClickLabel::mousePressEvent(QMouseEvent *ev)
{
    this->x = ev->x();
    this->y = ev->y();
    emit mousePressed();
}

void QClickLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    this->x = ev->x();
    this->y = ev->y();
    emit mouseReleased();
}

void QClickLabel::leavEvent(QEvent *)
{
    emit mouseLeft();
}
