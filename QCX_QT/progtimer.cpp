#include "progtimer.h"

ProgTimer::ProgTimer(int Interval, EdenClass::AudioRecorder * AR_, EdenClass::AudioPlayer * AP_, QObject *parent) : QObject(parent)
{
    AR = AR_;
    AP = AP_;
    T = new QTimer(this);
    QObject::connect(T, SIGNAL(timeout()), this, SLOT(Event()));
    T->start(Interval);
}

ProgTimer::~ProgTimer()
{
    T->stop();
    QObject::disconnect(T, SIGNAL(timeout()), this, SLOT(Event()));
    T->deleteLater();
}

void ProgTimer::Event()
{
    AR->FillAudioBuffer();
    AP->FillAudioBuffer();
    emit TimerEvent();
}
