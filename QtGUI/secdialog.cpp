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
    else
    {
        QMessageBox::critical(this, tr("The title"), tr("Timeout!!"));
        this->close();

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
    fileName = QFileDialog::getOpenFileName(
                this,
                tr("Choose a File You Want to Encrypt"),
                "C://",
                "All files(*.*);;"
                );
    if(!fileName.isEmpty())
        ui->fileName->setText(QFileInfo(fileName).fileName());
}

void SecDialog::on_choose_output_files_clicked()
{
    folderName = QFileDialog::getExistingDirectory(
                this,
                tr("Choose Destination Folder"),
                QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                    QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog
                );
                getOpenFileName(
                this,
                tr("Choose a File You Want to Encrypt"),
                "C://",
                "All files(*.*);;"
                );
    if(!fileName.isEmpty())
        ui->fileName->setText(QFileInfo(fileName).fileName());
}

void SecDialog::on_encrypt_clicked()
{
    QFile file1(fileName);
    QFile file2("Encrypted.txt");

    if(file1.open(QFile::ReadOnly) || !file2.open(QFile::WriteOnly))
        return;

   /* QProgressDialog *dlg = new QProgressDialog(this);
    qint64 len = src.bytesAvailable();
    dlg->setRange(0,len);
    dlg->show();

    char ch;
    while(!src.atEnd())
    {
      src.getChar(&ch);
      dst.putChar(ch);
      dlg->setValue(dlg->value()+1);
      qApp->processEvents();
    }*/
}
