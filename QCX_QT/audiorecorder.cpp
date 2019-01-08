#include "audiorecorder.h"

namespace EdenClass
{
    AudioRecorder::AudioRecorder()
    {
        StereoMode = 0;
        ART = new AudioRecorderThread();
        ART_ = new QThread(this);
        ART->moveToThread(ART_);
        ART_->start();
    }

    AudioRecorder::~AudioRecorder()
    {
        ART->deleteLater();
        ART_->quit();
        ART_->wait(5000);
    }

    int *AudioRecorder::GetPossibleSampleRates()
    {
        return ART->GetPossibleSampleRates();
    }

    void AudioRecorder::SetParams(int SampleRate, int SampleSize, int Channels)
    {
        if ((StereoMode > 0) && (Channels == 1))
        {
            Channels = 2;
            ForceStereo = true;
        }
        else
        {
            ForceStereo = false;
        }
        int XXX = SampleRate * SampleSize * Channels;
        ART->SetParams(SampleRate, SampleSize, Channels, XXX / 2, 100);
    }

    void AudioRecorder::SetParams(int SampleRate, int SampleSize, int Channels, int AudioBufSize, int AudioBufInterval)
    {
        if ((StereoMode > 0) && (Channels == 1))
        {
            Channels = 2;
            ForceStereo = true;
        }
        else
        {
            ForceStereo = false;
        }
        ART->SetParams(SampleRate, SampleSize, Channels, AudioBufSize, AudioBufInterval);
    }

    void AudioRecorder::RecordStart()
    {
        ART->RecordStart();
    }

    void AudioRecorder::RecordStop()
    {
        ART->RecordStop();
    }

    int AudioRecorder::GetAudioRemaining()
    {
        return ART->GetAudioRemaining();
    }

    char *AudioRecorder::GetFromBuffer8(int BufLen, char *TempBuf)
    {
        if (ForceStereo)
        {
            if (TempBuf == NULL)
            {
                TempBuf = new char[BufLen << 1];
            }
            TempBuf = ART->GetFromBuffer8(BufLen, TempBuf);
            switch (StereoMode)
            {
                case 1:
                {
                    for (int I = 0; I < BufLen; I++)
                    {
                        TempBuf[I] = ((TempBuf[I << 1]) >> 1) + ((TempBuf[(I << 1) + 1]) >> 1);
                    }
                    break;
                }
                case 2:
                {
                    for (int I = 0; I < BufLen; I++)
                    {
                        TempBuf[I] = ((TempBuf[I << 1]) >> 1) - ((TempBuf[(I << 1) + 1]) >> 1);
                    }
                    break;
                }
                case 3:
                {
                    for (int I = 0; I < BufLen; I++)
                    {
                        TempBuf[I] = (TempBuf[I << 1]);
                    }
                    break;
                }
                case 4:
                {
                    for (int I = 0; I < BufLen; I++)
                    {
                        TempBuf[I] = (TempBuf[(I << 1) + 1]);
                    }
                    break;
                }
            }
            return TempBuf;
        }
        else
        {
            return ART->GetFromBuffer8(BufLen, TempBuf);
        }
    }

    short *AudioRecorder::GetFromBuffer16(int BufLen, short *TempBuf)
    {
        if (ForceStereo)
        {
            if (TempBuf == NULL)
            {
                TempBuf = new short[BufLen << 1];
            }
            TempBuf = ART->GetFromBuffer16(BufLen, TempBuf);
            switch (StereoMode)
            {
                case 1:
                {
                    for (int I = 0; I < BufLen; I++)
                    {
                        TempBuf[I] = (TempBuf[I << 1] >> 1) + (TempBuf[(I << 1) + 1] >> 1);
                    }
                    break;
                }
                case 2:
                {
                    for (int I = 0; I < BufLen; I++)
                    {
                        TempBuf[I] = (TempBuf[I << 1] >> 1) - (TempBuf[(I << 1) + 1] >> 1);
                    }
                    break;
                }
                case 3:
                {
                    for (int I = 0; I < BufLen; I++)
                    {
                        TempBuf[I] = TempBuf[I << 1];
                    }
                    break;
                }
                case 4:
                {
                    for (int I = 0; I < BufLen; I++)
                    {
                        TempBuf[I] = TempBuf[(I << 1) + 1];
                    }
                    break;
                }
            }
            return TempBuf;
        }
        else
        {
            return ART->GetFromBuffer16(BufLen, TempBuf);
        }
    }

    void AudioRecorder::BufferFlush()
    {
        ART->BufferFlush();
    }

    void AudioRecorder::FillAudioBuffer()
    {
        ART->FillAudioBuffer();
    }

    string *AudioRecorder::GetDeviceList()
    {
        return ART->GetDeviceList();
    }

    bool AudioRecorder::SelectDevice(string DeviceName)
    {
        return ART->SelectDevice(DeviceName);
    }

    bool AudioRecorder::SelectDevice(int DeviceNo)
    {
        return ART->SelectDevice(DeviceNo);
    }
}
