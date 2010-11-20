#ifndef QDRAGABLELABEL_H
#define QDRAGABLELABEL_H

#include <QLabel>

class QWidget;

class QDragableLabel : public QLabel
{
public:
    enum Shape {
        Cercle = 0,
        Diamond = 1,
        Rectangle = 2,
        Square = 3,
        Triangle = 4,
        Triangle2 = 5
    };

    explicit QDragableLabel(const int _number, const Shape _shape, QWidget *_parent);
    explicit QDragableLabel(const int _number, const Shape _shape, const QColor &_front = Qt::red, const QColor &_background = QColor(), QWidget *_parent = NULL);

    Shape         getShape() const;
    int           getNumber() const;
    const QColor &getFrontColor() const;
    const QColor &getBackgroundColor() const;

    void drawPixmap();

private:
    int m_number;
    Shape m_shape;
    QColor m_front, m_background;
};

#endif // QDRAGABLELABEL_H
