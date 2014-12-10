/********************************************************************************
** Form generated from reading UI file 'secdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SECDIALOG_H
#define UI_SECDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SecDialog
{
public:
    QPushButton *refresh_button;
    QLabel *time_untill_lockout;
    QLabel *input_files;
    QLabel *output_files;
    QLabel *Digital_clock;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QPushButton *encrypt;
    QLabel *pending_inputs;
    QProgressBar *progressBar;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout_4;
    QPushButton *decrypt;
    QLabel *pending_outputs;
    QProgressBar *progressBar_2;
    QPushButton *choose_input_files;
    QLabel *fileName;
    QLabel *folderName;
    QPushButton *choose_output_folder;

    void setupUi(QDialog *SecDialog)
    {
        if (SecDialog->objectName().isEmpty())
            SecDialog->setObjectName(QStringLiteral("SecDialog"));
        SecDialog->resize(953, 642);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(110, 110, 110, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        SecDialog->setPalette(palette);
        refresh_button = new QPushButton(SecDialog);
        refresh_button->setObjectName(QStringLiteral("refresh_button"));
        refresh_button->setGeometry(QRect(691, 33, 191, 41));
        QPalette palette1;
        QBrush brush2(QColor(255, 0, 0, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        QBrush brush3(QColor(120, 120, 120, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush3);
        refresh_button->setPalette(palette1);
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        refresh_button->setFont(font);
        time_untill_lockout = new QLabel(SecDialog);
        time_untill_lockout->setObjectName(QStringLiteral("time_untill_lockout"));
        time_untill_lockout->setGeometry(QRect(70, 20, 291, 61));
        QPalette palette2;
        QBrush brush4(QColor(0, 0, 0, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        time_untill_lockout->setPalette(palette2);
        QFont font1;
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setWeight(75);
        time_untill_lockout->setFont(font1);
        input_files = new QLabel(SecDialog);
        input_files->setObjectName(QStringLiteral("input_files"));
        input_files->setGeometry(QRect(60, 280, 371, 71));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        input_files->setPalette(palette3);
        input_files->setFont(font1);
        output_files = new QLabel(SecDialog);
        output_files->setObjectName(QStringLiteral("output_files"));
        output_files->setGeometry(QRect(570, 280, 331, 31));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush3);
        output_files->setPalette(palette4);
        output_files->setFont(font1);
        Digital_clock = new QLabel(SecDialog);
        Digital_clock->setObjectName(QStringLiteral("Digital_clock"));
        Digital_clock->setGeometry(QRect(390, 20, 211, 61));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        Digital_clock->setPalette(palette5);
        QFont font2;
        font2.setPointSize(16);
        Digital_clock->setFont(font2);
        layoutWidget = new QWidget(SecDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(60, 120, 331, 141));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        encrypt = new QPushButton(layoutWidget);
        encrypt->setObjectName(QStringLiteral("encrypt"));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush3);
        encrypt->setPalette(palette6);
        QFont font3;
        font3.setPointSize(16);
        font3.setBold(true);
        font3.setWeight(75);
        encrypt->setFont(font3);

        verticalLayout_3->addWidget(encrypt);

        pending_inputs = new QLabel(layoutWidget);
        pending_inputs->setObjectName(QStringLiteral("pending_inputs"));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        pending_inputs->setPalette(palette7);
        pending_inputs->setFont(font1);

        verticalLayout_3->addWidget(pending_inputs);

        progressBar = new QProgressBar(layoutWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        verticalLayout_3->addWidget(progressBar);

        layoutWidget1 = new QWidget(SecDialog);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(570, 110, 321, 151));
        verticalLayout_4 = new QVBoxLayout(layoutWidget1);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        decrypt = new QPushButton(layoutWidget1);
        decrypt->setObjectName(QStringLiteral("decrypt"));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::ButtonText, brush3);
        decrypt->setPalette(palette8);
        decrypt->setFont(font3);

        verticalLayout_4->addWidget(decrypt);

        pending_outputs = new QLabel(layoutWidget1);
        pending_outputs->setObjectName(QStringLiteral("pending_outputs"));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush4);
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush4);
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        pending_outputs->setPalette(palette9);
        pending_outputs->setFont(font1);

        verticalLayout_4->addWidget(pending_outputs);

        progressBar_2 = new QProgressBar(layoutWidget1);
        progressBar_2->setObjectName(QStringLiteral("progressBar_2"));
        progressBar_2->setValue(24);

        verticalLayout_4->addWidget(progressBar_2);

        choose_input_files = new QPushButton(SecDialog);
        choose_input_files->setObjectName(QStringLiteral("choose_input_files"));
        choose_input_files->setGeometry(QRect(58, 540, 331, 45));
        QFont font4;
        font4.setPointSize(14);
        choose_input_files->setFont(font4);
        fileName = new QLabel(SecDialog);
        fileName->setObjectName(QStringLiteral("fileName"));
        fileName->setGeometry(QRect(60, 350, 521, 191));
        QFont font5;
        font5.setPointSize(10);
        font5.setBold(true);
        font5.setWeight(75);
        fileName->setFont(font5);
        folderName = new QLabel(SecDialog);
        folderName->setObjectName(QStringLiteral("folderName"));
        folderName->setGeometry(QRect(580, 310, 321, 231));
        folderName->setFont(font5);
        choose_output_folder = new QPushButton(SecDialog);
        choose_output_folder->setObjectName(QStringLiteral("choose_output_folder"));
        choose_output_folder->setGeometry(QRect(580, 540, 321, 45));
        choose_output_folder->setFont(font4);

        retranslateUi(SecDialog);

        QMetaObject::connectSlotsByName(SecDialog);
    } // setupUi

    void retranslateUi(QDialog *SecDialog)
    {
        SecDialog->setWindowTitle(QApplication::translate("SecDialog", "Dialog", 0));
        refresh_button->setText(QApplication::translate("SecDialog", "Refresh", 0));
        time_untill_lockout->setText(QApplication::translate("SecDialog", "Time untill lockout:", 0));
        input_files->setText(QApplication::translate("SecDialog", "Input File(s) or Folder(s):\n"
"MAX 10 Files", 0));
        output_files->setText(QApplication::translate("SecDialog", "Destination Folder:", 0));
        Digital_clock->setText(QApplication::translate("SecDialog", "00 : 10 : 00", 0));
        encrypt->setText(QApplication::translate("SecDialog", "Encrypt", 0));
        pending_inputs->setText(QApplication::translate("SecDialog", "Pending Inputs:", 0));
        decrypt->setText(QApplication::translate("SecDialog", "Decrypt", 0));
        pending_outputs->setText(QApplication::translate("SecDialog", "Pending Outputs:", 0));
        choose_input_files->setText(QApplication::translate("SecDialog", "Select", 0));
        fileName->setText(QApplication::translate("SecDialog", "File Name", 0));
        folderName->setText(QApplication::translate("SecDialog", "Folder Name", 0));
        choose_output_folder->setText(QApplication::translate("SecDialog", "Select", 0));
    } // retranslateUi

};

namespace Ui {
    class SecDialog: public Ui_SecDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SECDIALOG_H
