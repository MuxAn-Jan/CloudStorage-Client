#include "LoginDialog.h"
#include "ui_LoginDialog.h"
#include "Utils.h"
#include "RegisterDialog.h"
#include <QHostAddress>
#include <QDebug>
#include "MainWindow.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    socket = OnlySocket::getInstance()->getSocket();
    connect(socket, &QTcpSocket::readyRead, this, &LoginDialog::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &LoginDialog::onConnected);
    //连接服务器
    socket->connectToHost(QHostAddress("192.168.31.180"), 6688);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButton_login_clicked()
{
    QString id = ui->lineEdit_id->text();
    QString pwd = ui->lineEdit_pwd->text();
    Data data;
    data.data.type = loginType;
    strcpy(data.data.name, id.toUtf8().data());
    strcpy(data.data.passwd, pwd.toUtf8().data());
    socket->write(data.transData, sizeof(data.transData));
    
}

void LoginDialog::on_pushButton_register_clicked()
{
    //断开登录界面的readyRead信号与槽的连接
    disconnect(socket, &QTcpSocket::readyRead, this, &LoginDialog::onReadyRead);
    RegisterDialog* rd = new RegisterDialog(this);
    rd->exec();//使用exec函数显示界面，会产生一个阻塞，直到关闭注册界面才向下继续执行
    //当关闭注册界面的时候，重新连接readyRead信号与槽
    connect(socket, &QTcpSocket::readyRead, this, &LoginDialog::onReadyRead);
}

void LoginDialog::onReadyRead()
{
    Data data;
    socket->read(data.transData, sizeof(data.transData));
    if(data.data.type == successType)//登录成功
    {
        //在切换界面时需要断开登录界面的readyRead连接
        disconnect(socket, &QTcpSocket::readyRead, this, &LoginDialog::onReadyRead);
        OnlySocket::getInstance()->setId(data.data.name);
        MainWindow* mw = new MainWindow;
        mw->show();
        close();
    }
    else
    {
        QMessageBox::warning(this, "警告", "登录失败");
    }
}

void LoginDialog::onConnected()
{
    ui->pushButton_login->setEnabled(true);
    ui->pushButton_register->setEnabled(true);
    ui->lineEdit_id->setEnabled(true);
    ui->lineEdit_pwd->setEnabled(true);
}
