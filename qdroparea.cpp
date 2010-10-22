#include "qdroparea.h"

#include <QtGui>
#include <QtDebug>

QDropArea::QDropArea(QWidget *parent) :
    QLabel(parent)
{
    setMinimumSize(200, 200);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAlignment(Qt::AlignCenter);
    setAcceptDrops(true);
//    setScaledContents(true);
//    setAutoFillBackground(true);
    clear();
}

void QDropArea::dragEnterEvent(QDragEnterEvent *event)
{
    setText(tr("<drop content>"));
    setBackgroundRole(QPalette::Highlight);

    event->acceptProposedAction();
//    emit changed(event->mimeData());
}

void QDropArea::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
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
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasImage()) {
        setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
    } else if (mimeData->hasHtml()) {
        setText(mimeData->html());
        setTextFormat(Qt::RichText);
    } else if (mimeData->hasText()) {
        setText(mimeData->text());
        setTextFormat(Qt::PlainText);
    } else if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        QString text;
        for (int i = 0; i < urlList.size() && i < 32; ++i) {
            QString url = urlList.at(i).path();
            text += url + QString("\n");
        }
        setText(text);
    } else {
        setText(tr("Cannot display data"));
    }

    setBackgroundRole(QPalette::Dark);
    event->acceptProposedAction();
}

void QDropArea::dragLeaveEvent(QDragLeaveEvent *event)
{
    clear();
    event->accept();
}

void QDropArea::mousePressEvent(QMouseEvent *event)
{
    QPixmap pixmap = QPixmap(":/tags/tag1");
/*
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos());
*/
    //qDebug() << event->pos();
    //qDebug() << this->pixmap()->rect();
    //QPixmap tempPixmap = *this->pixmap();
    QPoint realPoint = event->pos();
    if (!convertPoinFromLabelToRealPixmap(realPoint))
        return;
    QPainter painter;
    painter.begin(&m_pixmap);
    painter.drawRect(realPoint.x() - 60, realPoint.y()- 60, 120, 120);
    painter.drawText(realPoint.x() - 50, realPoint.y()- 50, 100, 100, Qt::AlignCenter, "1");
    //painter.drawPixmap(event->x() - pixmap.width()/2, event->y()- pixmap.height()/2, pixmap);
    painter.end();

    setPixmap(m_pixmap);
    fitImage();
/*
    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
        close();
    else {
        show();
        setPixmap(pixmap);
    }
*/
}

void QDropArea::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    //qDebug("[%d,%d] [%d,%d]", event->oldSize().width(), event->oldSize().height(), event->size().width(), event->size().height());
    fitImage();
}

void QDropArea::loadImage(const QString &fileName, const char *format, Qt::ImageConversionFlags flags)
{
    m_pixmap = QPixmap(fileName, format, flags);
    fitImage();
}

void QDropArea::fitImage()
{   // TODO : reset a timer each time called in order to avoid resizing each time the size change when resizing the window
    if (!m_pixmap.isNull())
        setPixmap(m_pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
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


