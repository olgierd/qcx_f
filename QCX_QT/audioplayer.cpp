#include "audioplayer.h"

namespace EdenClass
{
    AudioPlayer::AudioPlayer()
    {
        APT = new AudioPlayerThread();
        APT_ = new QThread(this);
        APT->moveToThread(APT_);
        APT_->start();
    }

    AudioPlayer::~AudioPlayer()
    {
        APT->deleteLater();
        APT_->quit();
        APT_->wait(5000);
    }

    int *AudioPlayer::GetPossibleSampleRates()
    {
        return APT->GetPossibleSampleRates();
    }

    int AudioPlayer::GetAudioRemaining()
    {
        return APT->GetAudioRemaining();
    }

    void AudioPlayer::SetParams(int SampleRate, int SampleSize, int Channels)
    {
        int XXX = SampleRate * SampleSize * Channels;
        APT->SetParams(SampleRate, SampleSize, Channels, XXX / 2, 100);
    }

    void AudioPlayer::SetParams(int SampleRate, int SampleSize, int Channels, int AudioBufSize, int AudioBufInterval)
    {
        APT->SetParams(SampleRate, SampleSize, Channels, AudioBufSize, AudioBufInterval);
    }

    void AudioPlayer::PutToBuffer(char *Buf, int BufLen)
    {
        APT->PutToBuffer(Buf, BufLen);
    }

    void AudioPlayer::PutToBuffer(short *Buf, int BufLen)
    {
        APT->PutToBuffer(Buf, BufLen);
    }

    void AudioPlayer::PutSilenceToBuffer(int Len)
    {
        APT->PutSilenceToBuffer(Len);
    }

    void AudioPlayer::PlayStart()
    {
        APT->PlayStart();
    }

    void AudioPlayer::PlayStop()
    {
        APT->PlayStop();
    }

    void AudioPlayer::BufferFlush()
    {
        APT->BufferFlush();
    }

    void AudioPlayer::FillAudioBuffer()
    {
        APT->FillAudioBuffer();
    }

    string *AudioPlayer::GetDeviceList()
    {
        return APT->GetDeviceList();
    }

    bool AudioPlayer::SelectDevice(string DeviceName)
    {
        return APT->SelectDevice(DeviceName);
    }

    bool AudioPlayer::SelectDevice(int DeviceNo)
    {
        return APT->SelectDevice(DeviceNo);
    }
}
