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
    : QLabel(_parent)
    , m_number(_number)
    , m_shape(_shape)
    , m_front(Qt::red)
    , m_background(QColor())
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

quint32 QDragableLabel::getNumber() const
{
    return m_number;
}


void QDragableLabel::setNumber(const quint32 _number)
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
    // FIXIT: Find a better way to refresh the tag size on the screen
    //        Without this, if the editted tag is bigger, it is painted only on the old(smaller) region
    hide();
    show();
}

QTextStream &operator<<(QTextStream &out, const QDragableLabel &tag)
{
    out << tag.getNumber() << endl;
    out << tag.getShape() << endl;
    out << tag.getFrontColor().red() << ";" << tag.getFrontColor().green() << ";" << tag.getFrontColor().blue() << ";" << tag.getFrontColor().alpha() << endl;
    out << tag.getBackgroundColor().red() << ";" << tag.getBackgroundColor().green() << ";" << tag.getBackgroundColor().blue() << ";" << tag.getBackgroundColor().alpha() << endl;
    out << tag.getFont().family() << ";" << tag.getFont().pointSize() << ";" << tag.getFont().weight() << ";" << tag.getFont().italic() << endl;
    out << tag.pos().x() << ";" << tag.pos().y() << endl;
    out << endl;

    return out;
}

QTextStream &operator>>(QTextStream &in, QDragableLabel &tag)
{
    QString tmp;
    quint32 number;
    QDragableLabel::Shape shape;
    QColor front, background;
    QFont font;
    QPoint pos;

    number = in.readLine().toInt();
    shape = (QDragableLabel::Shape)in.readLine().toInt();
    tmp = in.readLine();
    front.setRgb(tmp.section(";", 0, 0).toInt(), tmp.section(";", 1, 1).toInt(), tmp.section(";", 2, 2).toInt(), tmp.section(";", 3, 3).toInt());
    tmp = in.readLine();
    background.setRgb(tmp.section(";", 0, 0).toInt(), tmp.section(";", 1, 1).toInt(), tmp.section(";", 2, 2).toInt(), tmp.section(";", 3, 3).toInt());
    tmp = in.readLine();
    font = QFont(tmp.section(";", 0, 0), tmp.section(";", 1, 1).toInt(), tmp.section(";", 2, 2).toInt(), tmp.section(";", 3, 3).toInt());
    tmp = in.readLine();
    pos = QPoint(tmp.section(";", 0, 0).toInt(), tmp.section(";", 1, 1).toInt());
    in.readLine();

    tag.setNumber(number);
    tag.setShape(shape);
    tag.setFrontColor(front);
    tag.setBackgroundColor(background);
    tag.setFont(font);
    tag.move(pos);
    tag.drawPixmap();
    tag.hide();
    tag.show();
    return in;
}
