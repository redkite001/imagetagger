#include "edittagdialog.h"
#include "ui_edittagdialog.h"

EditTagDialog::EditTagDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTagDialog)
{
    ui->setupUi(this);
}

EditTagDialog::~EditTagDialog()
{
    delete ui;
}
