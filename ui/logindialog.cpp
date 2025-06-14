#include "logindialog.h"
#include "ui_logindialog.h"
#include "../core/apirequest.h"

#include <QRegularExpression>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle("登录");
    setFixedSize(this->width(), this->height());
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_lineEdit_phone_textChanged(const QString &arg1)
{
    static QRegularExpression re("^1[3-9]\\d{9}$");
    QRegularExpressionMatch match = re.match(arg1);

    ui->pushButton_sendCode->setEnabled(match.hasMatch());
}


void LoginDialog::on_pushButton_sendCode_clicked()
{
    QString phone = ui->lineEdit_phone->text();
    int code = getAuthCode(phone);
    if(code <= 0){
        QMessageBox::critical(this, "失败", "发送失败，请检查网络");
        return;
    }
    ui->pushButton_sendCode->setEnabled(false);
}


void LoginDialog::on_pushButton_cancel_clicked()
{
    this->close();
}


void LoginDialog::on_pushButton_ok_clicked()
{
    QString phone = ui->lineEdit_phone->text().trimmed();
    QString code = ui->lineEdit_code->text().trimmed();

    if(phone.isEmpty() || code.isEmpty()){
        QMessageBox::warning(this, "警告", "手机或验证码为空");
        return;
    }

    QString token = getAuthToken(phone, code);
    if(token==""){
        QMessageBox::critical(this, "失败", "登录失败");
        return;
    }

    emit loginSuccess(token);
    this->accept();
}

