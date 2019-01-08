#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include "audiorecorderthread.h"

using namespace std;

namespace EdenClass
{
    class AudioRecorder : public QObject
    {
        Q_OBJECT
    public:
        AudioRecorder();
        ~AudioRecorder();
        void SetParams(int SampleRate, int SampleSize, int Channels);
        void SetParams(int SampleRate, int SampleSize, int Channels, int AudioBufSize, int AudioBufInterval);
        void RecordStart();
        void RecordStop();
        void BufferFlush();
        int GetAudioRemaining();
        char *GetFromBuffer8(int BufLen, char *TempBuf);
        short *GetFromBuffer16(int BufLen, short *TempBuf);
        int *GetPossibleSampleRates();
        void FillAudioBuffer();
        string *GetDeviceList();
        bool SelectDevice(string DeviceName);
        bool SelectDevice(int DeviceNo);
        int StereoMode;
    private:
        QThread * ART_;
        AudioRecorderThread * ART;
        bool ForceStereo = false;
    };
}

#endif // AUDIORECORDER_H
