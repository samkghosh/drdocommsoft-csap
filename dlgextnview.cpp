///////////////////////////////////////////////////////////////////
// FileName: dlgextnview.cpp
//  Fethes the eextenssion and display on the scerrn. Add mdify extension action
//
//////////////////////////////////////////////////////////////////

#include "dlgextnview.h"
#include "ui_dlgextnview.h"
#include <QTableWidget>

#include <QMessageBox>



DlgExtnView::DlgExtnView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgExtnView)
{
    ui->setupUi(this);
    setWindowTitle("Extension Management");
    dlgExtensionConf = new DlgExtensionConf;

    connect(ui->tblExtns, SIGNAL(cellDoubleClicked(int,int)),this, SLOT(cellDoubleClicked(int,int)));

    connect(dlgExtensionConf, SIGNAL(extensionModified()), this, SLOT(onExtensionModified()));
}


DlgExtnView::~DlgExtnView()
{
    delete  dlgExtensionConf;
    delete ui;
}


void DlgExtnView::cellDoubleClicked(int row, int col)
{
    QTableWidgetItem *item=nullptr;
    Extension *extn;
    qDebug() << "row:"<<row <<" "<<col;

    item = ui->tblExtns->item(row,0);
    qDebug() << "Extn:" << item->text();

    extn = extensions.value(item->text());
    dlgExtensionConf->setExtensionList(&extensions);
    dlgExtensionConf->showConfigDialog(extn);

}

void DlgExtnView::on_pbAdd_clicked()
{
    // if checked, then get the extension info of the first
    // checked etension

    dlgExtensionConf->setModal(true);
    dlgExtensionConf->setExtensionList(&extensions);
    dlgExtensionConf->show();
}



void DlgExtnView::displayExtension()
{
    int iRow=0;
    int iCol=0;
    QMap<QString, Extension *>:: iterator itr=nullptr;
    QString extNum="";
    DBAccess *dba = new DBAccess("extn");
    Extension *extnData=nullptr;
    int iExtnCnt =0;

    if ( extensions.count() == 0)
    {
        iExtnCnt = dba->readExtensionData(extensions);

        if ( iExtnCnt < 0)
        {
            QMessageBox::warning(this, "Extension", "Unable to get extension informaion, check Database");
            return;
        }
        else if (iExtnCnt == 0)
        {
            QMessageBox::warning(this, "Extension", "No Extension defined. Add extensions.");

        }

        delete dba;
    }

    ui->tblExtns->clear();
    ui->tblExtns->setRowCount(0);

   QStringList lbl;
//   lbl << "" <<"Extension" << "Name" << "Password" << "Codec" << "Admin Mode" << "Type" << "Record Mode";
      lbl << "Extension" << "Name" << "Password" << "Codec" << "Admin Mode" << "Type" << "Record Mode";
   ui->tblExtns->setColumnCount(7);

   ui->tblExtns->setHorizontalHeaderLabels(lbl);

    iRow=0;

   for(itr=extensions.begin(); itr != extensions.end();itr++)
   {
//       QTableWidgetItem *select = new QTableWidgetItem();
       QTableWidgetItem *exNum=new QTableWidgetItem();
       QTableWidgetItem *name=new QTableWidgetItem();
       QTableWidgetItem *pwd =new QTableWidgetItem();
       QTableWidgetItem *codec =new QTableWidgetItem();
       QTableWidgetItem *mode =new QTableWidgetItem();
       QTableWidgetItem *type=new QTableWidgetItem();
       QTableWidgetItem *recMode =new QTableWidgetItem();

       extNum = itr.key();
       extnData = itr.value();

       exNum->setText(extnData->endPoint.extn);
       name->setText(extnData->endPoint.name);
       pwd->setText(extnData->auths.password);
       codec->setText(extnData->endPoint.allowCodecs);
       switch(extnData->endPoint.recMode)
       {
            case 0:
                recMode->setText("No");
               break;
            case 1:
                  recMode->setText("Channel only");
                  break;
           case 2:
                 recMode->setText("Conv. only");
                 break;
           case 3:
                  recMode->setText("Both");
                break;
           default:
                  recMode->setText("No");
                  break;

       }

       mode->setText("recMode");
       type->setText("extn Type");

       ui->tblExtns->insertRow(iRow);
       iCol=0;

  //     ui->tblExtns->setItem(iRow, iCol++, select);
  //     select->setCheckState(Qt::Unchecked);

       ui->tblExtns->setItem(iRow,iCol++,exNum);
       ui->tblExtns->setItem(iRow,iCol++,name);
       ui->tblExtns->setItem(iRow,iCol++,pwd);
       ui->tblExtns->setItem(iRow,iCol++,codec);
//       ui->tblExtns->setItem(iRow,iCol++,mode);
       iCol++;
//       ui->tblExtns->setItem(iRow,iCol++,type);
        iCol++;
       ui->tblExtns->setItem(iRow,iCol++,recMode);

        iRow++;
    }

    ui->lblTotal->setText("Total: "+QString::number(iRow));
}

void DlgExtnView::on_lineEdit_textChanged(const QString &arg1)
{
    QList<Extension *> extns = extensions.values(arg1);
    QString extNum;


    if ( extns.count() > 0)
    {
        ui->tblExtns->clear();
        ui->tblExtns->setRowCount(0);

        QStringList lbl;
   //   lbl << "" <<"Extension" << "Name" << "Password" << "Codec" << "Admin Mode" << "Type" << "Record Mode";
         lbl << "Extension" << "Name" << "Password" << "Codec" << "Admin Mode" << "Type" << "Record Mode";
        ui->tblExtns->setColumnCount(7);

        ui->tblExtns->setHorizontalHeaderLabels(lbl);

        int iRow=0;

        for(int i =0; i < extns.count(); i++)
        {
             QTableWidgetItem *exNum=new QTableWidgetItem();
             QTableWidgetItem *name=new QTableWidgetItem();
             QTableWidgetItem *pwd =new QTableWidgetItem();
             QTableWidgetItem *codec =new QTableWidgetItem();
             QTableWidgetItem *mode =new QTableWidgetItem();
             QTableWidgetItem *type=new QTableWidgetItem();
             QTableWidgetItem *recMode =new QTableWidgetItem();


 //         extNum = extns[i]->endPoint.extn;

             exNum->setText(extns[i]->endPoint.extn);
             name->setText(extns[i]->endPoint.name);
             pwd->setText(extns[i]->auths.password);
             codec->setText(extns[i]->endPoint.allowCodecs);
             switch(extns[i]->endPoint.recMode)
             {
                  case 0:
                      recMode->setText("No");
                     break;
                  case 1:
                        recMode->setText("Channel only");
                        break;
                 case 2:
                       recMode->setText("Conv. only");
                       break;
                 case 3:
                        recMode->setText("Both");
                      break;
                 default:
                        recMode->setText("No");
                        break;

             }

             mode->setText("recMode");
             type->setText("extn Type");

             ui->tblExtns->insertRow(iRow);

             int iCol =0;
        //     ui->tblExtns->setItem(iRow, iCol++, select);
        //     select->setCheckState(Qt::Unchecked);

             ui->tblExtns->setItem(iRow,iCol++,exNum);
             ui->tblExtns->setItem(iRow,iCol++,name);
             ui->tblExtns->setItem(iRow,iCol++,pwd);
             ui->tblExtns->setItem(iRow,iCol++,codec);
   //       ui->tblExtns->setItem(iRow,iCol++,mode);
             iCol++;
   //       ui->tblExtns->setItem(iRow,iCol++,type);
              iCol++;
            ui->tblExtns->setItem(iRow,iCol++,recMode);

           iRow++;
       }

       ui->lblTotal->setText("Total: "+QString::number(iRow));
    }
    else
    {
        displayExtension();
    }
}



void DlgExtnView::onExtensionModified()
{
//    QMessageBox::warning(this, "Extension", "Extension Modified");
    extensions.clear();
    displayExtension();
}
