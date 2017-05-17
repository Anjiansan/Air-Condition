#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include "clientsocket.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent,ClientSocket *client);
    ~MainWindow();

private slots:
    void on_exitBtn_clicked();

    void on_reqStopBtn_clicked();

    void on_reqResumeBtn_clicked();

    void on_reqUpdateBnt_clicked();

    void on_rptStateBtm_clicked();

private:
    Ui::MainWindow *ui;

    ClientSocket *client;
};

#endif // MAINWINDOW_H
