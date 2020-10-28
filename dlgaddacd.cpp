#include "dlgaddacd.h"
#include "ui_dlgaddacd.h"
#include <QMessageBox>
#include "dbaccess.h"

DlgAddACD::DlgAddACD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAddACD)
{
    ui->setupUi(this);

    showAcdInfo(nullptr);
}

void DlgAddACD::showAcdInfo(ACD *acd)
{
    QStringList listYesNo;

    QStringList listStrategy;

    listYesNo << "no" << "yes";
    // "ringall" << "leastrecent" << "fewestcalls" << "random" << "rrmemory" << "linear" << "wrandom" << "rrordered";

    listStrategy << "ringall" << "leastrecent" << "fewestcalls" << "random" << "rrmemory" << "linear" << "wrandom" << "rrordered";

    ui->cbStrategy->clear();
    ui->cbRiU->clear();

    if ( acd == nullptr)
    {
        ui->cbStrategy->addItems(listStrategy);

        ui->cbRiU->addItems(listYesNo);

        ui->cbStrategy->setCurrentText("ringall");
        ui->cbRiU->setCurrentText("no");
    }
    else
    {
        ui->leName->setText(acd->name);

        ui->cbStrategy->addItems(listStrategy);

        ui->cbRiU->addItems(listYesNo);

        ui->cbStrategy->setCurrentText(acd->strategy);
        ui->cbRiU->setCurrentText(acd->ringInUse);

    }

}


DlgAddACD::~DlgAddACD()
{
    delete ui;
}

void DlgAddACD::on_pbCancel_clicked()
{
    close();
}

void DlgAddACD::on_pbSave_clicked()
{
    QString name;
    QString riu;
    QString strategy;


    name = ui->leName->text();

    if ( name == nullptr || name == "")
    {
        QMessageBox::information(this,"Add ACD","ACD/Ring group name cann't be blank");
        return;
    }

    riu = ui->cbRiU->currentText();
    if ( riu == nullptr || riu == "")
    {
        QMessageBox::information(this,"Add ACD", "Select 'yes or no in Ring in-use");
        return;
    }
    strategy = ui->cbStrategy->currentText();
    if ( strategy == nullptr || strategy == "")
    {
        QMessageBox::information(this,"Add ACD", "Select a Strategy");
        return;
    }

    ACD acd;

    acd.name = name;
    acd.context="";
    acd.ringInUse = riu;
    acd.strategy = strategy;

    DBAccess *dba = new DBAccess();

    if ( dba->saveACDData(acd) <= 0)
    {
        QMessageBox::information(this,"Add ACD", "Unable add ACD/Ring Group.");

    }
    else
    {
        QMessageBox::information(this,"Add ACD", "ACD/Ring Group added successfully.");
        emit acdModified();
        close();
    }
    delete dba;
}

void DlgAddACD::on_pbDel_clicked()
{
    QString name;
    QString riu;
    QString strategy;


    if (QMessageBox::question(this,"ACD/Ring Group", "Are you sure to delet?") != QMessageBox::Yes)
    {
        return;
    }
    else
    {
        QMessageBox::warning(this,"ACD/Ring Group", "Not Implemented.");
    }
}

void DlgAddACD::on_leName_textChanged(const QString &arg1)
{
    for(int i=0; i < acds.count(); i++)
    {
        if ( arg1 == acds[i]->name)
        {
            showAcdInfo(acds[i]);
        }
    }
}

void DlgAddACD::setAcds(QList<ACD *> acd)
{
    acds = acd;
}
