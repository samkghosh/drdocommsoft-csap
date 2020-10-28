
#include <QMessageBox>
#include "dlgaddroute.h"
#include "ui_dlgaddroute.h"
#include "dbaccess.h"


DlgAddRoute::DlgAddRoute(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAddRoute)
{
    ui->setupUi(this);
    this->setWindowTitle("Add/Del DID Routing");
}

DlgAddRoute::~DlgAddRoute()
{
    delete ui;
}

void DlgAddRoute::on_pbSave_clicked()
{
        RTData rd;

        rd.context = "incomingcall";
        rd.DID = ui->leDID->text();
        if ( rd.DID == nullptr || rd.DID=="")
        {
            QMessageBox::warning(this,"Route Table","DID cann't be blank.");
            return;
        }

        rd.priority = ui->lePriority->text().toInt();

        if ( rd.priority <= 0)
        {
            QMessageBox::warning(this,"Route Table","Invalid priority.");
            return;
        }

        rd.action = ui->leAction->text();
        if ( rd.action == nullptr || rd.action == "")
        {
            QMessageBox::warning(this,"Route Table","Action cann't be blank.");
            return;
        }

        rd.destination = ui->leDest->text();
        if ( rd.destination == nullptr || rd.destination == "")
        {
            QMessageBox::warning(this,"Route Table","Destination cann't be blank.");
            return;
        }


        DBAccess *dba = new DBAccess("route");

        int rc = dba->saveRoutingTable(rd);

        if ( rc > 0 )
        {
           QMessageBox::information(this, "ACD/Ringgroup", "Added successfully");
           close();
        }
        else
        {
            QMessageBox::warning(this, "ACD/Ringgroup", "Unble to added. Return value:"+QString::number(rc));

        }
}

void DlgAddRoute::on_pbCancel_clicked()
{
    close();
}

void DlgAddRoute::on_leDID_textChanged(const QString &arg1)
{
    QList<RTData *> *rtd =nullptr;

//    ui->leDID->setText("");
    ui->lePriority->setText("1");
    ui->lePriority->setDisabled(true);
    ui->leAction->setText("Dial");
    ui->leDest->setText("");

    rtd = &routeData->value(arg1)->data;

    if ( rtd != nullptr)
    {
        showRouteData(rtd);
    }

}

void DlgAddRoute::setRoutingData(RoutingTable *rtd)
{
    routeData = rtd;
}

void DlgAddRoute::showRouteData(QList<RTData *>*rtd)
{
    ui->leDID->setText(rtd[0].at(0)->DID);
    ui->lePriority->setText(QString::number(rtd[0].at(0)->priority));
    ui->leAction->setText(rtd[0].at(0)->action);
    ui->leDest->setText(rtd[0].at(0)->destination);
    ui->lePriority->setDisabled(false);

}


//delete routing info
void DlgAddRoute::on_pushButton_clicked()
{
    RTData rd;

    if (QMessageBox::question(this,"Routing Table","Are you sure to delete?") == QMessageBox::Yes)
    {

        rd.context = "incomingcall";
        rd.DID = ui->leDID->text();
        if ( rd.DID == nullptr || rd.DID=="")
        {
            QMessageBox::warning(this,"Route Table","DID cann't be blank.");
            return;
        }

        rd.priority = ui->lePriority->text().toInt();

        if ( rd.priority <= 0)
        {
            QMessageBox::warning(this,"Route Table","Invalid priority.");
            return;
        }

        rd.action = ui->leAction->text();
        if ( rd.action == nullptr || rd.action == "")
        {
            QMessageBox::warning(this,"Route Table","Action cann't be blank.");
            return;
        }


        DBAccess *dba = new DBAccess("route");

        int rc = dba->deleteRoutingTable(rd);

        if ( rc > 0 )
        {
           QMessageBox::information(this, "ACD/Ringgroup", "Deleted successfully");
           close();
        }
        else
        {
            QMessageBox::warning(this, "ACD/Ringgroup", "Unble to delete. Return value:"+QString::number(rc));

        }

    }
}

