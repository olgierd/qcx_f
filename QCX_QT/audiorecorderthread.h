#ifndef AUDIORECORDERTHREAD_H
#define AUDIORECORDERTHREAD_H

#include <QtGui>
#include <QIODevice>
#include <QAudioInput>
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
    class AudioRecorderThread : public QObject
    {
        Q_OBJECT
    public:
        AudioRecorderThread();
        ~AudioRecorderThread();
        void SetParams(int SampleRate, int SampleSize, int Channels, int AudioBufSize, int AudioBufInterval);
        void RecordStart();
        void RecordStop();
        void BufferFlush();
        int GetAudioRemaining();
        char *GetFromBuffer8(int BufLen, char *TempBuf);
        short *GetFromBuffer16(int BufLen, short *TempBuf);
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
        queue<QByteArray> BufQueue;
        QAudioInput* mAudioInput;
        QIODevice* mIOInput;
        bool Working;
        int BufSize;
        int NotifyInterval;
        int AudioPointer;
        int TotalLength;
        void RemoveHeapObjects(bool Aud);
        char *TempBuf8 = NULL;
        int TempBuf8L = 0;
    public slots:
        void FillAudioBuffer();
    };
}

#endif // AUDIORECORDERTHREAD_H
