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

    QFont                 getCurrentFont() const;
    int                   getCurrentNumber() const;
    QDragableLabel::Shape getCurrentShape() const;
    const QColor &        getCurrentFrontColor() const;
    const QColor &        getCurrentBackgroundColor() const;

protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_imagePathPB_clicked();
    void on_frontColorPB_clicked();
    void on_backgroundColorPB_clicked();
    void on_actionPrint_triggered();
    void on_actionLoadTags_triggered();
    void on_actionSaveTagsAs_triggered();
    void on_actionExport_triggered();
    void on_actionCopy_triggered();
    void slotTryToLoadPath(const QString &);
    void slotTagAdded(const QDragableLabel *);
    void slotTagMoved(const QDragableLabel *);

private:
    void readSettings();
    void writeSettings();

    Ui::MainWindow *ui;
    QDropArea *m_dropArea;
    QColor m_tagFront;
    QColor m_tagBackground;
    enum { MaxRecentFiles = 10 };
    QStringList m_recentFiles;
};

#endif // MAINWINDOW_H
