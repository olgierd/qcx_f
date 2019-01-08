#ifndef PROGTIMER_H
#define PROGTIMER_H

#include <QObject>
#include <QTime>
#include <QDebug>
#include <QTimer>
#include "audiorecorder.h"
#include "audioplayer.h"

class ProgTimer : public QObject
{
    Q_OBJECT
public:
    explicit ProgTimer(int Interval, EdenClass::AudioRecorder * AR_, EdenClass::AudioPlayer * AP_, QObject *parent = 0);
    ~ProgTimer();
    QTimer * T;
    EdenClass::AudioRecorder * AR;
    EdenClass::AudioPlayer * AP;
signals:
    void TimerEvent();
public slots:
    void Event();
};

#endif // PROGTIMER_H
