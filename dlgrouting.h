#ifndef DLGROUTING_H
#define DLGROUTING_H
#include <QtGui>
#include <QTreeWidget>
#include <QStyledItemDelegate>
#include "dbaccess.h"
#include "dlgaddroute.h"

#include <QDialog>

namespace Ui {
class DlgRouting;
}

class DlgRouting : public QDialog
{
    Q_OBJECT

public:
    explicit DlgRouting(QWidget *parent = nullptr);
    ~DlgRouting();
    void displyRoutingData();

private slots:
    void on_pbAdd_clicked();

    void on_pbCancel_clicked();

private:
    Ui::DlgRouting *ui;

    DlgAddRoute *dlgAddRout=nullptr;
    RoutingTable routeData;

    QTreeWidgetItem * addRoot(QString name);
    void addChild(QTreeWidgetItem *parent, RTData *rt);

};


class GridDelegate : public QStyledItemDelegate
{
public:
    explicit GridDelegate(QObject * parent = nullptr) : QStyledItemDelegate(parent) { }

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
    {
       painter->setPen(QColor(Qt::black)); // set ink to black
        painter->drawRect(option.rect);  // draw a rect covering cell area
        QStyledItemDelegate::paint(painter, option, index);  // tell it to draw as normally after
    }
};


#endif // DLGROUTING_H
