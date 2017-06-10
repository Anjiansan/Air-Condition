#include "mainwindow.h"
#include <QApplication>
#include "login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login login;
    MainWindow w(0,login.client);
    if(login.exec()==QDialog::Accepted)
    {
        w.show();
        return a.exec();
    }
    else
        return 0;
}
