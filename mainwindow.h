#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "dlgextensionconf.h"
#include "dlgrec.h"
#include "dlgcdr.h"
#include "dlgrouting.h"
#include "dlgacd.h"
#include "dlgextnview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_extnActionView_triggered();

    void on_pbExtn_clicked();

    void on_pbACD_clicked();

    void on_pbCDR_clicked();

    void on_pbRec_clicked();

    void on_pbCallRouting_clicked();

    void on_pbExit_clicked();

private:
    Ui::MainWindow *ui;
    DlgCDR *dlgCDR = nullptr;
    DlgRec *dlgRec = nullptr;
    DlgACD *dlgACD = nullptr;
    DlgRouting *dlgRouting = nullptr;
    DlgExtnView *dlgExtnView = nullptr;
};
#endif // MAINWINDOW_H
