#ifndef AUDIOPLAYERTHREAD_H
#define AUDIOPLAYERTHREAD_H

#include <QtGui>
#include <QIODevice>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QTime>
#include <stdio.h>
#include <queue>
#include <iostream>
#include <mutex>
#include <string>

using namespace std;

namespace EdenClass
{
    class AudioPlayerThread : public QObject
    {
        Q_OBJECT
    public:
        AudioPlayerThread();
        ~AudioPlayerThread();
        void SetParams(int SampleRate, int SampleSize, int Channels, int AudioBufSize, int AudioBufInterval);
        void PlayStart();
        void PlayStop();
        void BufferFlush();
        void PutToBuffer(char* Buf, int BufLen);
        void PutToBuffer(short* Buf, int BufLen);
        int GetAudioRemaining();
        void PutSilenceToBuffer(int Len);
        int *GetPossibleSampleRates();
        string *GetDeviceList();
        bool SelectDevice(string DeviceName);
        bool SelectDevice(int DeviceNo);
    private:
        string *DeviceList;
        int DeviceListN;
        int DeviceListCurrent;
        mutex Buffer_Mutex;
        int SizeFactor;
        bool Working;
        int BufSize;
        queue<QByteArray> BufQueue;
        void RemoveHeapObjects(bool Aud);
        QAudioOutput* mAudioOutput;
        QIODevice* mIOOutput;
        int NotifyInterval;
        int AudioPointer;
        int TotalLength;
    public slots:
        void FillAudioBuffer();
    };
}

#endif // AUDIOPLAYERTHREAD_H
