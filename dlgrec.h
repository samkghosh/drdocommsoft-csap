#ifndef DLGREC_H
#define DLGREC_H

#include "dbaccess.h"

#include <QDialog>

#include <QTableWidgetItem>

#include <QMediaPlayer>

namespace Ui {
class DlgRec;
}

class DlgRec: public QDialog
{
    Q_OBJECT

public:
    explicit DlgRec(QWidget *parent = nullptr);
    ~DlgRec();
    void displayRecording();
private slots:
    void onItemDoubleClick(QTableWidgetItem *x);

    void on_pushButton_clicked();

    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

    void on_pbStop_clicked();

private:
    Ui::DlgRec*ui;
    QList<CDR *> cdrs;
    QMediaPlayer *_player;
    QString currFile;
};

#endif // DLGREPORT_H
