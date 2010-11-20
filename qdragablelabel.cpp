#include "qdragablelabel.h"

#include <QImage>
#include <QFont>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QWidget>

QDragableLabel::QDragableLabel(const int &type, const int &number, QWidget *parent)
    : QLabel(parent)
{
    setAcceptDrops(true);

    int size = 15;
    resize(size,size);

    QImage image(size, size, QImage::Format_ARGB32_Premultiplied);
    image.fill(qRgba(0, 0, 0, 0));

    QFont font;
    font.setStyleStrategy(QFont::ForceOutline);

    QPainter painter;
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    painter.setPen(pen);

    painter.drawEllipse(1, 1, image.height()-2, image.height()-2);
    painter.end();

    setPixmap(QPixmap::fromImage(image));
}

QString QDragableLabel::labelText() const
{
    return m_labelText;
}
