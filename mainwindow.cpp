#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qdroparea.h"
#include <QColorDialog>
#include <QClipboard>
#include <QFileDialog>
#include <QLineEdit>
#include <QPrintDialog>
#include <QPrinter>
#include <QSettings>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_tagFront(Qt::red)
    , m_tagBackground(QColor(0, 0, 0, 0))
{
    ui->setupUi(this);
    readSettings();

    m_dropArea = new QDropArea();
    ui->scrollArea->setWidget(m_dropArea);
    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidgetResizable(false);
    ui->scrollArea->setAlignment(Qt::AlignCenter);

    connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->imagePathCB, SIGNAL(activated(QString)), this, SLOT(slotTryToLoadPath(QString)));
    connect(m_dropArea, SIGNAL(tagAdded(const QDragableLabel *)), this, SLOT(slotTagAdded(const QDragableLabel *)));
    connect(m_dropArea, SIGNAL(tagMoved(const QDragableLabel *)), this, SLOT(slotTagMoved(const QDragableLabel *)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    writeSettings();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    //m_dropArea->fitImage();
}

void MainWindow::on_imagePathPB_clicked()
{
   QString imagePath = QFileDialog::getOpenFileName(this, trUtf8("Load an image"), QDir::homePath(), trUtf8("Images (*.jpg *.jpeg *.png)"));

   if (!imagePath.isEmpty()) {
       ui->imagePathCB->setEditText(imagePath);
       slotTryToLoadPath(imagePath);
   }
}

void MainWindow::readSettings()
{
    QSettings settings;

    if (settings.contains("mainwindow/geometry"))
        restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    if (settings.contains("mainwindow/state"))
        restoreState(settings.value("mainwindow/state").toByteArray());
    if (settings.contains("mainwindow/lastFiles")) {
        ui->imagePathCB->addItems(settings.value("mainwindow/lastFiles").toStringList());
        ui->imagePathCB->setCurrentIndex(-1);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.setValue("mainwindow/geometry", saveGeometry());
    settings.setValue("mainwindow/state", saveState());
    QStringList lastFiles;
    for (int i = 0; i < 10 && i < ui->imagePathCB->count(); ++i)
        lastFiles << ui->imagePathCB->itemText(i);
    settings.setValue("mainwindow/lastFiles", lastFiles);
}

QFont MainWindow::getCurrentFont() const
{
    return ui->fontCB->currentFont();
}

int MainWindow::getCurrentNumber() const
{
    return ui->numberSB->value();
}

QDragableLabel::Shape MainWindow::getCurrentShape() const
{
    return (QDragableLabel::Shape)ui->shapeCB->currentIndex();
}

const QColor & MainWindow::getCurrentFrontColor() const
{
    return m_tagFront;
}

const QColor & MainWindow::getCurrentBackgroundColor() const
{
    return m_tagBackground;
}

void MainWindow::slotTryToLoadPath(const QString &_path)
{
    int index = ui->imagePathCB->findText(_path);
    if (index >= 0) ui->imagePathCB->removeItem(index);

    if (QFile::exists(_path)) {
        m_dropArea->clear();
        m_dropArea->loadImage(_path);
        ui->imagePathCB->insertItem(0, _path);
        ui->imagePathCB->setCurrentIndex(0);
        ui->numberSB->setValue(1);
    }
}

void MainWindow::slotTagAdded(const QDragableLabel *)
{
    ui->numberSB->setValue(ui->numberSB->value() + 1);
}

void MainWindow::slotTagMoved(const QDragableLabel *)
{
}

void MainWindow::on_frontColorPB_clicked()
{
    m_tagFront = QColorDialog::getColor(m_tagFront, this, trUtf8("Choose a text color"));
}

void MainWindow::on_backgroundColorPB_clicked()
{
    QColor init(qRgba(0, 0, 0, 0));
    if (m_tagBackground.isValid())
        init = m_tagBackground;
    m_tagBackground = QColorDialog::getColor(m_tagBackground, this, trUtf8("Choose a background color"), QColorDialog::ShowAlphaChannel);
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        // print ...
    }
}

void MainWindow::on_actionLoadTags_triggered()
{
    // FIXIT: Find a better way for checking that an image is loaded
    if (ui->imagePathCB->currentText().isEmpty())
        return;

    QString tagsPath = QFileDialog::getOpenFileName(this, trUtf8("Load a tag file"), QDir::homePath(), trUtf8("Text (*.txt)"));

    if (!tagsPath.isEmpty()) {
        m_dropArea->loadTags(tagsPath); // FIXIT: Check the structure of the file?
    }
}

void MainWindow::on_actionSaveTagsAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, trUtf8("Save file as ..."), QDir::homePath(), trUtf8("Text (*.txt)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);

        QList<QDragableLabel *>tagList = m_dropArea->getTagList();
        // Write the data
        for (int i = 0; i < tagList.count();++i)
            out << *tagList[i];
    }
}

void MainWindow::on_actionExport_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, trUtf8("Save file as ..."), QDir::homePath(), trUtf8("Images (*.png *.jpg *.jpeg)"));

    if (!fileName.isEmpty()) {
        QPixmap pixmap = QPixmap::grabWidget(m_dropArea);
        pixmap.save(fileName, "PNG"); // writes pixmap into bytes in PNG format
    }
}

void MainWindow::on_actionCopy_triggered()
{
    qApp->clipboard()->setPixmap(QPixmap::grabWidget(m_dropArea));
}





