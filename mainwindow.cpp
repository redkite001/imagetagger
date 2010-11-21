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
       m_dropArea->loadImage(imagePath);
   }
}

void MainWindow::readSettings()
{
    QSettings settings;

    if (settings.contains("mainwindow/size"))
        resize(settings.value("mainwindow/size").toSize());

    if (settings.value("mainwindow/size", false).toBool())
        showFullScreen();
}

void MainWindow::writeSettings()
{
    QSettings settings;

    settings.setValue("mainwindow/size", size());
    settings.setValue("mainwindow/fullScreen", isFullScreen());
}

int MainWindow::getCurrentNumber()
{
    return ui->numberSB->value();
}

QDragableLabel::Shape MainWindow::getCurrentShape()
{
    return (QDragableLabel::Shape)ui->shapeCB->currentIndex();
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

