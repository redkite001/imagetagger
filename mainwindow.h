#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qdragablelabel.h"

class QDropArea;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int getCurrentNumber();
    QDragableLabel::Shape getCurrentShape();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_imagePathPB_clicked();
    void slotTagAdded(const QDragableLabel *);
    void slotTagMoved(const QDragableLabel *);

private:
    void readSettings();
    void writeSettings();

    Ui::MainWindow *ui;
    QDropArea *m_dropArea;
};

#endif // MAINWINDOW_H
