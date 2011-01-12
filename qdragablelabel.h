#ifndef QDRAGABLELABEL_H
#define QDRAGABLELABEL_H

#include <QLabel>

class QWidget;

class QDragableLabel : public QLabel
{
    Q_OBJECT
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
    explicit QDragableLabel(const int _number, const Shape _shape, const QColor &_front = Qt::red, const QColor &_background = QColor(), const QFont &_font = QFont(), QWidget *_parent = NULL);

    Shape getShape() const;
    void  setShape(const Shape);

    int  getNumber() const;
    void setNumber(const int);

    const QColor &getFrontColor() const;
    void          setFrontColor(const QColor &);

    const QColor &getBackgroundColor() const;
    void          setBackgroundColor(const QColor &);

    const QFont &getFont() const;
    void         setFont(const QFont &);

    void drawPixmap();

public slots:
    void editTag();

private:

    int m_number;
    Shape m_shape;
    QColor m_front, m_background;
    QFont m_font;
};

#endif // QDRAGABLELABEL_H
