#ifndef DLGADDACD_H
#define DLGADDACD_H

#include <QDialog>
#include "dlgacdmember.h"

namespace Ui {
class DlgAddACD;
}

class DlgAddACD : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAddACD(QWidget *parent = nullptr);
    ~DlgAddACD();

    void setAcds(QList <ACD *> acd);
    void showAcdInfo(ACD *acd);


private slots:
    void on_pbCancel_clicked();

    void on_pbSave_clicked();

    void on_pbDel_clicked();

    void on_leName_textChanged(const QString &arg1);

signals:
    void acdModified();
private:
    Ui::DlgAddACD *ui;

    DlgACDMember *dlgACDMembers;

    QList<ACD *> acds;


};
#endif // DLGADDACD_H
