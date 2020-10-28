#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <QtCore>


#define CFGFILE "/etc/csap/config.cfg" // "/ect/csap/config.cfg"

class DBInfo
{
public:
    QString dBServer;
    QString dBUser;
    QString dBAuth;
    QString dBName;
};

class AdminInfo
{
public:
    QString adminName;
    QString auth;
};



class DataProcessing
{
public:
    DataProcessing();

    int readUserInfo(AdminInfo &ai);
    int readDBInfo(DBInfo &dinfo);

};

#endif // DATAPROCESSING_H
