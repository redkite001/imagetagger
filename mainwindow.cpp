#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qdroparea.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QLineEdit>
#include <QSettings>


MainWindow::MainWindow(QWidget *parent)
   :QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_tagFront(Qt::red),
    m_tagBackground(QColor())
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
       ui->imagePathCB->lineEdit()->setText(imagePath);
       slotTryToLoadPath(imagePath);
   }
}

void MainWindow::readSettings()
{
    QSettings settings;

    if (settings.contains("mainwindow/size"))
        resize(settings.value("mainwindow/size").toSize());
    if (settings.value("mainwindow/size", false).toBool())
        showFullScreen();
    if (settings.contains("mainwindow/lastFiles")) {
        ui->imagePathCB->addItems(settings.value("mainwindow/lastFiles").toStringList());
        ui->imagePathCB->setCurrentIndex(-1);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.setValue("mainwindow/size", size());
    settings.setValue("mainwindow/fullScreen", isFullScreen());
    QStringList lastFiles;
    for (int i = 0; i < 10 && i < ui->imagePathCB->count(); ++i)
        lastFiles << ui->imagePathCB->itemText(i);
    settings.setValue("mainwindow/lastFiles", lastFiles);
}

int MainWindow::getCurrentNumber()
{
    return ui->numberSB->value();
}

QDragableLabel::Shape MainWindow::getCurrentShape()
{
    return (QDragableLabel::Shape)ui->shapeCB->currentIndex();
}

void MainWindow::slotTryToLoadPath(const QString &_path)
{
    int index = ui->imagePathCB->findText(_path);
    if (index >= 0) ui->imagePathCB->removeItem(index);

    if (QFile::exists(_path)) {
        m_dropArea->loadImage(_path);
        ui->imagePathCB->insertItem(0, _path);
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
    m_tagFront = QColorDialog::getColor(m_tagFront, this, tr("Choose a text color"));
}

void MainWindow::on_backgroundColorPB_clicked()
{
    QColor init(qRgba(0, 0, 0, 0));
    if (m_tagBackground.isValid())
        init = m_tagBackground;
    m_tagBackground = QColorDialog::getColor(m_tagBackground, this, tr("Choose a background color"));
}

const QColor & MainWindow::getCurrentFrontColor()
{
    return m_tagFront;
}

const QColor & MainWindow::getCurrentBackgroundColor()
{
    return m_tagBackground;
}

