#ifndef QDRAGABLELABEL_H
#define QDRAGABLELABEL_H

#include <QLabel>

class QDragableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit QDragableLabel(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);

signals:

public slots:

};

#endif // QDRAGABLELABEL_H
