//########################################################################
// FileName: dbaccess.cpp
//Purpose: Difines all da relates input output
//########################################################################

#include <QtCore>
#include <QStandardPaths>
#include <QJsonObject>
#include <QtDebug>
#include <QtSql>
#include <QSqlDatabase>


#include "dbaccess.h"
#include "dataprocessing.h"


DBAccess::DBAccess()
{
    if ( driverLoaded == 0)
    {
        db = QSqlDatabase::addDatabase("QMYSQL");
        driverLoaded = 1;
    }
}

DBAccess::DBAccess(QString conn)
{

    qDebug() << "DBAccess:" << conn;

    connectionName = conn;

    if ( driverLoaded == 0)
    {
        db = QSqlDatabase::addDatabase("QMYSQL");
        driverLoaded = 1;
    }
    setDBInfo();

}

DBAccess::~DBAccess()
{

    if (db.isOpen())
        db.close();

//    QSqlDatabase::removeDatabase(connectionName);
}

void DBAccess::setDBInfo()
{
//    dbServer="167.71.230.148";
///    dbUID="commsoft";
// dbPWD = "!Alliance@1986!";
 //   dbDBName = "asterisk";

    DataProcessing dp;
    DBInfo dbi;

    dp.readDBInfo(dbi);
    qDebug() << "Host:" << dbi.dBServer <<"Uid:"<<dbi.dBUser << "pwd:"<<dbi.dBAuth << "name:" << dbi.dBName;
    dbServer = dbi.dBServer;
    dbUID = dbi.dBUser;
    dbPWD = dbi.dBAuth;
    dbDBName = dbi.dBName;

    db.setHostName(dbServer);
    db.setUserName(dbUID);
    db.setPassword(dbPWD);
    db.setDatabaseName(dbDBName);

}

int DBAccess::openDB()
{

    qDebug() << "Conn:" << connectionName << "Drivename:"<<db.driverName();
    setDBInfo();

    db.setHostName(dbServer);
    db.setUserName(dbUID);
    db.setPassword(dbPWD);
    db.setDatabaseName(dbDBName);

 //  db = QSqlDatabase::addDatabase("QMYSQL", connectionName);

    bool bOk = db.open();
    if ( bOk == false)
    {
        QSqlError err = db.lastError();

        if (err.driverText().contains("Driver not loaded"))
        {
            qDebug() << "Found Driver not loaded. Retrying";
            db.addDatabase("QMYSQL");
            bOk = db.open();
            if ( bOk == false)
            {
                qDebug() << "Unable to open DB. Error:" << db.lastError() << "Server:" << dbServer;
            }
        }
        else
        {
            qDebug() << "Unable to open DB. Error:" << db.lastError() << "Server:" << dbServer;
        }
    }

//    connectionName = db.connectionName();

    qDebug() << "ConnectionName" << connectionName;
    return bOk;
}


void DBAccess::closeDB()
{

   if (db.open())
   {
        db.close();
    }
    db.removeDatabase(connectionName);
}

QString getLastExecutedQuery(const QSqlQuery& query)
{
 QString str = query.lastQuery();
 QMapIterator<QString, QVariant> it(query.boundValues());
 while (it.hasNext())
 {
  it.next();
  str.replace(it.key(),it.value().toString());
 }
 return str;
}

//////////////////////////////////////////////////////////////
// Extension information read-write into DB
///////////////////////////////////////////////////////////////
void DBAccess::extensionSetDef(Extension &extn)
{
//    extn.endPoint.extn="";

    extn.endPoint.name = extn.endPoint.extn;
    qDebug() << "Extn:" << extn.endPoint.extn << "Name:"<<extn.endPoint.name;
    extn.endPoint.transortType = "transport-udp"; //"transport_udp" allways in filed 'transport'
    extn.endPoint.aors = extn.endPoint.extn; //id of ps_aors in filed 'aors'
    extn.endPoint.auth = extn.endPoint.extn; //id of ps_auths in field 'auth'
    extn.endPoint.context = "outgoingcall"; // text in the filed 'context' 'i.e. outgoingall
    extn.endPoint.disallowCodecs = "all"; // "all" always in the filed 'disallow'
    extn.endPoint.allowCodecs = "alaw,ulaw"; // in allow field 'allow', i.e. alaw,ulaw
    extn.endPoint.directMedia = "no"; //def no, {yes,no}'iled 'direct_media'
    extn.endPoint.mediaUseRcvdTransport = "yes"; //def 'yes', {yes,no} in the filed 'media_use_received_transport'
    extn.endPoint.trustIdInbound = "yes"; // {yes,no} def yes, field 'trust_id_inbound
    extn.endPoint.mediaEncryption = "no"; // {yes,no}, def no, field 'media_encryption
    extn.endPoint.rtpSymetric = "yes"; //{yes,no}, def yes, field 'rtp_symetric
    extn.endPoint.rewiteContact = "yes"; //{yes,no}, def yes, filed 'rewrite_contact
    extn.endPoint.forceRport = "yes"; //{yes,no}, def yes, filed 'force_rport
    extn.endPoint.adminMode = 0; // {yes,no} def no, yes:  bargein or spy/wishper allowed, no: notallowed
    extn.endPoint.recMode = 3; //0: no recording, 1: chan rec, 2: converation recording, 3: both Chan and Conversation
    extn.endPoint.callForward = "no";
    extn.endPoint.forwardBusy ="";
    extn.endPoint.forwardNoAns="";


    extn.aors.extn =extn.endPoint.extn;
    extn.aors.maxContact = 1;
    extn.endPoint.name = extn.endPoint.extn;
    extn.aors.removeExisting = "yes";

    extn.auths.extn = extn.endPoint.extn;
    extn.auths.authType="userpass";
    extn.auths.password = extn.endPoint.extn;
    extn.auths.userName = extn.endPoint.extn;
}


int DBAccess::readExtensionData(QMap <QString, Extension *> &extnList)
{
    // if db is not opend open it
    Extension *extn;
    QString selqry;
#if 1
    if (db.isOpen() == false)
    {
        qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }



   selqry = "SELECT ep.id, ep.transport, ep.aors, ep.auth, ep.context, ep.disallow, ep.allow,  ep.direct_media, "
           " ep.media_use_received_transport, ep.trust_id_inbound, ep.media_encryption, ep.rtp_symmetric, "
           " ep.rewrite_contact, ep.force_rport, ep.e_callForward, ep.v_forwardBusy, ep.v_forwardNoAns, "
           " ep.i_adminMode, ep.i_recMode,ep.v_name, aors.max_contacts,aors.remove_existing, auth.auth_type, "
           " auth.password, auth.username FROM ps_endpoints ep, ps_aors aors, ps_auths auth WHERE ep.id = aors.id and ep.id=auth.id";

    QSqlQuery qry(db);

    qry.prepare(selqry);


    if (!qry.exec())
    {
        qDebug() << "Error in select:" <<db.lastError();
        return -1;
    }

    int i=0;
    extnList.empty();

    QSqlRecord rec = qry.record();
    int iRecIndex;
    while(qry.next())
    {
        extn = new Extension;

        iRecIndex = rec.indexOf("id");
        extn->endPoint.extn = QVariant(qry.value(iRecIndex)).toString();

        qDebug() << "Fetch Extension:" << extn->endPoint.extn;

        iRecIndex = rec.indexOf("transport");
        extn->endPoint.transortType = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("aors");
        extn->endPoint.aors = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("auth");
        extn->endPoint.auth = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("context");
        extn->endPoint.context = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("disallow");
        extn->endPoint.disallowCodecs = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("allow");
        extn->endPoint.allowCodecs = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("direct_media");
        extn->endPoint.directMedia = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("media_use_received_transport");
        extn->endPoint.mediaUseRcvdTransport = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("trust_id_inbound");
        extn->endPoint.trustIdInbound = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("media_encryption");
        extn->endPoint.mediaEncryption = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("rtp_symmetric");
        extn->endPoint.rtpSymetric = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("rewrite_contact");
        extn->endPoint.rewiteContact = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("force_rport");
        extn->endPoint.forceRport = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("e_callForward");
        extn->endPoint.callForward = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("v_forwardBusy");
        extn->endPoint.forwardBusy = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("v_forwardNoAns");
        extn->endPoint.forwardNoAns = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("i_adminMode");
        extn->endPoint.adminMode = QVariant(qry.value(iRecIndex)).toInt();

        iRecIndex = rec.indexOf("i_recMode");
        extn->endPoint.recMode = QVariant(qry.value(iRecIndex)).toInt();

        iRecIndex = rec.indexOf("v_name");
        extn->endPoint.name = QVariant(qry.value(iRecIndex)).toString();

        qDebug() << "Exten Name" << extn->endPoint.name;

        iRecIndex = rec.indexOf("max_contacts");
        extn->aors.maxContact = QVariant(qry.value(iRecIndex)).toInt();

        iRecIndex = rec.indexOf("remove_existing");
        extn->aors.removeExisting = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("remove_existing");
        extn->aors.removeExisting = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("auth_type");
        extn->auths.authType = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("password");
        extn->auths.password = QVariant(qry.value(iRecIndex)).toString();

        iRecIndex = rec.indexOf("username");
        extn->auths.userName = QVariant(qry.value(iRecIndex)).toString();

        extnList.insert(extn->endPoint.extn, extn);
        i++;
    }

   qry.finish();

    closeDB();
    return i;
#else
    return 0;
#endif
}


int DBAccess::saveExtensionData(Extension &extn)
{
    QString strQry;

    // open database
    if (db.isOpen() == false)
    {
        qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }

    QSqlQuery qry(db);

    //Insert Data into ps_endpoints
    strQry ="REPLACE INTO ps_endpoints (id, transport, aors, auth, context, disallow, allow, "
            "direct_media, media_use_received_transport, trust_id_inbound, media_encryption, rtp_symmetric, rewrite_contact, force_rport, "
             "e_callForward, v_forwardBusy, v_forwardNoAns, i_adminMode, i_recMode, v_name) VALUES "
                "( :id, :transport, :aors, :auth, :context, :disallow, :allow, "
                        ":direct_media, :media_use_received_transport, :trust_id_inbound, :media_encryption, :rtp_symmetric, :rewrite_contact, :force_rport, "
                         ":e_callForward, :v_forwardBusy, :v_forwardNoAns, :i_adminMode, :i_recMode, :v_name)";

    qry.prepare(strQry);



    qry.bindValue(":id", extn.endPoint.extn);
    qry.bindValue(":transport", extn.endPoint.transortType);
    qry.bindValue(":aors", extn.endPoint.aors);
    qry.bindValue(":auth", extn.endPoint.auth);
    qry.bindValue(":context", extn.endPoint.context);
    qry.bindValue(":disallow", extn.endPoint.disallowCodecs);
    qry.bindValue(":allow", extn.endPoint.allowCodecs);

    qry.bindValue(":direct_media", extn.endPoint.directMedia);
    qry.bindValue(":media_use_received_transport", extn.endPoint.mediaUseRcvdTransport);
    qry.bindValue(":trust_id_inbound", extn.endPoint.trustIdInbound);
    qry.bindValue(":media_encryption", extn.endPoint.mediaEncryption);
    qry.bindValue(":rtp_symmetric", extn.endPoint.rtpSymetric);
    qry.bindValue(":rewrite_contact", extn.endPoint.rewiteContact);
    qry.bindValue(":force_rport", extn.endPoint.forceRport);

    qry.bindValue(":e_callForward", extn.endPoint.callForward);
    qry.bindValue(":v_forwardBusy", extn.endPoint.forwardBusy);
    qry.bindValue(":v_forwardNoAns", extn.endPoint.forwardNoAns);
    qry.bindValue( ":i_adminMode",QString::number(extn.endPoint.adminMode));
    qry.bindValue(":i_recMode", QString::number(extn.endPoint.recMode));
    qry.bindValue(":v_name", extn.endPoint.name);

    qDebug() << "Last Qry:" <<getLastExecutedQuery(qry);

    if (!qry.exec())
    {
        qDebug() << "Error in insert:" << strQry <<"Error:"<<db.lastError();
        return -1;
    }


    //Insert Data into ps_auths
    //insert into ps_auths (id, auth_type, password, username)
    //values (7010, 'userpass', 'pass of 7010, 7010);

    strQry ="REPLACE INTO ps_auths(id, auth_type, password, username) VALUES (:id, :auth_type, :password, :username)";
    qry.prepare(strQry);
    qry.bindValue(":id", extn.endPoint.extn);
    qry.bindValue(":auth_type", extn.auths.authType);
    qry.bindValue(":password", extn.auths.password);
    qry.bindValue(":username", extn.auths.userName);
    if (!qry.exec())
    {
        // TBD:: remove data from ps_endpoints
        qDebug() << "Error in insert:" << getLastExecutedQuery(qry) <<"Error:"<<db.lastError();
        return -1;
    }

    //Inser Data into ps_aors
    //insert into ps_aors (id, max_contacts, remove_existing) values (7010, 1, 'yes');

    strQry ="REPLACE INTO ps_aors(id, max_contacts, remove_existing ) VALUES (:id, :max_contacts, :remove_existing)";
    qry.prepare(strQry);

    qry.bindValue(":id", extn.endPoint.extn);
    qry.bindValue(":max_contacts", extn.aors.maxContact);
    qry.bindValue(":remove_existing", extn.aors.removeExisting);

    qDebug() << "Last Qry:" <<getLastExecutedQuery(qry);


    if (!qry.exec())
    {
        // TBD:: remove data from ps_endpoints, ps_auths
        qDebug() << "Error in insert:" << strQry <<"Error:"<<db.lastError();
        return -1;
    }

    //Insert Data into voicemail
    //TBD:: if already there remove it
    strQry = "DELETE from voicemail WHERE mailbox = :mailbox";
    qry.prepare(strQry);
    qry.bindValue(":mailbox", extn.endPoint.extn);

    qDebug() << "Last Qry:" <<getLastExecutedQuery(qry);

    if (!qry.exec())
    {
        qDebug() << "Error in delete:" << strQry <<"Error:"<<db.lastError();
        return -1;
    }

    strQry ="INSERT INTO voicemail(context, mailbox, password, fullname) VALUES ('default', :mailbox, :password, :fullname)";
    qry.prepare(strQry);

    qry.bindValue(":mailbox", extn.endPoint.extn);
    qry.bindValue(":password", extn.endPoint.extn);
    qry.bindValue(":fullname", extn.endPoint.name);

    qDebug() <<"voicemail:" << getLastExecutedQuery(qry);
    if (!qry.exec())
    {
        // TBD:: remove data from ps_endpoints, ps_auths, ps_aors
        qDebug() << "Error in insert:" << getLastExecutedQuery(qry) <<"Error:"<<db.lastError();
        return -1;
    }

    //Insert data into followme
    //TBD:: if already there remove it
    strQry = "DELETE from followme WHERE name = :name";
    qry.prepare(strQry);
    qry.bindValue(":name", extn.endPoint.extn);

    qDebug() << "Last Qry:" <<getLastExecutedQuery(qry);

    if (!qry.exec())
    {
        qDebug() << "Error in delete:" << strQry <<"Error:"<<db.lastError();
        return -1;
    }
    strQry ="INSERT INTO followme(name,context) VALUES (:name, 'outgoingcall')";
    qry.prepare(strQry);

    qry.bindValue(":name", extn.endPoint.extn);

    if (!qry.exec())
    {
        // TBD:: remove data from ps_endpoints, ps_auths, ps_aors, voicemail
        qDebug() << "Error in insert:" << strQry <<"Error:"<<db.lastError();
        return -1;
    }

    return 1;

}


//Remove extension
int DBAccess::removeExtension(QString extnNum)
{
    QString strQry;

    // open database
    if (db.isOpen() == false)
    {
        qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }

    QSqlQuery qry(db);

    // remove from ps_endpoints
    strQry ="DELETE FROM ps_endpoints WHERE id= :id";
    qry.prepare(strQry);
    qry.bindValue(":id", extnNum);
    if (!qry.exec())
    {
        qDebug() << "Error in delet extension :" << getLastExecutedQuery(qry) <<"Error:"<<db.lastError();
        return -1;
    }

    // remove from ps_auths
    strQry ="DELETE FROM ps_auths WHERE id= :id";
    qry.prepare(strQry);
    qry.bindValue(":id", extnNum);
    if (!qry.exec())
    {
        qDebug() << "Error in delet extension :" << getLastExecutedQuery(qry) <<"Error:"<<db.lastError();
        return -1;
    }

    // remove from ps_aors
    strQry ="DELETE FROM ps_aors WHERE id= :id";
    qry.prepare(strQry);
    qry.bindValue(":id", extnNum);
    if (!qry.exec())
    {
        qDebug() << "Error in delet extension :" << getLastExecutedQuery(qry) <<"Error:"<<db.lastError();
        return -1;
    }

    // remove from voicemail
    strQry = "DELETE from voicemail WHERE mailbox = :mailbox";
    qry.prepare(strQry);
    qry.bindValue(":mailbox", extnNum);

    if (!qry.exec())
    {
        qDebug() << "Error in delete:" << getLastExecutedQuery(qry) <<"Error:"<<db.lastError();
        return -1;
    }

    //remove from followme
    strQry = "DELETE from followme WHERE name = :name";
    qry.prepare(strQry);
    qry.bindValue(":name", extnNum);

    if (!qry.exec())
    {
        qDebug() << "Error in delete:" << getLastExecutedQuery(qry) <<"Error:"<<db.lastError();
        return -1;
    }

    // remove from followme_numbers
    strQry = "DELETE from followme_numbers WHERE name = :name OR phonenumber = :phonenumber";
    qry.prepare(strQry);
    qry.bindValue(":name", extnNum);
    qry.bindValue(":phonenumber", extnNum);

    if (!qry.exec())
    {
        qDebug() << "Error in delete:" << getLastExecutedQuery(qry) <<"Error:"<<db.lastError();
        return -1;
    }

    QString pjsExtn;

    pjsExtn ="PJSIP/"+extnNum;

    // remove from extensions
    strQry = "DELETE from extensions WHERE app = 'Dial' and appdata= :appdata";
    qry.prepare(strQry);
    qry.bindValue(":appdata", pjsExtn);
    if (!qry.exec())
    {
        qDebug() << "Error in delete:" << getLastExecutedQuery(qry) <<"Error:"<<db.lastError();
        return -1;
    }


    //remove from queue_members
    strQry = "DELETE from queue_members WHERE interface = :interface";
    qry.prepare(strQry);
    qry.bindValue(":interface", pjsExtn);
    if (!qry.exec())
    {
        qDebug() << "Error in delete:" << getLastExecutedQuery(qry) <<"Error:"<<db.lastError();
        return -1;
    }

    return 1;

}


//CDR
int DBAccess::readCDRData(QList<CDR *> &cdrs, int type)
{
//   setDBInfo();
   CDR *cdr;
   QString selqry;

   if (db.isOpen() == false)
   {
       qDebug() << "Db Is not Opened";

       if (openDB() == false)
       {
           qDebug() << "Unable to open: " << db.lastError();
           return -1;
       }
       else
       {
           qDebug() << "Db Opened successfully";

       }

   }
   else
   {
       qDebug() << "Db Already Opened.";

   }


   if ( type == 0)
   {
       selqry = "SELECT calldate, src, dst, duration, disposition, dcontext from cdr order by calldate desc";
   }
   else
   {
       selqry = "SELECT calldate, src, dst, duration, disposition, dcontext,v_voicefileName FROM cdr WHERE "
              " v_voicefileName IS NOT NULL AND v_voicefileName != '' ORDER BY calldate DESC";
   }

   QSqlQuery *qry = new QSqlQuery(db);

   qry->prepare(selqry);


   if (!qry->exec())
   {
       qDebug() << "Error in select:" <<db.lastError();
       return -1;
   }


//   QSqlRecord rec = qry.record();

//   qDebug() << "Query sucess:" << selqry << "Rec Count:" <<rec.count() ;


   int i=0;
   cdrs.empty();


   while(qry->next())
   {
       cdr = new CDR;

//       QString selqry = "SELECT calldate, src, dst, duration, disposition from cdr";
       cdr->dateTime = QVariant(qry->value(0)).toDateTime();
       cdr->from = QVariant(qry->value(1)).toString();
       cdr->to = QVariant(qry->value(2)).toString();
       cdr->duration = QVariant(qry->value(3)).toInt();
       cdr->disposition = QVariant(qry->value(4)).toString();
       cdr->context = QVariant(qry->value(5)).toString();
       if (type == 0)
       {
           cdr->recFileName = "";
       }
       else
       {
           cdr->recFileName = QVariant(qry->value(6)).toString();
       }
//qDebug() << "from:" << cdr->from;
       cdrs.insert(i, cdr);
       i++;
   }

   qry->finish();
   delete qry;

   closeDB();

   return i;
}


// ACD/RingGroup
//ACD Data
int DBAccess::readACDData(QList<ACD *> &acds)
{
    //   setDBInfo();
       ACD *acd;
       QString selqry;

       if (db.isOpen() == false)
       {
           qDebug() << "Db Is not Opened";

           if (openDB() == false)
           {
               qDebug() << "Unable to open: " << db.lastError();
               return -1;
           }
           else
           {
               qDebug() << "Db Opened successfully";

           }

       }
       else
       {
           qDebug() << "Db Already Opened.";

       }

       selqry = "SELECT name, ringinuse, strategy FROM queues";

       QSqlQuery *qry = new QSqlQuery(db);

       qry->prepare(selqry);


       if (!qry->exec())
       {
           qDebug() << "Error in select:" <<db.lastError();
           return -1;
       }

       int i=0;
       acds.empty();

       while(qry->next())
       {
           acd = new ACD;

           acd->name = QVariant(qry->value(0)).toString();
           acd->ringInUse = QVariant(qry->value(1)).toString();
           acd->strategy = QVariant(qry->value(2)).toString();

           acds.insert(i, acd);
           i++;
       }

       qry->finish();
       delete qry;

       closeDB();

       return i;
}


int DBAccess::saveACDData(ACD &acd)
{
    QString strQry;

    // open database
    if (db.isOpen() == false)
    {
        qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }

    QSqlQuery qry(db);

    //Insert Data into ps_endpoints
    strQry ="REPLACE INTO queues (name, ringinuse, strategy)  VALUES "
                "( :name, :ringinuse, :strategy)";

    qry.prepare(strQry);

    qry.bindValue(":name", acd.name);
    qry.bindValue(":ringinuse", acd.ringInUse);

    qry.bindValue(":strategy",acd.strategy);
    if (!qry.exec())
    {
        qDebug() << "Error in insert:" << strQry <<"Error:"<<db.lastError();
        return -1;
    }



    return 1;

}

//Ring Group
int DBAccess::readACDMembers(QMap<QString, ACDMember *> &members, QString acd)
{
    //   setDBInfo();
      ACDMemberPro *mProp;
      QString key;
      QString val;
      QString selqry;
      QList<ACDMemberPro *> mProList;

       if (db.isOpen() == false)
       {
           qDebug() << "Db Is not Opened";

           if (openDB() == false)
           {
               qDebug() << "Unable to open: " << db.lastError();
               return -1;
           }
           else
           {
               qDebug() << "Db Opened successfully";

           }

       }
       else
       {
           qDebug() << "Db Already Opened.";

       }

       QSqlQuery qry = QSqlQuery(db);


       if ( acd == nullptr || acd =="")
       {
           selqry = "SELECT queue_name, interface, membername, state_interface, penalty, paused, uniqueid FROM queue_members";
           qry.prepare(selqry);
       }
       else
       {
           selqry = "SELECT queue_name, interface, membername, state_interface, penalty, paused, uniqueid FROM queue_members where queue_name = :queue_name";
           qry.prepare(selqry);
           qry.bindValue(":queue_name", acd);
       }
qDebug() << "Member Query" << getLastExecutedQuery(qry);

       if (!qry.exec())
       {
           qDebug() << "Error in select:"<< getLastExecutedQuery(qry) <<db.lastError();
           return -1;
       }

       int i=0;
       members.empty();
       mProList.empty();

       while(qry.next())
       {
           mProp = new ACDMemberPro;

           key = QVariant(qry.value(0)).toString(); // queue name
           mProp->interface = QVariant(qry.value(1)).toString();
           mProp->memberName = QVariant(qry.value(2)).toString();
           mProp->stateInterface = QVariant(qry.value(3)).toString();
           mProp->penalty = QVariant(qry.value(4)).toInt();
           mProp->paused = QVariant(qry.value(5)).toInt();
           mProList.insert(i, mProp);
           i++;
           qDebug() << "Iterface" << mProp->interface;

       }
       ACDMember *acdMem = new ACDMember;
       acdMem->acdName = key;
       acdMem->member = mProList;
       members.insert(key, acdMem);

       qry.finish();

       closeDB();

// qDebug() << "ACD Count:" << members.count() << "Members:" << i;

       return i;

}


int DBAccess::saveRGData(ACDMember &members)
{
    QString strQry;
    QList<ACDMemberPro *> mProList;
    QString acdName;
    int iCount=0;


    // open database
    if (db.isOpen() == false)
    {
        qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }


    QSqlQuery qry(db);

    acdName = members.acdName;
    mProList = members.member;

    //remove all
    strQry ="DELETE FROM queue_members where queue_name = :queue_name";

    qry.prepare(strQry);
    qry.bindValue(":queue_name", acdName);
    if (!qry.exec())
    {
        qDebug() << "Error in delet:" << getLastExecutedQuery(qry) <<"Error:"<<db.lastError();
        return -1;
    }


    for(iCount=0; iCount < mProList.count(); iCount++)
    {
        // insert into queue_members
        strQry ="INSERT INTO queue_members (queue_name, interface, membername, state_interface, penalty, paused)  VALUES "
                " ( :queue_name, :interface, :membername, :state_interface, :penalty, :paused)";

        qry.prepare(strQry);

        qry.bindValue(":queue_name", acdName);
        qry.bindValue(":interface", mProList.at(iCount)->interface);
        qry.bindValue(":membername", mProList.at(iCount)->memberName);
        qry.bindValue(":state_interface",mProList.at(iCount)->stateInterface);
        qry.bindValue(":penalty",QString::number(mProList.at(iCount)->penalty));
        qry.bindValue(":paused", QString::number(mProList.at(iCount)->paused));

        if (!qry.exec())
        {
            qDebug() << "Error in insert:" << getLastExecutedQuery(qry) <<"Error:"<<db.lastError();
            return -1;
        }

    }

    return 1;

}

///////////// Routing table query
int DBAccess::readRoutingTable(RoutingTable &rt)
{
    QString selqry;
    // get all routing names
    QList<QString> routes;
    QList<RTData *> *rtData;
    RTData *rTbl;
    RouteTable *routeTable;

    // open database
    if (db.isOpen() == false)
    {
        qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }

    QSqlQuery qry = QSqlQuery(db);


    selqry = "SELECT DISTINCT(exten) from extensions order by exten asc";
        qry.prepare(selqry);

//qDebug() << "Member Query" << getLastExecutedQuery(qry);

    if (!qry.exec())
    {
        qry.finish();
        closeDB();

        qDebug() << "Error in select:"<< getLastExecutedQuery(qry) <<db.lastError();
        return -1;
    }

    rt.empty();

    QString key;
    int i=0;
    while(qry.next())
    {
        key = QVariant(qry.value(0)).toString(); // queue name
//        rtData = new.empty();
//qDebug() << "Key:"<<key;

        // search all the entries
        QSqlQuery subqry = QSqlQuery(db);


        selqry = "SELECT context, exten, priority,app, appdata from extensions where exten=:exten order by priority asc";
        subqry.prepare(selqry);
        subqry.bindValue(":exten", key);

//    qDebug() << "Member Query" << getLastExecutedQuery(subqry);

        if (!subqry.exec())
        {
            qry.finish();
            subqry.finish();

            closeDB();
            qDebug() << "Error in select:"<< getLastExecutedQuery(subqry) <<db.lastError();
            return -1;
        }

        rtData = new QList<RTData *>;
        int iCount=0;
        while(subqry.next())
        {
            rTbl = new RTData;
            rTbl->context = QVariant(subqry.value(0)).toString();
            rTbl->DID = QVariant(subqry.value(1)).toString();
            rTbl->priority = QVariant(subqry.value(2)).toInt();
            rTbl->action = QVariant(subqry.value(3)).toString();
            rTbl->destination = QVariant(subqry.value(4)).toString();
            rtData->insert(iCount, rTbl);
            iCount++;
            qDebug() << "Route:"<< rTbl->DID << "no of items:"<< rtData->length();
        }
        if (iCount > 0)
        {
           routeTable = new RouteTable;
           routeTable->data = *rtData;
           rt.insert(key,routeTable);
        }
        i++;
        subqry.finish();
    }

    qry.finish();

    closeDB();
    return i;

}

int DBAccess::saveRoutingTable(RTData &rd)
{
    QString exten;

    // open database
    if (db.isOpen() == false)
    {
        qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }

    QSqlQuery qry = QSqlQuery(db);

    QString qrySel = "SELECT exten from extensions where context = :context AND exten = :exten and priority = :priority";

    qry.prepare(qrySel);

    qry.bindValue(":context", rd.context);
    qry.bindValue(":exten", rd.DID);
    qry.bindValue(":priority", rd.priority);
//qDebug() << "Select qry:" << getLastExecutedQuery(qry);
    if (!qry.exec())
    {
        qry.finish();
        qry.clear();
        closeDB();
        qDebug() << "Error in select:"<< getLastExecutedQuery(qry) <<db.lastError();
        return -1;
    }

    if ( qry.next())
    {
        exten = QVariant(qry.value(0)).toString();
        qDebug() << "Exten:" << exten << "DID" << rd.DID;
        if ( exten == rd.DID)
        {
            qry.finish();
            qry.clear();
            closeDB();

            return -2;
        }
    }

    // now we can add the record
    qry.clear();
    qry.finish();

    qrySel = "INSERT INTO extensions (context, exten, priority, app, appdata) VALUES (:context, :exten, :priority, :app, :appdata)";

    qry.prepare(qrySel);

    qry.bindValue(":context", rd.context);
    qry.bindValue(":exten", rd.DID);
    qry.bindValue(":priority", rd.priority);
    qry.bindValue(":app", rd.action);
    qry.bindValue(":appdata", rd.destination);

    qDebug() << "Error in select:"<< getLastExecutedQuery(qry) <<db.lastError();

    if ( !qry.exec())
    {
        qry.finish();
        qry.clear();
        closeDB();
        return -3;
    }

    qry.finish();
    closeDB();

    return 1;


}


int DBAccess::deleteRoutingTable(RTData &rd)
{
    QString exten;

    // open database
    if (db.isOpen() == false)
    {
        qDebug() << "Db Is not Opened";

        if (openDB() == false)
        {
            qDebug() << "Unable to open: " << db.lastError();
            return -1;
        }
        else
        {
            qDebug() << "Db Opened successfully";

        }

    }
    else
    {
        qDebug() << "Db Already Opened.";

    }

    QSqlQuery qry = QSqlQuery(db);

    QString qrySel = "DELETE FROM extensions where context = :context AND exten = :exten and priority = :priority";

    qry.prepare(qrySel);

    qry.bindValue(":context", rd.context);
    qry.bindValue(":exten", rd.DID);
    qry.bindValue(":priority", rd.priority);
qDebug() << "Select qry:" << getLastExecutedQuery(qry);
    if (!qry.exec())
    {
        qry.finish();
        qry.clear();
        closeDB();
        qDebug() << "Error in select:"<< getLastExecutedQuery(qry) <<db.lastError();
        return -1;
    }

    qry.finish();
    closeDB();

    return 1;


}
