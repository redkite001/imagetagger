#include "edittagdialog.h"
#include "ui_edittagdialog.h"

#include "qdragablelabel.h"

#include <QColorDialog>

EditTagDialog::EditTagDialog(QDragableLabel &tag, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditTagDialog)
    , m_tag(tag)
{
    ui->setupUi(this);

    ui->fontCB->setCurrentFont(m_tag.getFont());
    m_tagFront = m_tag.getFrontColor();
    m_tagBackground = m_tag.getBackgroundColor();
    ui->numberSB->setValue(m_tag.getNumber());
    ui->shapeCB->setCurrentIndex((int)m_tag.getShape());
}

EditTagDialog::~EditTagDialog()
{
    delete ui;
}

void EditTagDialog::on_buttonBox_accepted()
{
    m_tag.setFont(ui->fontCB->currentFont());
    m_tag.setFrontColor(m_tagFront);
    m_tag.setBackgroundColor(m_tagBackground);
    m_tag.setNumber(ui->numberSB->value());
    m_tag.setShape((QDragableLabel::Shape)ui->shapeCB->currentIndex());
}

void EditTagDialog::on_frontColorPB_clicked()
{
    m_tagFront = QColorDialog::getColor(m_tagFront, this, trUtf8("Choose a text color"));
}

void EditTagDialog::on_backgroundColorPB_clicked()
{
    QColor init(qRgba(0, 0, 0, 0));
    if (m_tagBackground.isValid())
        init = m_tagBackground;
    m_tagBackground = QColorDialog::getColor(m_tagBackground, this, trUtf8("Choose a background color"), QColorDialog::ShowAlphaChannel);
}

