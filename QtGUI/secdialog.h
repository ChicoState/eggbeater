#ifndef SECDIALOG_H
#define SECDIALOG_H

#include <QDialog>
#include <QTime>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>

namespace Ui 
{
  class SecDialog;
}

class SecDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SecDialog(QWidget *parent = 0);
    ~SecDialog();

    QTimer *countDown;
    int startMillionseconds;

protected slots:
    void timeOut();

private slots:
    void on_refresh_button_clicked();
    void on_choose_input_files_clicked();
    void on_choose_output_files_clicked();

private:
    Ui::SecDialog *ui;
};

#endif // SECDIALOG_H
