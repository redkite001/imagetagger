#ifndef EDITTAGDIALOG_H
#define EDITTAGDIALOG_H

#include <QColor>
#include <QDialog>

namespace Ui {
    class EditTagDialog;
}

class QDragableLabel;

class EditTagDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTagDialog(QDragableLabel &tag, QWidget *parent = 0);
    ~EditTagDialog();

public slots:
    void on_buttonBox_accepted();
    void on_frontColorPB_clicked();
    void on_backgroundColorPB_clicked();

private:
    Ui::EditTagDialog *ui;

    QDragableLabel &m_tag;
    QColor m_tagFront;
    QColor m_tagBackground;
};

#endif // EDITTAGDIALOG_H
