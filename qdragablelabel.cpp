#include "qdragablelabel.h"

#include <QImage>
#include <QFont>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QWidget>

QDragableLabel::QDragableLabel(const int _number, const Shape _shape, QWidget *_parent)
   :QLabel(_parent),
    m_number(_number),
    m_shape(_shape),
    m_front(Qt::red),
    m_background(Qt::white)
{
    setAcceptDrops(true);
    drawPixmap();
}

QDragableLabel::QDragableLabel(const int _number, const Shape _shape, const QColor &_front, const QColor &_background, QWidget *_parent)
   :QLabel(_parent),
    m_number(_number),
    m_shape(_shape),
    m_front(_front),
    m_background(_background)
{
    setAcceptDrops(true);
    drawPixmap();
}

QDragableLabel::Shape QDragableLabel::getShape() const
{
    return m_shape;
}

int QDragableLabel::getNumber() const
{
    return m_number;
}

void QDragableLabel::drawPixmap()
{
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
