#include "LoginDialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    LoginDialog ld;
    ld.show();
    return a.exec();
}
