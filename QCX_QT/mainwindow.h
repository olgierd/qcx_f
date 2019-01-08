#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "audiorecorder.h"
#include "audioplayer.h"
#include "eden.h"
#include <string>
#include "progtimer.h"
#include "wavefile.h"
#include "stopwatch.h"
#include <QSerialPort>
#include "configfile.h"
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    EdenClass::AudioRecorder * AR;
    EdenClass::AudioPlayer * AP;
    string * DevList;
    int * SmpList;
    QThread * ProgTimer__ = NULL;
    ProgTimer * ProgTimer_ = NULL;
    int SampleRate;
    EdenClass::WaveFile * WF;
    EdenClass::StopWatch SW;
    QSerialPort SP;
    bool SPOpen;


    // variables for audio processing
    int i=1, rec = 0, reject=0, empty=0, symbol = 0, newqrg = 0;
    double last = -1;
    double diff=0, first=0;
    double freq=0, lastfreq=0;
    char message[10];

    void closeEvent(QCloseEvent *event);


private slots:
    void on_WorkStart_clicked();

    void on_WorkStop_clicked();

    void AudioTimerTick();

    void on_SerialTestB_clicked();

private:
    Ui::MainWindow *ui;
    bool Info1;
    bool Info2;
    bool Info3;
    void Print(int MsgType, string X);
    void SerialOpen();
    void SerialMsg(string Msg);
    void SerialClose();
    string SerialLastMsg = "";
};

#endif // MAINWINDOW_H
