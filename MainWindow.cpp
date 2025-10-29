#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include "Utils.h"
#include "OnlySocket.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //获得socket实例
    socket = OnlySocket::getInstance()->getSocket();
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    on_pushButton_update_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doList(Data &data)
{
    //我们将文件列表中的文件名都放在数据包的data数组中，以字符串形式存放，文件名之间以逗号分隔
    QString fileNames = QString::fromUtf8(data.data.data);//将列表数据转换成字符串
    QStringList sl = fileNames.split(",");//根据逗号拆分文件名
    for(auto item : sl)
    {
        if(item=="."||item==".."||item=="")
            continue;
        ui->listWidget_file->addItem(item);//显示所有文件名
    }
}

void MainWindow::doDownload(Data &data)
{
    //接收上传文件的第一包数据
    if(file == nullptr)
    {
        //创建文件对象
        file = new QFile(path+"/"+QString(data.data.fileName));
        //打开文件
        file->open(QIODevice::WriteOnly);
    }
    //接收上传文件的最后一包数据
    if(data.data.dataSize == -1)
    {
        file->close();//关闭文件
        delete file;
        file = nullptr;
        return ;
    }
    file->write(data.data.data, data.data.dataSize);//向文件中写入数据
}

void MainWindow::on_pushButton_upload_clicked()
{
    QFile file(ui->lineEdit_path->text());
    QStringList sl = ui->lineEdit_path->text().split("/");//拆分文件路径，获得文件名
    QString fileName = sl.last();//文件名
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "警告", "上传的文件可能不存在");
        return ;
    }
    Data data;
    strcpy(data.data.fileName, fileName.toUtf8().data());//写入文件名
    OnlySocket::getInstance()->getId(data.data.name);//写入用户的id
    while(!file.atEnd())
    {
        data.data.type = uploadType;//上传数据类型
        qint64 size = file.read(data.data.data, sizeof(data.data.data));//读取文件数据
        data.data.dataSize = size;//数据长度
        socket->write(data.transData, sizeof(data.transData));
        socket->waitForBytesWritten();//等待数据完全写入，阻塞函数
    }
    data.data.dataSize = -1;//文件上传完毕
    socket->write(data.transData, sizeof(data.transData));
    socket->waitForBytesWritten();
    file.close();
}

void MainWindow::on_pushButton_browser_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "选择上传文件", ".");
    ui->lineEdit_path->setText(path);
}

void MainWindow::on_pushButton_update_clicked()
{
    ui->listWidget_file->clear();//清空原有的列表显示
    Data data;
    data.data.type = listType;//获得列表的数据包类型
    OnlySocket::getInstance()->getId(data.data.name);
    socket->write(data.transData, sizeof(data.transData));
    socket->waitForBytesWritten();
}

void MainWindow::onReadyRead()
{
    Data data;//联合体对象
    int readSize;
    //读取数据 ， 每次执行onReadyRead 可能不只有一包数据
    while((readSize = socket->read(data.transData, sizeof(data.transData)-tempData.size()))>0)
    {   
        //如果发现读取的数据小于数据包，说明这包数据被打断了，数据包需要拼接
        if(readSize < sizeof(data.transData))
        {
            tempData.append(data.transData, readSize);//向缓存中追加数据
            if(tempData.size() == sizeof(data.transData))//组成了一包数据
            {
                //将缓存中的数据拷贝到数据包对象中
                memcpy(data.transData, tempData.data(), sizeof(data.transData));
                tempData.clear();//清空缓存
            }
            else//不完全的数据包不能执行下面的操作
            {
                break;
            }
        } 
            
        switch(data.data.type)//判断数据包类型
        {
        case listType:
            doList(data);
            break;
        case downloadType:
            doDownload(data);
            break;
        }
    }
}

void MainWindow::on_listWidget_file_itemDoubleClicked(QListWidgetItem *item)
{
    path = QFileDialog::getExistingDirectory(this, "选择下载路径", ".");
    
    QString fileName = item->text();//获得双击选项中文件的名字
    Data data;
    data.data.type = downloadType;//数据包类型
    OnlySocket::getInstance()->getId(data.data.name);//用户id
    strcpy(data.data.fileName, fileName.toUtf8().data());//下载文件的名字
    socket->write(data.transData, sizeof(data.transData));//发送数据
}
