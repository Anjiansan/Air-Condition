#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMessageBox>

#define ACCOUNT "admin"
#define PWD "admin"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void on_loginBtn_clicked();

    void on_cancleBtn_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
