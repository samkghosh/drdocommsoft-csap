#ifndef PLAYER_H
#define PLAYER_H
#include <QMultimedia>

//#include "videowidget.h"

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class QAbstractItemView;
class QLabel;
class QMediaPlayer;
class QModelIndex;
class QPushButton;
class QSlider;
class QVideoProbe;
class QVideoWidget;
class QAudioProbe;

class PlaylistModel;
class HistogramWidget;

class Player : public QWidget
{
    Q_OBJECT

public:
    Player(QWidget *parent = 0);
    ~Player();

    bool isPlayerAvailable() const;

    void addToPlaylist(const QList<QUrl> urls);

signals:
    void fullScreenChanged(bool fullScreen);
private slots:
    void open();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void metaDataChanged();

    void previousClicked();

    void seek(int seconds);
    void jump(const QModelIndex &index);
    void playlistPositionChanged(int);

    void statusChanged(QMediaPlayer::MediaStatus status);
    void stateChanged(QMediaPlayer::State state);
    void bufferingProgress(int progress);
    void videoAvailableChanged(bool available);

    void displayErrorMessage();

    void showColorDialog();
private:
    void clearHistogram();
    void setTrackInfo(const QString &info);
    void setStatusInfo(const QString &info);
    void handleCursor(QMediaPlayer::MediaStatus status);
    void updateDurationInfo(qint64 currentInfo);

    QMediaPlayer *player;
    QMediaPlaylist *playlist;
  //  VideoWidget *videoWidget;
    QLabel *coverLabel;
    QSlider *slider;
    QLabel *labelDuration;
    QPushButton *fullScreenButton;
    QPushButton *colorButton;
    QDialog *colorDialog;

    QLabel *labelHistogram;
    HistogramWidget *videoHistogram;
    HistogramWidget *audioHistogram;
    QVideoProbe *videoProbe;
    QAudioProbe *audioProbe;
    PlaylistModel *playlistModel;
        QAbstractItemView *playlistView;
        QString trackInfo;
        QString statusInfo;
        qint64 duration;

};
#endif // PLAYER_H
