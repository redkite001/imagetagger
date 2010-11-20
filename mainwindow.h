#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    static int getCurrentType();
    static int getCurrentNumber();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_imagePathPB_clicked();

private:
    void readSettings();
    void writeSettings();

    Ui::MainWindow *ui;
    QDropArea *m_dropArea;
};

#endif // MAINWINDOW_H
