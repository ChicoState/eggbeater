#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_authentication_clicked()
{
    // If user could get Authentication, go to second window
    SecDialog secdialog;
    secdialog.setModal(true);
    secdialog.exec();
}
