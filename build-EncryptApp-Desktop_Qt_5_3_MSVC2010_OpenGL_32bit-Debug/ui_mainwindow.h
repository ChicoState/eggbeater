/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *authentication;
    QLabel *board_detected;
    QTextBrowser *get_started;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(969, 685);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        QBrush brush3(QColor(110, 110, 110, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        QBrush brush4(QColor(120, 120, 120, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        MainWindow->setPalette(palette);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        authentication = new QPushButton(centralWidget);
        authentication->setObjectName(QStringLiteral("authentication"));
        authentication->setGeometry(QRect(290, 150, 361, 111));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        QBrush brush5(QColor(193, 193, 193, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush5);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Highlight, brush2);
        palette1.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Link, brush2);
        palette1.setBrush(QPalette::Active, QPalette::LinkVisited, brush2);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        palette1.setBrush(QPalette::Active, QPalette::NoRole, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Highlight, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Link, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::NoRole, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush2);
        QBrush brush6(QColor(51, 153, 255, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::Highlight, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Link, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::NoRole, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush2);
        authentication->setPalette(palette1);
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        authentication->setFont(font);
        board_detected = new QLabel(centralWidget);
        board_detected->setObjectName(QStringLiteral("board_detected"));
        board_detected->setGeometry(QRect(380, 60, 361, 20));
        board_detected->setFont(font);
        get_started = new QTextBrowser(centralWidget);
        get_started->setObjectName(QStringLiteral("get_started"));
        get_started->setGeometry(QRect(70, 361, 801, 241));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Base, brush5);
        QBrush brush7(QColor(246, 16, 70, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::AlternateBase, brush7);
        QBrush brush8(QColor(255, 0, 127, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::NoRole, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush5);
        palette2.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::NoRole, brush8);
        QBrush brush9(QColor(107, 107, 107, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush9);
        palette2.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::NoRole, brush8);
        get_started->setPalette(palette2);
        QFont font1;
        font1.setPointSize(14);
        get_started->setFont(font1);
        MainWindow->setCentralWidget(centralWidget);
        authentication->raise();
        get_started->raise();
        board_detected->raise();
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        authentication->setText(QApplication::translate("MainWindow", "Authentication", 0));
        board_detected->setText(QApplication::translate("MainWindow", "Board Detected", 0));
        get_started->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:14pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt;\">EggBeater</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt;\">To get started:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt;\">-1 Connect keypad</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0"
                        "px;\"><span style=\" font-size:16pt;\">-2 Follow instructions on keypad</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt;\">-3 Press [Authenticate]</span></p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
