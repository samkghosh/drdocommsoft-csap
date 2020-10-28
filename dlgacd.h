#ifndef DLGACD_H
#define DLGACD_H

#include <QDialog>
#include <QTableWidgetItem>
#include "dbaccess.h"
#include "dlgaddacd.h"

namespace Ui {
class DlgACD;
}

class DlgACD : public QDialog
{
    Q_OBJECT

public:
    explicit DlgACD(QWidget *parent = nullptr);
    ~DlgACD();
    void onTableClick(const QModelIndex &Index);
    void getAndShowValues();

private slots:
    void on_tblACDView_cellClicked(int row, int column);

    void on_pbAdd_clicked();
//    void onItemDoubleClick(QTableWidgetItem *x);
    void onAcdModified();
private:
    Ui::DlgACD *ui;

    QList<ACD *> acds;

    DlgAddACD * dlgAddACD;
    DlgACDMember *dlgACDMembers;
};

#endif // DLGACD_H
