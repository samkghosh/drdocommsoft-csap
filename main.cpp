#include "mainwindow.h"

#include <QApplication>

#include "dlglogin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    DlgLogin *dlgLogin = new DlgLogin();

    dlgLogin->show();



//    w.show();
    return a.exec();
}
