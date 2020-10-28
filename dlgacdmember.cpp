#include "dlgacdmember.h"
#include "ui_dlgacdmember.h"
#include "dbaccess.h"
#include <QMessageBox>
#include <QListWidgetItem>

DlgACDMember::DlgACDMember(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgACDMember)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowStaysOnTopHint);

    connect(ui->lstExtns,SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(sendExtnToMemebr(QListWidgetItem *)));
    connect(ui->lstMembers,SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(sendMemberToExtn(QListWidgetItem *)));

}

DlgACDMember::~DlgACDMember()
{
    delete ui;
}

void DlgACDMember::setMembers()
{
    QMap<QString,Extension *>::iterator itr;
    QMap<QString,ACDMember*>::iterator itrMember;

    QListWidgetItem *lstItem;
    QString key;
    ACDMember *acdMembers;
    int iExits;

    ui->lstExtns->clear();
    ui->lstMembers->clear();

    members.clear();
 //   empty();

    DBAccess *dba = new DBAccess("queue");

    if ( dba->readExtensionData(extnList) < 0)
    {
        QMessageBox::warning(this,"ACD Members", "Unable to get List");
        return;
    }

    if ( dba->readACDMembers(members, acdName) < 0)
    {
        QMessageBox::warning(this,"ACD Members", "Unable to get Existing Members");
        return;
    }


    // add the List into ist list
//    qDebug() << "Members has: " << members.count();

    if ( members.count() > 0)
    {
        itrMember = members.begin();
        key = itrMember.key();
        acdMembers = itrMember.value();
    }

    for (itr = extnList.begin(); itr != extnList.end(); itr++)
    {
        // chek if it is in members
        key = itr.key();
        iExits=0;

        for(int i=0; i < acdMembers->member.count(); i++)
        {
            if ("PJSIP/"+key == acdMembers->member[i]->interface )
            {
                iExits =1;
                break;
            }
        }

        if (iExits == 0)
        {
            // add it here
            lstItem = new QListWidgetItem();
            lstItem->setText(key);
            ui->lstExtns->addItem(lstItem);
        }
    }

    //add the members in the second list
    for (int i=0; i < acdMembers->member.count(); i++)
    {
       // add it here
        lstItem = new QListWidgetItem();
        lstItem->setText(acdMembers->member[i]->interface.replace("PJSIP/",""));
        ui->lstMembers->addItem(lstItem);
    }


}
void DlgACDMember::showMembers(QString name)
{
    acdName = name;
  //  QMessageBox::information(this,"ACD Name", acdName);
    setMembers();
    ui->lbACDName->setText("ACD/Ring Group Name: "+acdName);
    show();

}

void DlgACDMember::sendExtnToMemebr(QListWidgetItem *item )
{
    qDebug() << "Exten:" << item->text();
    QListWidgetItem *listitem = new QListWidgetItem();
    listitem->setText(item->text());
    ui->lstMembers->addItem(listitem);
//    ui->lstExtns->removeItemWidget(item);
    delete item;
}

void DlgACDMember::sendMemberToExtn(QListWidgetItem *item )
{
    QListWidgetItem *listitem = new QListWidgetItem();
    listitem->setText(item->text());
    ui->lstExtns->addItem(listitem);
    delete item;
}

void DlgACDMember::on_pbSave_clicked()
{
    QString name;
    ACDMember members;
    ACDMemberPro *memPro;

    // get all the items
    members.acdName = acdName;
    for(int i = 0; i < ui->lstMembers->count(); ++i)
    {
         name= ui->lstMembers->item(i)->text();
         memPro = new ACDMemberPro();

         memPro->paused=0;
         memPro->penalty=0;
         memPro->interface = "PJSIP/"+ name;
         memPro->stateInterface="";
         memPro->stateInterface="";
         memPro->memberName = name;

         members.member.insert(i, memPro);
        //Do stuff!
    }
    DBAccess *dba = new DBAccess("members");

    if(dba->saveRGData(members) < 0)
    {
        QMessageBox::warning(this,"ACD Members","Members could not be added");
    }
    else
    {
        QMessageBox::warning(this,"ACD Members","Members added Successfully");
        close();
    }
     delete dba;
}

void DlgACDMember::on_pushButton_2_clicked()
{
    close();
}
