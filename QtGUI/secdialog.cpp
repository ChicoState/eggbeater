#include "secdialog.h"
#include "ui_secdialog.h"

SecDialog::SecDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecDialog)
{
    ui->setupUi(this);

    startMillionseconds = (1000 * 60 * 10)+1000;
    countDown = new QTimer(this);   //Construct the timer
    countDown->setInterval(1000);   //One second interval
    countDown->setSingleShot(false);    //Multipul shot. This means that the signal timeout will be signed each second
    connect(countDown, SIGNAL(timeout()), this, SLOT(clock_time()));  //Connect the timeout signal to my slot timeOut
    countDown->start(); //Start the timer
}

SecDialog::~SecDialog()
{
    delete ui;
}

void SecDialog::clock_time()
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
    file_dlg = new FileDialog();
    int count = 0;

    if(file_dlg->exec())
    {
        fileNames = file_dlg->selectedFiles();

        if(!fileNames.isEmpty())
        {
            ui->fileName->setText("check");
            for(int i=0; i<fileNames.size(); i++)
            {
                QString fn = fileNames.at(i);
                QString temp = ui->fileName->text();

                if(temp=="File Name" || temp=="check")
                {
                    ui->fileName->setText(QFileInfo(fn).fileName());
                    count++;
                }

                else
                {
                    count++;
                    if((i%2)==1)
                    {
                        temp.append(", " + QFileInfo(fn).fileName());
                        ui->fileName->setText(temp);
                    }

                    else
                    {
                        temp.append("\n" + QFileInfo(fn).fileName());
                        ui->fileName->setText(temp);
                    }
                }

            }
        }
    }
    else
    {
        fileNames = QStringList();
        ui->fileName->setText("File Name");
        return;
    }
}

void SecDialog::on_choose_output_folder_clicked()
{
    folderName = QFileDialog::getExistingDirectory(
                this,
                tr("Open Directry"),
                "C/:",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                );

    if(!folderName.isEmpty())
        ui->folderName->setText(QFileInfo(folderName).fileName());
    else
        ui->folderName->setText("Folder Name");
}

void SecDialog::on_encrypt_clicked()
{
    if(!fileNames.isEmpty() && folderName.isEmpty())
    {
        QMessageBox::warning(this, tr("The title"), tr("Slect Destination Folder."));
        return;
    }

    else if(fileNames.isEmpty() && !folderName.isEmpty())
    {
        QMessageBox::warning(this, tr("The title"), tr("Slect File(s) or Folder(S)."));
        return;
    }

    else if(fileNames.isEmpty() && folderName.isEmpty())
    {
        QMessageBox::warning(this, tr("The title"), tr("Slect File(s) or Folder(s)\nand Destination Folder."));
        return;
    }

    else
    {
        for(int i=0; i<fileNames.size(); i++)
        {
            QFile file(fileNames.at(i));
            if(!file.open(QFile::ReadOnly|QFile::Text))
            {
                QMessageBox::warning(this, tr("Application"),
                                     tr("Cannot read file %1:\n%2")
                                     .arg(fileNames.at(i))
                                     .arg(file.errorString())
                                     );
            }

            else
            {
                bool discover = invoke->discoverDevice();
                bool checkSession = invoke->sessionIsOpen();
                if(discover==false && checkSession==false)
                {
                    QMessageBox::warning(this, tr("The title"), tr("Session is not opened\nand\nDevice is not connected!"));
                    return;
                }
                else if(discover==true && checkSession==false)
                {
                    QMessageBox::warning(this, tr("The title"), tr("Session is not opened!"));
                    return;
                }
                else if(discover==false && checkSession==true)
                {
                    QMessageBox::warning(this, tr("The title"), tr("Device is not connected!"));
                    return;
                }
                else
                {
                    invoke->startSession();
                    invoke->encryptFiles(fileNames, "CFB");
                    invoke->closeSession();
                }
            }
        }
    }

/*
    QProgressDialog *dlg = new QProgressDialog(this);
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

void SecDialog::on_decrypt_clicked()
{
    if(fileNames.isEmpty() && folderName.isEmpty())
    {
        QMessageBox::warning(this, tr("The title"), tr("Slect File(s) or Folder(s)\nand Destination Folder."));
        return;
    }

    else if(!fileNames.isEmpty() && folderName.isEmpty())
    {
        QMessageBox::warning(this, tr("The title"), tr("Slect Destination Folder."));
        return;
    }

    else if(fileNames.isEmpty() && !folderName.isEmpty())
    {
        QMessageBox::warning(this, tr("The title"), tr("Slect File(s) or Folder(S)."));
        return;
    }

    else
    {
        for(int i=0; i<fileNames.size(); i++)
        {
            QFile file(fileNames.at(i));
            if(!file.open(QFile::ReadOnly|QFile::Text))
            {
                QMessageBox::warning(this, tr("Application"),
                                     tr("Cannot read file %1:\n%2")
                                     .arg(fileNames.at(i))
                                     .arg(file.errorString())
                                     );
            }

            else
            {
                bool discover = invoke->discoverDevice();
                bool checkSession = invoke->sessionIsOpen();
                if(discover==false && checkSession==false)
                {
                    QMessageBox::warning(this, tr("The title"), tr("Session is not opened\nand\nDevice is not connected!"));
                    return;
                }
                else if(discover==true && checkSession==false)
                {
                    QMessageBox::warning(this, tr("The title"), tr("Session is not opened!"));
                    return;
                }
                else if(discover==false && checkSession==true)
                {
                    QMessageBox::warning(this, tr("The title"), tr("Device is not connected!"));
                    return;
                }
                else
                {
                    invoke->startSession();
                    invoke->decryptFiles(fileNames, "CFB");
                    invoke->closeSession();
                }
            }
        }
    }
}
