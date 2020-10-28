#include "dlglogin.h"
#include "ui_dlglogin.h"
#include "mainwindow.h"
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QDebug>

#include "dataprocessing.h"


DlgLogin::DlgLogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgLogin)
{
//   DlgLogin *dlg=new DlgLogin();

  // dlg->setObjectName("Login");
    ui->setupUi(this);
    setWindowTitle("Login");
}

DlgLogin::~DlgLogin()
{
    delete ui;
}

void DlgLogin::on_btnLoginOK_clicked()
{
    MainWindow *mw=new MainWindow();
    QString passWord;
    QString userID;
    QMessageBox *msg;

    DataProcessing dp;
    AdminInfo admin;

    dp.readUserInfo(admin);

    qDebug() << admin.adminName <<"::" << admin.auth;

    passWord = ui->lePwd->text();
    userID = ui->leUserID->text();


    qDebug() <<userID <<":" << admin.adminName <<"::"<<passWord<<":"<<admin.auth;

    if (passWord == "" || userID == "" || !( passWord == admin.auth && userID == admin.adminName))
    {
        msg = new QMessageBox();

        msg->information(this, "Login Failure","Invalid UserID or Password");
        close();
    }
    else
    {
        this->hide();
        mw->show();
    }
}
