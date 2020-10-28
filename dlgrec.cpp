#include "dlgrec.h"
#include "ui_dlgrec.h"
#include <QMediaPlayer>

DlgRec::DlgRec(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRec)
{
    ui->setupUi(this);
    _player = new QMediaPlayer(this);
    ui->pbStop->setDisabled(true);
    connect(ui->tblRec, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(onItemDoubleClick(QTableWidgetItem *)));

    connect(_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus )));

    displayRecording();
}

DlgRec::~DlgRec()
{
    delete ui;

}


void DlgRec::displayRecording()
{
    int iRow=0;
    int iCol=0;
    QStringList hdr;
    QString key=nullptr;

    DBAccess *dba1 = new DBAccess("rec");

//    ui->tabWidget->setTabEnabled(1, true);
    this->setWindowTitle("Report on Recording");

    int rows = ui->tblRec->rowCount();

    for (int i =0; i < rows; i++)
            ui->tblRec->removeRow(i);

    ui->tblRec->setRowCount(0);

    hdr << "" <<"From Number" << "To Number" << "context" << "Start Time"<< "Duration" << "RecFile";

    ui->tblRec->setColumnCount(7);


    ui->tblRec->setHorizontalHeaderLabels(hdr);
    ui->tblRec->setColumnWidth(0,20);

    qDeleteAll(cdrs.begin(), cdrs.end());
    cdrs.clear();
    int iRecCount = dba1->readCDRData(cdrs, 1);

    delete dba1;
    qDebug() << "CDR Record Count:" << iRecCount;

    iRow = 0;
    for(int i=0; i < cdrs.count(); i++)
    {
        iCol=0;

        QTableWidgetItem *select = new QTableWidgetItem();
        QTableWidgetItem *from = new QTableWidgetItem();
        QTableWidgetItem *to= new QTableWidgetItem();
        QTableWidgetItem *context = new QTableWidgetItem();
        QTableWidgetItem *date = new QTableWidgetItem();
        QTableWidgetItem *duration = new QTableWidgetItem();
        QTableWidgetItem *recFile = new QTableWidgetItem();


        from->setText(cdrs[i]->from);
        to->setText(cdrs[i]->to);
        context->setText(cdrs[i]->context);
        date->setText(cdrs[i]->dateTime.toString());
        duration->setText(QString::number(cdrs[i]->duration));
        recFile->setText(cdrs[i]->recFileName);

        ui->tblRec->insertRow(iRow);
        ui->tblRec->setItem(iRow, iCol++, select);
        select->setCheckState(Qt::Unchecked);

        ui->tblRec->setItem(iRow, iCol++, from);
        ui->tblRec->setItem(iRow, iCol++, to);
        ui->tblRec->setItem(iRow, iCol++, context);
        ui->tblRec->setColumnWidth(iCol,180);
        ui->tblRec->setItem(iRow, iCol++, date);
        ui->tblRec->setItem(iRow, iCol++, duration);
        ui->tblRec->setColumnWidth(iCol,300);
        ui->tblRec->setItem(iRow, iCol++, recFile);

        iRow++;

    }

    ui->lblRecTotal->setText("Total:"+QString::number(iRow));
}


// play
void DlgRec::onItemDoubleClick(QTableWidgetItem *item)
{
    QTableWidgetItem *cellItem;
    int iRow = item->row();

    cellItem = ui->tblRec->item(iRow,6);

    currFile = cellItem->text();

    QUrl file = QUrl::fromLocalFile("/etc/softphone/rt.wav");
                //QFileDialog::getOpenFileName(this, tr("Open Music"), "", tr("")));
    if (file.url() == "")
        return ;
    _player->setMedia(file);
    _player->setVolume(50);
    if ( _player->state() == QMediaPlayer::PlayingState)
    {
        _player->stop();
    }
    ui->pbStop->setDisabled(false);
    _player->play();

}

void DlgRec::on_pushButton_clicked()
{
    _player->stop();
    ui->pbStop->setDisabled(true);
}

void DlgRec::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
  if (status == 6  )
  {
      ui->pbStop->setText("Playing:"+currFile);
      return;
  }
  if (status == 3)
  {
      ui->pbStop->setDisabled(true);
      ui->pbStop->setText("Not Playing now.");
  }
}

void DlgRec::on_pbStop_clicked()
{
    _player->stop();
   // ui->pbStop->setDisabled(true);
//    ui->pbStop->setText("Not playing now");
}
