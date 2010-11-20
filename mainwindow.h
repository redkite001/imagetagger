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

    int                   getCurrentNumber();
    QDragableLabel::Shape getCurrentShape();
    const QColor &        getCurrentFrontColor();
    const QColor &        getCurrentBackgroundColor();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_imagePathPB_clicked();
    void on_frontColorPB_clicked();
    void on_backgroundColorPB_clicked();
    void slotTagAdded(const QDragableLabel *);
    void slotTagMoved(const QDragableLabel *);

private:
    void readSettings();
    void writeSettings();

    Ui::MainWindow *ui;
    QDropArea *m_dropArea;
    QColor m_tagFront;
    QColor m_tagBackground;
};

#endif // MAINWINDOW_H
