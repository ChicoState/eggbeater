#include "selectCipher.h"
#include "ui_selectCipher.h"

selectCipher::selectCipher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectCipher)
{
    i = 0;
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
void selectCipher::setI(int num)
{
    i = num;
}
int selectCipher::getI()
{
    return i;
}

void selectCipher::on_CFB_Button_clicked()
{
    cipherMode = "cfb";
    ui->OFB_Button->setAutoExclusive(false);
    ui->OFB_Button->setChecked(false);
    ui->GCM_Button->setAutoExclusive(false);
    ui->GCM_Button->setChecked(false);
}

void selectCipher::on_OFB_Button_clicked()
{
    cipherMode = "ofb";
    ui->CFB_Button->setAutoExclusive(false);
    ui->CFB_Button->setChecked(false);
    ui->GCM_Button->setAutoExclusive(false);
    ui->GCM_Button->setChecked(false);
}

void selectCipher::on_GCM_Button_clicked()
{
    cipherMode = "gcm";
    ui->CFB_Button->setAutoExclusive(false);
    ui->CFB_Button->setChecked(false);
    ui->OFB_Button->setAutoExclusive(false);
    ui->OFB_Button->setChecked(false);
}

void selectCipher::on_OK_Button_clicked()
{
    i++;
    this->close();
}
