//#####################################################################
//FileName: mainwindows.cpp
// Purpose: This is the main file which displayes all the menus and calls
//          other submenus.
// Aug-2020
//Author: Samir Ghosh
//#########################################################################

#include <QMessageBox>

//#include <QSettings>
//#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dlgextnview.h"
#include "dlgacd.h"
#include "dlgrec.h"
#include "dlgacd.h"



//################# main window program####################################
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->pbTrunk->hide();
    ui->pbRingGroup->hide();


    setWindowTitle("Communication Software Configuration Panel");

    // Set background image
    QPixmap bkgnd("/etc/csap/mainmenu_bg.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio); 
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}


//############## Destructor######################
MainWindow::~MainWindow()
{
    if (dlgCDR != nullptr)
    {
        dlgCDR->close();
        delete dlgCDR;
    }
    if ( dlgRec != nullptr)
    {
        dlgRec->close();
        delete dlgRec;
    }

    if ( dlgACD != nullptr)
    {
        dlgACD->close();
        delete dlgACD;
    }
    if (dlgRouting == nullptr)
    {
        dlgRouting->close();
        delete dlgRouting;
    }
    if (dlgExtnView == nullptr)
    {
        dlgExtnView->close();
        delete dlgExtnView;
    }

    delete ui;
}




void MainWindow::on_extnActionView_triggered()
{
    if ( dlgExtnView == nullptr)
    {
        dlgExtnView = new DlgExtnView();
    }
    ui->statusbar->showMessage("View Extension");

    dlgExtnView->show();
}

void MainWindow::on_pbExtn_clicked()
{
    if (dlgExtnView == nullptr)
    {
        dlgExtnView = new DlgExtnView();
    }
    dlgExtnView->displayExtension();
    dlgExtnView->show();

}

void MainWindow::on_pbACD_clicked()
{
    if ( dlgACD == nullptr)
    {
        dlgACD = new DlgACD();
    }
    dlgACD->getAndShowValues();
    dlgACD->show();
}

void MainWindow::on_pbCDR_clicked()
{

    if (dlgCDR == nullptr)
    {
        dlgCDR = new DlgCDR();
    }
    dlgCDR->setModal(true);
    dlgCDR->displayCDR();
    dlgCDR->show();
}

void MainWindow::on_pbRec_clicked()
{
    if( dlgRec == nullptr)
    {
        dlgRec = new DlgRec;
    }
    dlgRec->setModal(true);
    dlgRec->displayRecording();

    dlgRec->show();
}


void MainWindow::on_pbCallRouting_clicked()
{
    if ( dlgRouting == nullptr)
    {
        dlgRouting = new DlgRouting();
    }
    dlgRouting->setModal(true);
    dlgRouting->displyRoutingData();
    dlgRouting->show();
}

void MainWindow::on_pbExit_clicked()
{
    close();
}
