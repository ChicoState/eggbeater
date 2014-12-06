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
