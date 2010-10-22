#include "qdroparea.h"

#include <QtGui>

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
    event->acceptProposedAction();
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


