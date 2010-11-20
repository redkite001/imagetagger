#include "qdroparea.h"

#include <QtGui>
#include <QtDebug>
#include "qdragablelabel.h"

QDropArea::QDropArea(QWidget *parent)
   :QLabel(parent)
{
    setMinimumSize(200, 200);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAlignment(Qt::AlignCenter);


    QDragableLabel *dl = new QDragableLabel("test", this);
    dl->move(5, 5);
     dl->show();
     dl->setAttribute(Qt::WA_DeleteOnClose);

    setAcceptDrops(true);
}

void QDropArea::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-fridgemagnet")) {
           if (children().contains(event->source())) {
               event->setDropAction(Qt::MoveAction);
               event->accept();
           } else {
               event->acceptProposedAction();
           }
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void QDropArea::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-fridgemagnet")) {
            if (children().contains(event->source())) {
                event->setDropAction(Qt::MoveAction);
                event->accept();
            } else {
                event->acceptProposedAction();
            }
        } else if (event->mimeData()->hasText()) {
            event->acceptProposedAction();
        } else {
            event->ignore();
        }
}

void QDropArea::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-fridgemagnet")) {
             const QMimeData *mime = event->mimeData();
             QByteArray itemData = mime->data("application/x-fridgemagnet");
                     QDataStream dataStream(&itemData, QIODevice::ReadOnly);

                     QString text;
                     QPoint offset;
                     dataStream >> text >> offset;
                     QDragableLabel *newLabel = new QDragableLabel(text, this);
                             newLabel->move(event->pos() - offset);
                             newLabel->show();
                             newLabel->setAttribute(Qt::WA_DeleteOnClose);

                             if (event->source() == this) {
                                 event->setDropAction(Qt::MoveAction);
                                 event->accept();
                             } else {
                                 event->acceptProposedAction();
                             }
    } else if (event->mimeData()->hasText()) {
          QStringList pieces = event->mimeData()->text().split(QRegExp("\\s+"),
                               QString::SkipEmptyParts);
          QPoint position = event->pos();

          foreach (QString piece, pieces) {
              QDragableLabel *newLabel = new QDragableLabel(piece, this);
              newLabel->move(position);
              newLabel->show();
              newLabel->setAttribute(Qt::WA_DeleteOnClose);

              position += QPoint(newLabel->width(), 0);
          }

          event->acceptProposedAction();
      } else {
          event->ignore();
      }
}

void QDropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
    clear();
    event->accept();
}

void QDropArea::mousePressEvent(QMouseEvent *event)
{
       QDragableLabel *child = static_cast<QDragableLabel*>(childAt(event->pos()));
       if (!child)
       {
           child = new QDragableLabel(1,1, this);
           child->move(event->pos() - child->rect().center());
       }

       QPoint hotSpot = event->pos() - child->pos();

       QByteArray itemData;
       QDataStream dataStream(&itemData, QIODevice::WriteOnly);
       dataStream << child->labelText() << QPoint(hotSpot);
       QMimeData *mimeData = new QMimeData;
         mimeData->setData("application/x-fridgemagnet", itemData);
         mimeData->setText(child->labelText());
         QDrag *drag = new QDrag(this);
          drag->setMimeData(mimeData);
          drag->setPixmap(*child->pixmap());
          drag->setHotSpot(hotSpot);

          child->hide();if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
              child->close();
          else
              child->show();
}

void QDropArea::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    //qDebug("[%d,%d] [%d,%d]", event->oldSize().width(), event->oldSize().height(), event->size().width(), event->size().height());
    //fitImage();
}

void QDropArea::loadImage(const QString &fileName, const char *format, Qt::ImageConversionFlags flags)
{
    m_pixmap = QPixmap(fileName, format, flags);
    //fitImage();
    setPixmap(m_pixmap);
}

void QDropArea::fitImage()
{   // TODO : reset a timer each time called in order to avoid resizing each time the size change when resizing the window
//    if (!m_pixmap.isNull())
//        setPixmap(m_pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void QDropArea::clear()
{
    setBackgroundRole(QPalette::Dark);
}

bool QDropArea::convertPoinFromLabelToRealPixmap(QPoint &_labelPoint)
{
    QRect labelPixmapRect;
    const QPixmap *labelPixmap = this->pixmap();

    labelPixmapRect.setSize(labelPixmap->size());

    labelPixmapRect.setX(width()/2 - labelPixmap->width()/2);
    labelPixmapRect.setY(height()/2 - labelPixmap->height()/2);

    if (_labelPoint.x() < labelPixmapRect.x() ||
            _labelPoint.x() > labelPixmapRect.x() + labelPixmapRect.width() ||
            _labelPoint.y() < labelPixmapRect.y() ||
            _labelPoint.y() > labelPixmapRect.y() + labelPixmapRect.height() )
    {
        // The point wasn't on the pixmap
        return false;
    }

    int ratio = m_pixmap.width() / labelPixmapRect.width();
    _labelPoint.setX(_labelPoint.x() - labelPixmapRect.x());
    _labelPoint.setY(_labelPoint.y() - labelPixmapRect.y());
    _labelPoint = _labelPoint * ratio;
    qDebug() << _labelPoint;
    return true;
}


