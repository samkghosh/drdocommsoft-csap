#include "dlgrouting.h"
#include <QStyledItemDelegate>
#include "ui_dlgrouting.h"
#include <QMessageBox>


DlgRouting::DlgRouting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRouting)
{
    ui->setupUi(this);
    this->setWindowTitle("DID Routing Table");
}

DlgRouting::~DlgRouting()
{
    delete ui;
}

void DlgRouting::on_pbAdd_clicked()
{
    if (dlgAddRout == nullptr)
    {
        dlgAddRout = new DlgAddRoute();
    }
    dlgAddRout->setModal(true);
    dlgAddRout->setRoutingData(&routeData);
    dlgAddRout->show();
}


void DlgRouting::on_pbCancel_clicked()
{
    close();
}



void DlgRouting::displyRoutingData()
{
    QTreeWidgetItem *root;
    RouteTable *routeTable;

    QTreeWidgetItem *hdr = new QTreeWidgetItem;

    ui->treeRouting->clear();
    // set the header
    hdr->setText(0,"DID");
    hdr->setText(1,"Priority");
    hdr->setText(2,"Action");
    hdr->setText(3,"Destination");

    ui->treeRouting->setHeaderItem(hdr);
    ui->treeRouting->setItemDelegate(new GridDelegate);
  //  qDebug()<<"1";
    ui->treeRouting->setColumnCount(4);
    DBAccess *dba=new DBAccess("route");
//    qDebug()<<"2";

    routeData.clear();

    if ( dba->readRoutingTable(routeData) < 0 )
    {
    //    qDebug()<<"3";

        QMessageBox::warning(this, "Route","Unable to get data from.");
        return;
    }
    else
    {
   //     qDebug()<<"4";
        RoutingTable::iterator itr;
        QString key;
//        QList<RTable *> *rt;
qDebug() << "route Table Length:" << routeData.count();
        for(itr=routeData.begin(); itr != routeData.end(); itr++)
        {
            key = itr.key();
            root = addRoot(key);
            routeTable = itr.value();
         //   qDebug() << "Key" << key << "Numb of Item:" << routeTable->data.length();
            for (int i=0; i < routeTable->data.length(); i++)
            {
           //     qDebug() << "Data:" << routeTable->data[i]->DID <<routeTable->data[i]->context;
                addChild(root, routeTable->data[i]);

            }
        }

    }

}


QTreeWidgetItem * DlgRouting::addRoot(QString name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeRouting);
    item->setText(0,name);
    ui->treeRouting->addTopLevelItem(item);
    return item;
}

void DlgRouting::addChild(QTreeWidgetItem *parent, RTData *rt)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0,rt->DID);
    item->setText(1,QString::number(rt->priority));
    item->setText(2,rt->action);
    item->setText(3,rt->destination);

    parent->addChild(item);
}

