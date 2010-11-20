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
    m_background(QColor())
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
//    resize(size,size);

    QFontMetrics metric(font());
    QSize size = metric.size(Qt::TextSingleLine, QString::number(m_number));
    QImage image;

    switch (m_shape) {
        case Cercle:
            image = QImage(qMax(size.width(), size.height()) + 4, qMax(size.width(), size.height()) + 4, QImage::Format_ARGB32_Premultiplied);
            break;
        case Diamond:
            //image = QImage(qMax(size.width(), size.height()) + 4, qMax(size.width(), size.height()) + 4, QImage::Format_ARGB32_Premultiplied);
            break;
        case Rectangle:
            image = QImage(size.width() + 6, size.height(), QImage::Format_ARGB32_Premultiplied);
            break;
        case Square:
            image = QImage(qMax(size.width() + 6, size.height()), qMax(size.width() + 6, size.height()), QImage::Format_ARGB32_Premultiplied);
            break;
        case Triangle:
            break;
        case Triangle2:
            break;
    }

    image.fill(qRgba(0, 0, 0, 0));

    QFont font;
    font.setStyleStrategy(QFont::ForceOutline);

    QPainter painter;
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;
    pen.setColor(m_front);
    pen.setWidth(2);

    painter.setPen(pen);
    if (m_background.isValid())
        painter.setBrush(QBrush(m_background));

    switch (m_shape) {
        case Cercle:
            painter.drawEllipse(1, 1, image.width() - 2, image.height() - 2);
            break;
        case Diamond:
           //painter.dra(1, 1, image.width() - 2, image.height() - 2);
            break;
        case Rectangle:
        case Square:
            painter.drawRect(1, 1, image.width() - 2, image.height() - 2);
            break;
        case Triangle:
            break;
        case Triangle2:
            break;
    }

    painter.drawText(image.rect(), Qt::AlignCenter, QString::number(m_number));
    painter.end();

    setPixmap(QPixmap::fromImage(image));
}

const QColor & QDragableLabel::getFrontColor() const
{

    return m_front;
}

const QColor & QDragableLabel::getBackgroundColor() const
{
    return m_background;
}
