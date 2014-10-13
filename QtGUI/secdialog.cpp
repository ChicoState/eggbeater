#include "secdialog.h"
#include "ui_secdialog.h"

SecDialog::SecDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecDialog)
{
    ui->setupUi(this);

    startMillionseconds = (1000 * 60 *10)+1000;
    countDown = new QTimer(this);   //Construct the timer
    countDown->setInterval(1000);   //One second interval
    countDown->setSingleShot(false);    //Multipul shot. This means that the signal timeout will be signed each second
    connect(countDown, SIGNAL(timeout()), this, SLOT(timeOut()));  //Connect the timeout signal to my slot timeOut
    countDown->start(); //Start the timer
}

SecDialog::~SecDialog()
{
    delete ui;
}

void SecDialog::timeOut()
{
    int min=0;
    int sec=0;

    if(startMillionseconds-1000>=0)
    {
        startMillionseconds -= 1000;
        min = (startMillionseconds%(1000*60*60))/(1000*60);
        sec = ((startMillionseconds%(1000*60*60))%(1000*60))/1000;
        QTime time = QTime::QTime(0,min,sec);
        QString time_text = time.toString("hh : mm : ss");
        ui->Digital_clock->setText(time_text);
    }
}

void SecDialog::on_refresh_button_clicked()
{
    int min = 0;
    int sec = 0;

    startMillionseconds = 1000 * 60 *10;
    min = (startMillionseconds%(1000*60*60))/(1000*60);
    sec = ((startMillionseconds%(1000*60*60))%(1000*60))/1000;
    QTime time = QTime::QTime(0,min,sec);
    QString time_text = time.toString("hh : mm : ss");
    ui->Digital_clock->setText(time_text);
}

void SecDialog::on_choose_input_files_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(
                this,
                tr("Choose Folder You Want to Encrypt"),
                QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                    QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog
                );
}

void SecDialog::on_choose_output_files_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(
                this,
                tr("Choose Destination Folder"),
                QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                    QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog
                );
}
