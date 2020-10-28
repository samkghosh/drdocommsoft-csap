#ifndef DLGADDROUTE_H
#define DLGADDROUTE_H
#include <dbaccess.h>
#include <QDialog>

namespace Ui {
class DlgAddRoute;
}

class DlgAddRoute : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAddRoute(QWidget *parent = nullptr);
    ~DlgAddRoute();
    void setRoutingData(RoutingTable *routeData);
private slots:
    void on_pbSave_clicked();

    void on_pbCancel_clicked();

    void on_leDID_textChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::DlgAddRoute *ui;
    RoutingTable *routeData;
    void showRouteData(QList<RTData *>*rtd);

};

#endif // DLGADDROUTE_H
