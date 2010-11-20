#ifndef QDRAGABLELABEL_H
#define QDRAGABLELABEL_H

#include <QLabel>

class QWidget;

class QDragableLabel : public QLabel
{
public:
    explicit QDragableLabel(const int &type, const int &number, QWidget *parent);
    int getType() const;
    int getNumber() const;

private:
    int type;
    int number;
};

#endif // QDRAGABLELABEL_H
