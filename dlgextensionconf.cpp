#include "dlgextensionconf.h"
#include "ui_dlgextensionconf.h"

#include <QComboBox>
#include <QStringList>
#include <QMessageBox>

DlgExtensionConf::DlgExtensionConf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgExtensionConf)
{
    ui->setupUi(this);

    QStringList listYesNo;
    QStringList listRecMode;

    listYesNo << "no" << "yes";
    ui->cbAdminMode->addItems(listYesNo);
    ui->cbMediaEncry->addItems(listYesNo);

    listRecMode << "no" << "Channel Recording" << "Conversation Recorind" << "Both";
    ui->cbRecMode->addItems(listRecMode);

    ui->cbAdminMode->setCurrentIndex(0);
    ui->cbMediaEncry->setCurrentIndex(0);
    ui->cbRecMode->setCurrentIndex(3);

    ui->leExtionsion->setReadOnly(false);
}

DlgExtensionConf::~DlgExtensionConf()
{
    delete ui;
}

void DlgExtensionConf::on_pbClear_clicked()
{
    ui->leExtionsion->setReadOnly(false);

    ui->leExtionsion->setText("");
    ui->leName->setText("");
    ui->lePassword->setText("");
    ui->leCodec->setText("");
    ui->leContext->setText("");
    ui->cbRecMode->setCurrentIndex(3);
    ui->cbMediaEncry->setCurrentIndex(1);
    ui->cbAdminMode->setCurrentIndex(1);

}

void DlgExtensionConf::on_pbDefault_clicked()
{
    DBAccess dba;
    QString extn;

    extn = ui->leExtionsion->text();
    if (  extn == "")
    {
        QMessageBox::warning(this,"Extension","Eneter a new extension");
    }
    extnData.endPoint.extn=extn;

    dba.extensionSetDef(extnData);
    showConfigDialog(&extnData);
    ui->leExtionsion->setReadOnly(false);

}

void DlgExtensionConf::on_pbSave_clicked()
{
    QString val;

    if ( extnData.endPoint.extn == nullptr || extnData.endPoint.extn == "")
    {
        QMessageBox::warning(this,"Extension","Extension is Blank. Set values before adding");
        return;
    }

    //Update the values from UI
//    ui->leExtionsion->setText(extnData->endPoint.extn);
    extnData.endPoint.name = ui->leName->text();
    if ( extnData.endPoint.name == "")
        extnData.endPoint.name = extnData.endPoint.extn;

    extnData.auths.password = ui->lePassword->text();
    if(extnData.auths.password == "")
        extnData.auths.password = extnData.endPoint.extn;

    val = ui->leCodec->text();
    if (val == "")
    {
        QMessageBox::warning(this,"Extension", "Codec can not be blank.");
        return;
    }

    extnData.endPoint.allowCodecs = val;

   val =  ui->leContext->text();

   if ( val == "")
   {
       QMessageBox::warning(this,"Extension", "Conext can not be blank.");
       return;
   }

   extnData.endPoint.context = val;
   extnData.endPoint.recMode = ui->cbRecMode->currentIndex();

   qDebug() << "RecMode" << extnData.endPoint.recMode;

   extnData.endPoint.mediaEncryption = ui->cbMediaEncry->currentText();

   extnData.endPoint.adminMode = ui->cbAdminMode->currentIndex();

   qDebug() << "AdminMode" << extnData.endPoint.adminMode;

    DBAccess dba;

    if ( dba.saveExtensionData(extnData) == 1)
    {
        QMessageBox::information(this,"Extension","Extension saved successfully.");
        this->close();

        Extension *extn = new Extension();

        memcpy(extn, &extnData, sizeof(Extension));

        extnList->insert(extnData.endPoint.extn,extn);

        emit extensionModified();
        close();

    }
    else
    {
        QMessageBox::warning(this,"Extension","Extension couldnot be saved.");

    }

    close();
    return;
}

void DlgExtensionConf::on_leExtionsion_textChanged(const QString &arg1)
{
    Extension *ext=nullptr;

    ext = extnList->value(arg1);

    if ( ext != nullptr)
    {
        extnData = *ext;
        showConfigDialog(&extnData);
    }
}

int DlgExtensionConf::showConfigDialog(Extension *extnData)
{

    ui->leExtionsion->setText(extnData->endPoint.extn);
    ui->leName->setText(extnData->endPoint.name);
    ui->lePassword->setText(extnData->auths.password);
    ui->leCodec->setText(extnData->endPoint.allowCodecs);
    ui->leContext->setText(extnData->endPoint.context);
    ui->cbRecMode->setCurrentIndex(extnData->endPoint.recMode);
    ui->cbMediaEncry->setCurrentText(extnData->endPoint.mediaEncryption);
    ui->cbAdminMode->setCurrentIndex(extnData->endPoint.adminMode);

    ui->leExtionsion->setReadOnly(true);

    this->show();

    return 0;
}

void DlgExtensionConf::on_pbDelete_clicked()
{
    QString extn = ui->leExtionsion->text();

    if (QMessageBox::question(this,"Extension","Deleteing extension "+extn+ " will not be avaiable more. Continue?") == QMessageBox::Yes)
    {
        if (QMessageBox::question(this,"Extension","Aru you sure to delete this extension " +extn +"?") == QMessageBox::Yes)
        {
            DBAccess *dba = new DBAccess("Extn");
            if (dba->removeExtension(extn) == 1)
            {
                QMessageBox::information(this,"Extension", "Extension deleted successfully.");
                emit extensionModified();

                extnList->remove(extn);
                this->close();

            }
            else
            {
                QMessageBox::information(this,"Extension", "Extension coudnot be deleted.");

            }
        }
    }

}
