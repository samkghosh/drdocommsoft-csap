#include "dlgcdr.h"
#include "ui_dlgcdr.h"

DlgCDR::DlgCDR(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCDR)
{
    ui->setupUi(this);
    displayCDR();
}

DlgCDR::~DlgCDR()
{
    delete ui;

}


void DlgCDR::displayCDR()
{
    int iRow=0;
    int iCol=0;
    QStringList hdr;
    QString key=nullptr;

    DBAccess *dba = new DBAccess("cdr");

    this->setWindowTitle("Report on CDR");

    int rows = ui->tblCDR->rowCount();

    for (int i =0; i < rows; i++)
            ui->tblCDR->removeRow(i);
    ui->tblCDR->setRowCount(0);

    hdr << "" <<"From Number" << "To Number" << "context" << "Start Time"<< "Duration";

    ui->tblCDR->setColumnCount(6);


    ui->tblCDR->setHorizontalHeaderLabels(hdr);
    ui->tblCDR->setColumnWidth(0,20);

    qDeleteAll(cdrs.begin(), cdrs.end());
    cdrs.clear();

    int iRecCount = dba->readCDRData(cdrs,0);

    delete dba;
    qDebug() << "CDR Record Count:" << iRecCount;

    iRow = 0;
    for(int i=0; i < cdrs.count(); i++)
    {
        iCol=0;

        QTableWidgetItem *select = new QTableWidgetItem();
        QTableWidgetItem *from = new QTableWidgetItem();
        QTableWidgetItem *to= new QTableWidgetItem();
        QTableWidgetItem *context = new QTableWidgetItem();
        QTableWidgetItem *date = new QTableWidgetItem();
        QTableWidgetItem *duration = new QTableWidgetItem();

        from->setText(cdrs[i]->from);
        to->setText(cdrs[i]->to);
        context->setText(cdrs[i]->context);
        date->setText(cdrs[i]->dateTime.toString());
        duration->setText(QString::number(cdrs[i]->duration));

        ui->tblCDR->insertRow(iRow);
        ui->tblCDR->setItem(iRow, iCol++, select);
        select->setCheckState(Qt::Unchecked);

        ui->tblCDR->setItem(iRow, iCol++, from);
        ui->tblCDR->setItem(iRow, iCol++, to);
        ui->tblCDR->setItem(iRow, iCol++, context);
        ui->tblCDR->setColumnWidth(iCol, 180);
        ui->tblCDR->setItem(iRow, iCol++, date);
        ui->tblCDR->setItem(iRow, iCol++, duration);

        iRow++;

    }
    ui->lblCDRTotal->setText("Total:"+QString::number(iRow));


}


