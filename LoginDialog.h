#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "OnlySocket.h"

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
    void on_pushButton_login_clicked();
    
    void on_pushButton_register_clicked();
    void onReadyRead();
    void onConnected();
private:
    Ui::LoginDialog *ui;
    QTcpSocket* socket;
};

#endif // LOGINDIALOG_H
