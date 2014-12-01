#include "secdialog.h"
#include "ui_secdialog.h"
#include <QTextStream>
#include <iterator>

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
    else // When time is 0
    {
        QMessageBox::critical(this, tr("The title"), tr("Timeout!!"));
        this->close();

    }
}
void SecDialog::on_refresh_button_clicked()
{
    int min = 0;
    int sec = 0;
    
    // Refresh time to 10 mins
    startMillionseconds = 1000 * 60 *10;
    min = (startMillionseconds%(1000*60*60))/(1000*60);
    sec = ((startMillionseconds%(1000*60*60))%(1000*60))/1000;
    QTime time = QTime::QTime(0,min,sec);
    QString time_text = time.toString("hh : mm : ss");
    ui->Digital_clock->setText(time_text);
}

void SecDialog::on_choose_input_files_clicked()
{
    // Choose file(s) user want to encrypt/decrypt and display file name(s) on second window
    file_dlg = new FileDialog();
    int count = 0;

    if(file_dlg->exec())
    {
        fileNames = file_dlg->selectedFiles(); // Copy files user selected

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
                    // Display tow file names for each line
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
    
    // When user close the dialog without selecting files, reset
    else
    {
        fileNames = QStringList();
        ui->fileName->setText("File Name");
        return;
    }
}

void SecDialog::on_choose_output_folder_clicked()
{
    // Choose a destination folder and display folder name on second window
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
    // Check user select file(s) and a destination folder
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

    // If Both of file(s) and destinatind folder are not empty, go into it
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

                // Check session is open and divice is discovered
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

                // When discover is true and checkSession is ture, encrypt files user selected
                else
                {
                    invoke->encryptFiles(this, fileNames, folderName);
                    invoke->closeSession();
                }
            }
        }
    }


/****Start of progress bar update*******/
    //testing code for updating the progress bar
    //need to break into functions and integrate into "InvokeCLI"

        QString temp = QDir::tempPath()+"TempComm.cpp"; //"C:\Users\sam\AppData\Local\TempComm.cpp"; //System::GetTempPath();
        qint64 min=0;
        qint64 max=100;
        qint64 i=0;
        qint64 progress=0;
        qint64 curBlock=0;
        qint64 positionTrack=0;
        QString curFileCount=0;
        QString maxCount=0;
        ui->progressBar->setMaximum(max);
        ui->progressBar->setMinimum(min);
        QFile file(temp);
        if(!file.open(QFile::ReadOnly))
        {
            QMessageBox::warning(this, tr("Application"), QDir::currentPath()+file.fileName(), tr("ok")); //tr("Cant find temp file \n")
            return;
        }
        QTextStream in(&file);
        while(!in.atEnd())
        {
            QString line=in.readLine();
            QString::iterator it= line.begin();
            QVector<char> curFileName;
            if(it->isDigit()==true)
            {
                int linePos=0;
                for(it=line.begin(); linePos<line.length(); linePos++)
                {
                    if(*it== "\"")
                    {
                        i=0;
                        while(*it+1!="\"")
                        {
                            QByteArray temparray = (*it+1).toUtf8();
                            curFileName[i] = temparray.at(i);
                            i++;
                        }
                    }


                    switch(positionTrack)
                    {
                        case 0:
                            i=0;
                            while(it->isDigit()==true)
                            {
                                if(i=0)
                                    curFileCount=*it;
                                else
                                    curFileCount=curFileCount.append(*it);
                                i++;
                                it++;
                            }
                            positionTrack++;
                        case 1:
                            i=0;
                            if(it->isDigit()==false)
                                it++;
                            while(it->isDigit()==true)
                            {
                                if(i=0)
                                {
                                    maxCount=*it;
                                }
                                else
                                    maxCount=maxCount.append(*it);
                                i++;
                                it++;
                                positionTrack++;
                            }
                        case 2:
                            while(it->isDigit()==true)
                            {
                                it++;
                                positionTrack++;
                            }
                        case 3:
                            while(it->isDigit()==true)
                            {
                                it++;
                                positionTrack++;
                            }
                    }
                }
            }
        }
        file.close();
        qreal curC=curFileCount.toInt();
        qreal maxC=maxCount.toInt();
        progress=(curC/maxC)*100;

        //use message box to test values
        //QMessageBox::warning(this, sprogress, curFileCount, maxCount); //tr("Cant find temp file \n")
        QString sprogress=QString::number(progress);
        ui->progressBar->setValue(progress);

/****End of progressbar update --SamAdams*******/

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
    // Check user select file(s) and a destination folder
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

    // If Both of file(s) and destinatind folder are not empty, go into it
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
                
                //Check session is open and divice is discovered
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
                
                // When discover is true and checkSession is ture, decrypt files user selected
                else
                {
                    invoke->decryptFiles(this, fileNames, folderName, "cfb");
                    invoke->closeSession();
                }
            }
        }
    }
}
