#ifndef DLGACDMEMBER_H
#define DLGACDMEMBER_H

#include <QListWidgetItem>

#include "dbaccess.h"


#include <QDialog>

namespace Ui {
class DlgACDMember;
}

class DlgACDMember : public QDialog
{
    Q_OBJECT

public:
    explicit DlgACDMember(QWidget *parent = nullptr);
    ~DlgACDMember();
    void showMembers(QString name);

public slots:
    void sendExtnToMemebr(QListWidgetItem *item );
    void sendMemberToExtn(QListWidgetItem *item );

private slots:
    void on_pbSave_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DlgACDMember *ui;
    QString acdName;

    QMap<QString,Extension *> extnList;

    QMap<QString, ACDMember *> members;

    void setMembers();
};

#endif // DLGACDMEMBER_H
