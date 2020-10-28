#include "dlgacd.h"
#include "ui_dlgacd.h"
#include <QMessageBox>
#include "dbaccess.h"
#include "dlgaddacd.h"



DlgACD::DlgACD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgACD)
{
    ui->setupUi(this);
    setWindowTitle("ACD/Ring Group Management");

    dlgAddACD = new DlgAddACD();

    dlgACDMembers = new DlgACDMember();


    connect(ui->tblACDView, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(onItemDoubleClick(QTableWidgetItem *)));

    connect(dlgAddACD, SIGNAL(acdModified()), this, SLOT(onAcdModified()));
    // getAndShowValues();
}

void DlgACD::getAndShowValues()
{
    int iRow=0;
    int iCol=0;
    QStringList lbl;
//    lbl << "Name" << "Context" << "Ring in Use" << "Strategy";
    lbl << "Name" << "Ring in Use" << "Strategy";
    DBAccess *dba = new DBAccess("acd");

    ui->tblACDView->clear();
    ui->tblACDView->setRowCount(0);

    ui->tblACDView->setColumnCount(3);

    ui->tblACDView->setHorizontalHeaderLabels(lbl);

    if ( acds.count() == 0)
    {
        if ( dba->readACDData(acds) < 0)
        {
            QMessageBox::warning(this, "ACD", "Unable to fetch Data, check Database");
            delete dba;
            return;
        }
    }

    delete dba;


    for (iRow =0; iRow < acds.count(); iRow++ )
    {
            ui->tblACDView->insertRow(iRow);
            iCol=0;

      //      QTableWidgetItem *select = new QTableWidgetItem();

            QTableWidgetItem *name= new QTableWidgetItem();

            //QTableWidgetItem *context = new QTableWidgetItem();

            QTableWidgetItem *RiU = new QTableWidgetItem();
            QTableWidgetItem *strategy = new QTableWidgetItem();

            name->setText(acds.at(iRow)->name);
            //context->setText(acds.at(iRow)->context);
            RiU->setText(acds.at(iRow)->ringInUse);
            strategy->setText(acds.at(iRow)->strategy);

    //        ui->tblACDView->setItem(iRow, iCol++, select);
   //         select->setCheckState(Qt::Unchecked);

            ui->tblACDView->setItem(iRow,iCol++,name);
     //       ui->tblACDView->setItem(iRow,iCol++,context);
            ui->tblACDView->setItem(iRow,iCol++,RiU);
            ui->tblACDView->setItem(iRow,iCol++,strategy);
    }
        ui->lblTotal->setText("Total: "+QString::number(iRow));

}

DlgACD::~DlgACD()
{
    delete ui;
}


void DlgACD::on_tblACDView_cellClicked(int row, int column)
{
    QString acdName;

    column =0;
    QTableWidgetItem *q = ui->tblACDView->item(row,column);

    acdName = q->text();
    //  QMessageBox::information(this,"Col", q->text());
//qDebug() << "Acd Name:" << acdName << "xy"<<row << column;

    dlgACDMembers->showMembers(acdName);

}

void DlgACD::on_pbAdd_clicked()
{
    dlgAddACD->setModal(true);
    dlgAddACD->setAcds(acds);
    dlgAddACD->show();
}


void DlgACD::onAcdModified()
{
    acds.clear();
    getAndShowValues();
}
