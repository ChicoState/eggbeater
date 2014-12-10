/********************************************************************************
** Form generated from reading UI file 'selectCipher.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTCIPHER_H
#define UI_SELECTCIPHER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_selectCipher
{
public:
    QPushButton *OK_Button;
    QLabel *label;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QRadioButton *CFB_Button;
    QRadioButton *OFB_Button;
    QRadioButton *GCM_Button;

    void setupUi(QDialog *selectCipher)
    {
        if (selectCipher->objectName().isEmpty())
            selectCipher->setObjectName(QStringLiteral("selectCipher"));
        selectCipher->resize(320, 240);
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setItalic(true);
        font.setWeight(75);
        selectCipher->setFont(font);
        OK_Button = new QPushButton(selectCipher);
        OK_Button->setObjectName(QStringLiteral("OK_Button"));
        OK_Button->setGeometry(QRect(60, 190, 171, 34));
        QFont font1;
        font1.setPointSize(8);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        OK_Button->setFont(font1);
        label = new QLabel(selectCipher);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 10, 251, 31));
        widget = new QWidget(selectCipher);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(80, 50, 141, 121));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        CFB_Button = new QRadioButton(widget);
        CFB_Button->setObjectName(QStringLiteral("CFB_Button"));
        CFB_Button->setFont(font);

        verticalLayout->addWidget(CFB_Button);

        OFB_Button = new QRadioButton(widget);
        OFB_Button->setObjectName(QStringLiteral("OFB_Button"));
        OFB_Button->setFont(font);

        verticalLayout->addWidget(OFB_Button);

        GCM_Button = new QRadioButton(widget);
        GCM_Button->setObjectName(QStringLiteral("GCM_Button"));
        GCM_Button->setFont(font);

        verticalLayout->addWidget(GCM_Button);


        retranslateUi(selectCipher);

        QMetaObject::connectSlotsByName(selectCipher);
    } // setupUi

    void retranslateUi(QDialog *selectCipher)
    {
        selectCipher->setWindowTitle(QApplication::translate("selectCipher", "Dialog", 0));
        OK_Button->setText(QApplication::translate("selectCipher", "OK", 0));
        label->setText(QApplication::translate("selectCipher", "Select Cipher Mode", 0));
        CFB_Button->setText(QApplication::translate("selectCipher", "CFB", 0));
        OFB_Button->setText(QApplication::translate("selectCipher", "OFB", 0));
        GCM_Button->setText(QApplication::translate("selectCipher", "GCM", 0));
    } // retranslateUi

};

namespace Ui {
    class selectCipher: public Ui_selectCipher {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTCIPHER_H
