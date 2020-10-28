#include <QtCore>
#include <QStandardPaths>
#include <QJsonObject>
#include <QtDebug>


#include "dataprocessing.h"

DataProcessing::DataProcessing()
{

}



int DataProcessing::readDBInfo(DBInfo &dinfo)
{
    QSettings dbSetting(CFGFILE, QSettings::NativeFormat);

    dinfo.dBServer = dbSetting.value("DBServer").toString();
    dinfo.dBUser   = dbSetting.value("DBUser").toString();
    dinfo.dBAuth   = dbSetting.value("DBPwd").toString();
    dinfo.dBName   = dbSetting.value("DBName").toString();

qDebug() << dinfo.dBServer;

    return 1;
}

int DataProcessing::readUserInfo(AdminInfo &ai)
{
    QSettings qs(CFGFILE, QSettings::NativeFormat);
    QVariant val;

    val = qs.value("ADMINNAME");
    qDebug() << "Name:"<<val.toString();

    ai.adminName = qs.value("AdminName").toString();

    ai.auth = qs.value("AdminPwd").toString();
    return 1;

}
