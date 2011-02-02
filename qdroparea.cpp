#include "qdroparea.h"

#include <QtGui>    // Remove this
#include <QtDebug>
#include "qdragablelabel.h"
#include "mainwindow.h"

QDropArea::QDropArea(QWidget *parent)
   :QLabel(tr("< Choose an image >"), parent),
    isNewTag(false)
{
    //setFrameStyle(QFrame::Box  | QFrame::Sunken);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);

    m_temporaryTag = NULL;
}

void QDropArea::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-tag")) {
       if (children().contains(event->source())) {
           event->setDropAction(Qt::MoveAction);
           event->accept();
       } else {
           event->acceptProposedAction();
       }
    } else {
        event->ignore();
    }
}

void QDropArea::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-tag")) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void QDropArea::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-tag")) {
        const QMimeData *mime = event->mimeData();
        QByteArray itemData = mime->data("application/x-tag");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        int number, tmpShape;
        QDragableLabel::Shape shape;
        QColor front, background;
        QFont font;
        QPoint offset;
        dataStream >> number >> tmpShape >> front >> background >> font >> offset;
        shape = (QDragableLabel::Shape)tmpShape;

        QDragableLabel *newLabel = new QDragableLabel(number, shape, front, background, font, this);
        newLabel->move(event->pos() - offset);
        newLabel->show();
        newLabel->setAttribute(Qt::WA_DeleteOnClose);
        if (isNewTag) emit tagAdded(newLabel);
        else          emit tagMoved(newLabel);

        m_tagList.append(newLabel);

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction(); // When the tag comes from an other instance of the application
        }
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
    if (!pixmap())
        return;

    // Get the tag if we have clicked on one
    QDragableLabel *child = static_cast<QDragableLabel*>(childAt(event->pos()));

    if (event->button() == Qt::LeftButton) {
        if (child) {
            isNewTag = false;
        } else {
            // Create a new tag if we have not clicked on an existing one
            MainWindow *mw = (MainWindow *)topLevelWidget();
            child = new QDragableLabel(mw->getCurrentNumber(), mw->getCurrentShape(), mw->getCurrentFrontColor(), mw->getCurrentBackgroundColor(), mw->getCurrentFont(), this);
            child->move(event->pos() - child->pixmap()->rect().center());
            child->setAttribute(Qt::WA_DeleteOnClose);
            isNewTag = true;
        }

        QPoint hotSpot = event->pos() - child->pos();

        // Save datas in the QDrag
        QByteArray itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);
        dataStream << child->getNumber() << (qint32)child->getShape() << child->getFrontColor() << child->getBackgroundColor() << child->getFont() << QPoint(hotSpot);
        QMimeData *mimeData = new QMimeData;
        mimeData->setData("application/x-tag", itemData);
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(*child->pixmap());
        drag->setHotSpot(hotSpot);

        child->hide();

        if (drag->exec(Qt::MoveAction, Qt::MoveAction) == Qt::MoveAction) {
            m_temporaryTag = child;
            removeTag();
        } else
            child->show();
    } else if (event->button() == Qt::RightButton) {
        if (child) {
            m_temporaryTag = child;

            QMenu popupMenu(tr("Label %1").arg(child->getNumber()), this);

            popupMenu.addAction(tr("Edit"), child, SLOT(editTag()));
            popupMenu.addAction(tr("Delete"), this, SLOT(removeTag()));

            popupMenu.exec(QCursor::pos());

            m_temporaryTag = NULL;
        } else {
            QMenu popupMenu(tr("All Labels"), this);
            popupMenu.addAction(tr("Delete All"), this, SLOT(removeTag()));

            popupMenu.exec(QCursor::pos());

            m_temporaryTag = NULL;
        }
    }
}

void QDropArea::resizeEvent(QResizeEvent *event)
{
/*
    qreal scaleFactor = (qreal)event->size().width() / event->oldSize().width();
    QList<QDragableLabel *> allTags = findChildren<QDragableLabel *>();

    foreach (QDragableLabel *tag, allTags)
        tag->move(tag->pos() * scaleFactor);
*/
}

void QDropArea::loadImage(const QString &fileName, const char *format, Qt::ImageConversionFlags flags)
{
    m_pixmap = QPixmap(fileName, format, flags);
    fixedImage();
    //fitImage();
}

void QDropArea::fitImage()
{   // TODO : reset a timer each time called in order to avoid resizing each time the size change when resizing the window
    if (!m_pixmap.isNull()) {
        setPixmap(m_pixmap.scaled(parentWidget()->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        QSize sizeOfScalledPixmap = m_pixmap.size();
        sizeOfScalledPixmap.scale(parentWidget()->size(), Qt::KeepAspectRatio);
        resize(sizeOfScalledPixmap);
    }
}

void QDropArea::fixedImage()
{
    if (!m_pixmap.isNull()) {
        setPixmap(m_pixmap);
        resize(m_pixmap.size());
    }
}

void QDropArea::clear()
{
    setBackgroundRole(QPalette::Dark);
    while(!m_tagList.isEmpty())
        delete m_tagList.takeFirst();
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

void QDropArea::removeTag()
{
    if (m_temporaryTag) {
        m_tagList.removeAll(m_temporaryTag);
        m_temporaryTag->close();
        m_temporaryTag = NULL;
    } else {
        while(!m_tagList.isEmpty())
            m_tagList.takeFirst()->close();
    }
    qDebug("Nbr of tags : %d", m_tagList.size());
}


