#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "audioplayerthread.h"

using namespace std;

namespace EdenClass
{
    class AudioPlayer : public QObject
    {
        Q_OBJECT
    public:
        AudioPlayer();
        ~AudioPlayer();
        int *GetPossibleSampleRates();
        void SetParams(int SampleRate, int SampleSize, int Channels);
        void SetParams(int SampleRate, int SampleSize, int Channels, int AudioBufSize, int AudioBufInterval);
        void PlayStart();
        void PlayStop();
        void BufferFlush();
        void PutToBuffer(char* Buf, int BufLen);
        void PutToBuffer(short* Buf, int BufLen);
        int GetAudioRemaining();
        void PutSilenceToBuffer(int Len);
        void FillAudioBuffer();
        string *GetDeviceList();
        bool SelectDevice(string DeviceName);
        bool SelectDevice(int DeviceNo);
    private:
        QThread * APT_;
        AudioPlayerThread * APT;
    };
}

#endif // AUDIOPLAYER_H
