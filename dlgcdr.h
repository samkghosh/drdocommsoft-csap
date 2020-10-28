#ifndef DLGCDR_H
#define DLGCDR_H

#include "dbaccess.h"
#include <QDialog>
#include <QTableWidgetItem>


namespace Ui {
class DlgCDR;
}

class DlgCDR: public QDialog
{
    Q_OBJECT

public:
    explicit DlgCDR(QWidget *parent = nullptr);
    ~DlgCDR();
    void displayCDR();
private:
    Ui::DlgCDR *ui;
    QList<CDR *> cdrs;
};

#endif // DLGREPORT_H
