#ifndef QDROPAREA_H
#define QDROPAREA_H

#include <QLabel>

class QDropArea : public QLabel
{
    Q_OBJECT

public:
    explicit QDropArea(QWidget *parent = 0);
    void loadImage(const QString &fileName, const char *format = 0, Qt::ImageConversionFlags flags = Qt::AutoColor);

public slots:
    void fitImage();
    void clear();

signals:

protected:
     void dragEnterEvent(QDragEnterEvent *event);
     void dragMoveEvent(QDragMoveEvent *event);
     void dragLeaveEvent(QDragLeaveEvent *event);
     void dropEvent(QDropEvent *event);
     void mousePressEvent(QMouseEvent *event);
     void resizeEvent(QResizeEvent *event);

 private:
     bool convertPoinFromLabelToRealPixmap(QPoint &_labelPoint);

     QPixmap m_pixmap;
};

#endif // QDROPAREA_H
