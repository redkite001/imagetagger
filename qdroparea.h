#ifndef QDROPAREA_H
#define QDROPAREA_H

#include <QLabel>

class QDropArea : public QLabel
{
    Q_OBJECT

public:
    explicit QDropArea(QWidget *parent = 0);

public slots:
    void clear();

signals:

protected:
     void dragEnterEvent(QDragEnterEvent *event);
     void dragMoveEvent(QDragMoveEvent *event);
     void dragLeaveEvent(QDragLeaveEvent *event);
     void dropEvent(QDropEvent *event);

 private:
     QLabel *label;
}

#endif // QDROPAREA_H
