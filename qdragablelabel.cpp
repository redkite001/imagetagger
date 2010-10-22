#include "qdragablelabel.h"

#include <QtGui>

QDragableLabel::QDragableLabel(QWidget *parent) :
    QLabel(parent)
{
    setAcceptDrops(true);
}

void QDragableLabel::mousePressEvent(QMouseEvent *event)
{/*
    QLabel *child = static_cast<QLabel *>(childAt(event->pos()));
    if (!child)
        return;
*/
    QPixmap pixmap = *(this->pixmap());

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos());

    QPainter painter;
    painter.begin(&pixmap);
    painter.drawPixmap(event->x() + pixmap.rect().x(), event->y(), pixmap);
    painter.end();

}
