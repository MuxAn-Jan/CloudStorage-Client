#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "OnlySocket.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();
    
private:
    //当关闭界面的时候closeEvent会自动执行
    void closeEvent(QCloseEvent * e) override;
private slots:
    void on_pushButton_register_clicked();
    void onReadyRead();
private:
    Ui::RegisterDialog *ui;
    QTcpSocket* socket;
};

#endif // REGISTERDIALOG_H
