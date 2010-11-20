#ifndef QDRAGABLELABEL_H
#define QDRAGABLELABEL_H

#include <QLabel>

class QWidget;

class QDragableLabel : public QLabel
{
public:
    enum Shape {
        Cercle,
        Diamond,
        Rectangle,
        Triangle,
        Triangle2
    };

    explicit QDragableLabel(const int _number, const Shape _shape, QWidget *_parent);
    explicit QDragableLabel(const int _number, const Shape _shape, const QColor &_front = Qt::red, const QColor &_background = Qt::white, QWidget *_parent = NULL);

    Shape getShape() const;
    int getNumber() const;

    void drawPixmap();

private:
    int m_number;
    Shape m_shape;
    QColor m_front, m_background;
};

#endif // QDRAGABLELABEL_H
