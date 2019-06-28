#include "qmedia.h"
#include "ui_qmedia.h"
#include <QFileDialog>
#include <QStringList>
#include <QDebug>
#include <stdlib.h>

#define TIME_1SECOND 1000
#define TIME_100MSECOND 100
QMedia::QMedia(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QMedia)
{
    ui->setupUi(this);
    this->initAll();
}

QMedia::~QMedia()
{
    mplayer->kill();
    delete mplayer;
    delete ui;
}

void QMedia::initAll()
{
    srand(time(NULL));
    mode_value = STOP;
    volume = 80;
    mplayer = new QProcess(this);
    QStringList args;
    args << "-slave"; //使用slave模式
    args << "-quiet"; //不要输出冗余信息
    mplayer->start("mplayer", args); //启动该进程，并传入参数args

    ui->volume_slider->setRange(0,100);

    timer100MsID = this->startTimer(TIME_100MSECOND);

    QDir dir(QString("/mnt/usb/"));
    QStringList nameFilters;
    nameFilters << "*.mp4" << "*.WMV" << "*.mp3" << "*.wav";
    QString prefix = "/home/csdn/Music/";
    QStringList songsList = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    qDebug("%d",songsList.length());
    for(int i = 0; i < songsList.length(); i++)
    {
        QString song = prefix + songsList.at(i);
        music_paths.append(song);
        QString name = song.split("/").last();
        ui->music_list->addItem(name);
    }
    if(songsList.length() != 0)
        setMusic(prefix + songsList.at(0));
}

void QMedia::timerEvent(QTimerEvent *event)
{
//    if(event->timerId() == timer1SID)
//    {
//        mplayerReadMessages();
//    }
    if(event->timerId() == timer100MsID)
    {
        if(ui->start_buttton->text() == tr("pause"))
            mplayerReadMessages();
    }
}

void QMedia::setMusic(QString name)
{
    if(mplayer != NULL)
    {
        mplayer->kill();
    }
    mplayer = new QProcess(this);
    QStringList args;
    if(name.endsWith(".mp4")){
        args << "-slave";
        args << "-quiet";
        args << "-zoom";
        //args << "-x";
        //args << "731";
        //args << "-y";
        //args << "411";
        args << "-wid" << QString::number(ui->frame->winId());  //视频在指定窗口库播放
        //args << "-vo";
        //args << "x11";
        args << name;
    }
    else {

        args << "-slave"; //使用slave模式
        args << "-quiet"; //不要输出冗余信息
        args << name;
    }
    mplayer->start("mplayer", args); //启动该进程，并传入参数args

    ui->volume_slider->setValue(volume);
    ui->volume_button->setText(tr("音量"));
    ui->volume_button->setIcon(QIcon(":/images/volume.png"));
    mplayerWriteCmd("volume "+ QString::number(volume) + " 2\n");
}

void QMedia::mplayerWriteCmd(QString cmd)
{
    if(mplayer == NULL)
    {
        return;
    }
    mplayer->write(cmd.toUtf8());
}

void QMedia::mplayerReadMessages()
{
    mplayerWriteCmd("get_time_pos\n");
    mplayerWriteCmd("get_time_length\n");
    mplayerWriteCmd("get_percent_pos\n");
    while(mplayer->canReadLine())
    {
        QByteArray b=mplayer->readLine();
        if(b.startsWith("ANS_TIME_POSITION"))
        {
            b.replace(QByteArray("\n"),QByteArray(""));
            QString s(b);
            currentStr=s.mid(18).toFloat();

            ui->process_slider->setValue((int)s.mid(18).toFloat());    //更新进度条
        }
        else if((b.startsWith("ANS_LENGTH")))
        {
            b.replace(QByteArray("\n"),QByteArray(""));
            QString s(b);

            totalTime=s.mid(11).toFloat();
            QString currentTime = QString("%1:%2").arg(currentStr/60).arg(currentStr%60);
            QString totalTime_s = QString("%1:%2").arg(totalTime/60).arg(totalTime%60);  //标准格式输出时间
            ui->process_label_1->setText(currentTime);
            ui->process_label_2->setText(totalTime_s); //显示时间进度
            ui->process_slider->setRange(0,(int)s.mid(11).toFloat());
        }
        else if(b.startsWith(("ANS_PERCENT_POSITION")))
        {
            QString s(b);
            QString currentPercent = s.mid(21);
            if(currentPercent == tr("100\n"))
            {
                switch (mode_value) {
                case SHUFFLE:
                    int index;
                    index = rand() % music_paths.length();
                    ui->music_list->setCurrentRow(index);
                    setMusic(music_paths.at(index));
                    break;
                case SEQUENTIAL:
                    index = ui->music_list->currentRow();
                    index = (index + 1) % music_paths.length();
                    ui->music_list->setCurrentRow(index);
                    setMusic(music_paths.at(index));
                    break;
                default:
                    on_stop_button_clicked();//stop
                    break;
                }
            }
        }
    }
}

void QMedia::on_start_buttton_clicked()
{
    mplayer->write("pause\n");
    if(tr("play")==ui->start_buttton->text())
    {
        ui->start_buttton->setText(tr("pause"));
    }
    else
    {
        ui->start_buttton->setText(tr("play"));
    }
}

void QMedia::on_stop_button_clicked()
{
    if(mplayer != NULL)
        mplayer->kill();
    ui->process_label_1->clear();
    ui->process_label_2->clear();
    ui->process_slider->setValue(0);
    ui->start_buttton->setText(tr("play"));
}

void QMedia::on_back_button_clicked()
{
    mplayer->write("seek -1\n");
}

void QMedia::on_volume_button_clicked()
{
    if(ui->volume_button->text()==tr("音量"))
    {
        ui->volume_button->setText(tr("静音"));
        ui->volume_button->setIcon(QIcon(":/images/silence.png"));
        mplayer->write("mute 1\n");
    }
    else
    {
        ui->volume_button->setText(tr("音量"));
        ui->volume_button->setIcon(QIcon(":/images/volume.png"));
        mplayer->write("mute 0\n");
    }
}

void QMedia::on_forward_button_clicked()
{
    mplayer->write("seek 1\n");
}

void QMedia::on_volume_slider_sliderMoved(int position)
{
    if(mplayer == NULL)
    {
        return;
    }
    volume = position;
    mplayerWriteCmd("volume " + QString::number(position) + " 2\n");
}

void QMedia::on_add_button_clicked()
{

    QStringList songs = QFileDialog::getOpenFileNames(this,tr("choose files"),QDir::homePath(),tr("*.wav;;*.mp4;;*.WMV;;*.mp3"));

    for(int i = 0; i < songs.length(); i++)
    {
        QString song = songs.at(i);
        music_paths.append(song);
        QString name = song.split("/").last();
        ui->music_list->addItem(name);
    }
}

void QMedia::on_start_player_button_clicked()
{
    int index=ui->music_list->currentRow();
    if(-1!=index)
    {
        QString name = music_paths.at(index);
        this->setMusic(name);
        ui->start_buttton->setText(tr("pause"));
    }

}

void QMedia::on_mode_button_clicked()
{
    if(ui->mode_button->text()==tr("shuffle"))
    {
        ui->mode_button->setText(tr("seq"));
        mode_value = SHUFFLE;
        //shuffle();
    }
    else
    {
        ui->mode_button->setText(tr("shuffle"));
        mode_value = SEQUENTIAL;
        //sequential();
    }
}
