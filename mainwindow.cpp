#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "core/datacontrol.h"
#include "pojo/user.h"
#include "core/checkcore.h"
#include "ui/logindialog.h"
#include "core/apirequest.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    ui->infoOuter->append("Init...");
    DataControl dataCtrl;
    if(dataCtrl.initConfig()){
        ui->infoOuter->append("初始化成功！");
    }else{
        ui->infoOuter->append("失败！");
    }
    AuthConfig config = AuthConfig::getInstance();
    for(int i=0; i<config.getUsers().size(); i++){
        User it = config.getUsers()[i];
        addStudentRow(it.getStuId(), it.getName(), it.getAuth());
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addStudentRow(const QString &studentId, const QString &name, const QString &auth) {
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(studentId));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(name));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(auth));
}

void MainWindow::removeStudentById(const QString &studentId)
{
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem *item = ui->tableWidget->item(row, 0); // 学号列
        if (item && item->text() == studentId) {
            ui->tableWidget->removeRow(row);
            return;
        }
    }

    // 可选提示
    // QMessageBox::information(this, "提示", QString("未找到学号为 %1 的学生").arg(studentId));
}

void MainWindow::on_btnAllCheck_clicked()
{
    QString res = CheckAll();
    ui->infoOuter->append(res);
}


void MainWindow::on_btnAddAuth_clicked()
{
    LoginDialog l;
    connect(&l, &LoginDialog::loginSuccess, this, &MainWindow::handleLoginSuccess);
    l.exec();
}

void MainWindow::handleLoginSuccess(const QString &token){
    addStudentRow("", "", token);
}

