#include "qdragablelabel.h"

#include "edittagdialog.h"

#include <QImage>
#include <QFont>
#include <QInputDialog>
#include <QMouseEvent>
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

QDragableLabel::QDragableLabel(const int _number, const Shape _shape, const QColor &_front, const QColor &_background, const QFont &_font, QWidget *_parent)
   :QLabel(_parent),
    m_number(_number),
    m_shape(_shape),
    m_front(_front),
    m_background(_background),
    m_font(_font)
{
    setAcceptDrops(true);
    drawPixmap();
}

QDragableLabel::Shape QDragableLabel::getShape() const
{
    return m_shape;
}


void QDragableLabel::setShape(const Shape _shape)
{
    m_shape = _shape;
}

int QDragableLabel::getNumber() const
{
    return m_number;
}


void QDragableLabel::setNumber(const int _number)
{
    m_number = _number;
}

void QDragableLabel::drawPixmap()
{
    //font.setStyleStrategy(QFont::ForceOutline);

    QFontMetrics metric(m_font);
    QRect rect = metric.boundingRect(QString::number(m_number));
    //QSize rect = metric.size(Qt::TextSingleLine, QString::number(m_number));
    QImage image;

    switch (m_shape) {
        case Cercle:
            image = QImage(qMax(rect.width(), rect.height()) + 6, qMax(rect.width(), rect.height()) + 6, QImage::Format_ARGB32_Premultiplied);
            break;
        case Diamond:
            //image = QImage(qMax(size.width(), size.height()) + 4, qMax(size.width(), size.height()) + 4, QImage::Format_ARGB32_Premultiplied);
            break;
        case Rectangle:
            image = QImage(rect.width() + 8, rect.height() + 6, QImage::Format_ARGB32_Premultiplied);
            break;
        case Square:
            image = QImage(qMax(rect.width() + 6, rect.height() + 4), qMax(rect.width() + 6, rect.height() + 4), QImage::Format_ARGB32_Premultiplied);
            break;
        case Triangle:
            break;
        case Triangle2:
            break;
    }

    image.fill(qRgba(0, 0, 0, 0));

    QPainter painter;
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(m_font);

    QPen pen;
    pen.setColor(m_front);
    pen.setWidth(2);

    painter.setPen(pen);
    if (m_background.isValid())
        painter.setBrush(QBrush(m_background));

    switch (m_shape) {
        case Cercle:
            painter.drawEllipse(pen.width()/2, pen.width()/2, image.width() - pen.width(), image.height() - pen.width());
            break;
        case Diamond:
           //painter.dra(1, 1, image.width() - 2, image.height() - 2);
            break;
        case Rectangle:
        case Square:
            painter.drawRect(pen.width()/2, pen.width()/2, image.width() - pen.width(), image.height() - pen.width());
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

void QDragableLabel::setFrontColor(const QColor &_color)
{
    m_front = _color;
}

const QColor & QDragableLabel::getBackgroundColor() const
{
    return m_background;
}


void QDragableLabel::setBackgroundColor(const QColor &_color)
{
    m_background = _color;
}

const QFont & QDragableLabel::getFont() const
{
    return m_font;
}

void QDragableLabel::setFont(const QFont &_font)
{
    m_font = _font;
}

void QDragableLabel::editTag()  //TODO add the choice of shape, color, ...
{
    EditTagDialog *diag = new EditTagDialog(*this, this->parentWidget());
    diag->exec();

    drawPixmap();
    hide();
    show();
}
