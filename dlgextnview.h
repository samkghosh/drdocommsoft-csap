#ifndef DLGEXTNVIEW_H
#define DLGEXTNVIEW_H

#include "dlgextensionconf.h"

#include <QDialog>

namespace Ui {
class DlgExtnView;
}

class DlgExtnView : public QDialog
{
    Q_OBJECT

public:
    explicit DlgExtnView(QWidget *parent = nullptr);
    ~DlgExtnView();
    void displayExtension();

private slots:
    void on_pbAdd_clicked();
    void cellDoubleClicked(int row, int col);

    void on_lineEdit_textChanged(const QString &arg1);

    void onExtensionModified();
private:
    Ui::DlgExtnView *ui;
    DlgExtensionConf *dlgExtensionConf;
    QMap<QString, Extension*> extensions;
};

#endif // DLGEXTNVIEW_H
