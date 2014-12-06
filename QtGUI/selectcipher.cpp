#include "selectCipher.h"
#include "ui_selectCipher.h"

selectCipher::selectCipher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectCipher)
{
    ui->setupUi(this);
}

selectCipher::~selectCipher()
{
    delete ui;
}
QString selectCipher::getCipherMode()
{
    return cipherMode;
}
void selectCipher::resetRadioButtons()
{
    ui->CFB_Button->setAutoExclusive(false);
    ui->CFB_Button->setChecked(false);
    ui->OFB_Button->setAutoExclusive(false);
    ui->OFB_Button->setChecked(false);
    ui->GCM_Button->setAutoExclusive(false);
    ui->GCM_Button->setChecked(false);
}
void selectCipher::on_CFB_Button_clicked()
{
    cipherMode = "cfb";
}

void selectCipher::on_OFB_Button_clicked()
{
    cipherMode = "ofb";
}

void selectCipher::on_GCM_Button_clicked()
{
    cipherMode = "gcm";
}

void selectCipher::on_OK_Button_clicked()
{
    this->close();
}
