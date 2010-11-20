#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qdroparea.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent)
   :QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readSettings();

    m_dropArea = new QDropArea(this);
    this->setCentralWidget(m_dropArea);

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
    return QDragableLabel::Cercle;
}

void MainWindow::slotTagAdded(const QDragableLabel *)
{
    ui->numberSB->setValue(ui->numberSB->value() + 1);
}

void MainWindow::slotTagMoved(const QDragableLabel *)
{
}

