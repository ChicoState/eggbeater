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
    if (invoke.startSession(this, NULL))
    {

        // If user could get Authentication, go to second window
        SecDialog secdialog(&invoke);
        secdialog.setModal(true);
        secdialog.setWindowTitle("EncryptApp");
        secdialog.exec();
    }
    else
    {
        QMessageBox::critical(this, "Authentication", "Could not create an authenticated session", "Ok");
    }
}
