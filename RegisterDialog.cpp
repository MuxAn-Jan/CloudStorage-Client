#include "RegisterDialog.h"
#include "ui_RegisterDialog.h"
#include "Utils.h"
#include <QMessageBox>
#include <QDebug>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    socket = OnlySocket::getInstance()->getSocket();
    connect(socket, &QTcpSocket::readyRead, this, &RegisterDialog::onReadyRead);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e)
    qDebug()<<"register close";
    disconnect(socket, &QTcpSocket::readyRead, this, &RegisterDialog::onReadyRead);
}

void RegisterDialog::on_pushButton_register_clicked()
{
    QString id = ui->lineEdit_id->text();
    QString pwd = ui->lineEdit_pwd->text();
    QString rpwd = ui->lineEdit_rpwd->text();
    if(id.isEmpty() || pwd.isEmpty() || rpwd.isEmpty())
    {
        QMessageBox::warning(this, "警告", "账号密码不能为空");
        return;
    }
    
    if(pwd != rpwd)
    {
        QMessageBox::warning(this, "警告", "两次密码不一致");
        return;
    }
    
    Data data;//联合体对象
    data.data.type = registerType;//注册数据
    strcpy(data.data.name, id.toUtf8().data());
    strcpy(data.data.passwd, pwd.toUtf8().data());
    socket->write(data.transData, sizeof(data.transData));//发送数据
}

void RegisterDialog::onReadyRead()
{
    Data data;//联合体对象
    socket->read(data.transData, sizeof(data.transData));
    if(data.data.type == successType)//注册成功
    {
        QMessageBox::warning(this, "恭喜", "注册成功");
    }
    else
    {
        QMessageBox::warning(this, "不恭喜", "注册失败");
    }
}
