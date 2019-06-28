#ifndef QMEDIA_H
#define QMEDIA_H

#include <QMainWindow>
#include <QProcess>
#include <QTimerEvent>
namespace Ui {
class QMedia;
}

class QMedia : public QMainWindow
{
    Q_OBJECT

public:
    explicit QMedia(QWidget *parent = NULL);
    ~QMedia();
    void initAll();
    void mplayerWriteCmd(QString cmd);
    void setMusic(QString name);
    virtual void timerEvent(QTimerEvent *event);//定时器事件
    void mplayerReadMessages();
private slots:
    void on_start_buttton_clicked();

    void on_stop_button_clicked();

    void on_back_button_clicked();

    void on_volume_button_clicked();

    void on_forward_button_clicked();

    void on_volume_slider_sliderMoved(int position);

    void on_add_button_clicked();

    void on_start_player_button_clicked();

    void on_mode_button_clicked();

private:
    Ui::QMedia *ui;

private:
    QProcess *mplayer;
    QStringList music_paths;
    enum mode{SHUFFLE=0,SEQUENTIAL=1,STOP=2};
    int mode_value;
    int volume;
    int timer1SID;
    int timer100MsID;
    int currentStr;
    int totalTime;

};

#endif // QMEDIA_H
