#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_lineEdit_phone_textChanged(const QString &arg1);

    void on_pushButton_sendCode_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

signals:
    void loginSuccess(const QString &token);

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
