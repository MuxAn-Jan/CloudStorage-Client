#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QListWidgetItem>
#include "Utils.h"
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    void doList(Data& data);//处理列表的数据包
    void doDownload(Data& data);//处理下载数据包
private slots:
    void on_pushButton_upload_clicked();
    
    void on_pushButton_browser_clicked();
    
    void on_pushButton_update_clicked();
    
    void onReadyRead();    
    void on_listWidget_file_itemDoubleClicked(QListWidgetItem *item);
    
private:
    Ui::MainWindow *ui;
    QTcpSocket* socket;
    QByteArray tempData;//读取数据的临时缓冲区
    QFile* file = nullptr;//下载文件的指针
    QString path;//下载路径
};
#endif // MAINWINDOW_H
