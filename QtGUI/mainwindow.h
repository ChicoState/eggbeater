#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "secdialog.h"
#include "invokeCLI.h"

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_authentication_clicked();

private:
    Ui::MainWindow *ui;
    EggBeater::InvokeCLI invoke;
};

#endif // MAINWINDOW_H
