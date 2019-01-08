#include "audioplayerthread.h"

namespace EdenClass
{
    AudioPlayerThread::AudioPlayerThread()
    {
        mAudioOutput = 0;
        mIOOutput = 0;
        Working = false;
        TotalLength = 0;
        AudioPointer = 0;

        QList<QAudioDeviceInfo> DevInfo = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
        DeviceList = new string[DevInfo.length() + 2];
        for (int I = 0; I < DevInfo.length(); I++)
        {
            DeviceList[I + 1] = DevInfo[I].deviceName().toUtf8().constData();
            if (DeviceList[I + 1] == "")
            {
                DeviceList[I + 1] = "Device " + to_string(I);
            }
        }
        DeviceList[0] = "Default";
        DeviceList[DevInfo.length() + 1] = "";
        DeviceListN = DevInfo.length() + 1;
        DeviceListCurrent = 0;
    }

    AudioPlayerThread::~AudioPlayerThread()
    {
        RemoveHeapObjects(true);
        delete[] DeviceList;
    }

    ///
    /// \brief AudioPlayerThread::RemoveHeapObjects - Removing existing objects on heap
    /// \param Aud
    ///
    void AudioPlayerThread::RemoveHeapObjects(bool Aud)
    {
        Working = false;
        if (Aud)
        {
            if (mAudioOutput != 0)
            {
                delete mAudioOutput;
                mAudioOutput = 0;
            }
        }
        if (mIOOutput != 0)
        {
            delete mIOOutput;
            mIOOutput = 0;
        }
    }

    ///
    /// \brief AudioPlayerThread::GetPossibleSampleRates - Getting all possible sample rates
    /// \return
    ///
    int *AudioPlayerThread::GetPossibleSampleRates()
    {
        QList<int> XXX;
        if (DeviceListCurrent == 0)
        {
            QAudioDeviceInfo QAFI(QAudioDeviceInfo::defaultOutputDevice());
            XXX = QAFI.supportedSampleRates();
        }
        else
        {
            QAudioDeviceInfo QAFI(QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)[DeviceListCurrent - 1]);
            XXX = QAFI.supportedSampleRates();
        }
        int X = XXX.size();
        int * XX = new int[X + 1];
        for (int I = 0; I < X; I++)
        {
            XX[I] = XXX[I];
        }
        XX[X] = 0;
        return XX;
    }

    ///
    /// \brief AudioPlayerThread::GetAudioRemaining - Returning count of samples remaining in buffer
    /// \return
    ///
    int AudioPlayerThread::GetAudioRemaining()
    {
        Buffer_Mutex.lock();
        int X = ((TotalLength - AudioPointer) / SizeFactor);
        Buffer_Mutex.unlock();
        return X;
    }

    ///
    /// \brief AudioPlayerThread::SetParams - Setting playing parameters including buffer size, timer interval and minimal buffer filling
    /// \param SampleRate
    /// \param SampleSize
    /// \param Channels
    /// \param AudioBufSize
    /// \param AudioBufInterval
    ///
    void AudioPlayerThread::SetParams(int SampleRate, int SampleSize, int Channels, int AudioBufSize, int AudioBufInterval)
    {
        RemoveHeapObjects(true);

        SizeFactor = Channels * SampleSize;
        BufSize = AudioBufSize;
        NotifyInterval = AudioBufInterval;

        QAudioFormat QAF;
        QAF.setChannelCount(Channels);
        QAF.setSampleRate(SampleRate);
        QAF.setSampleSize(SampleSize * 8);
        QAF.setCodec("audio/pcm");
        QAF.setByteOrder(QAudioFormat::LittleEndian);
        QAF.setSampleType(QAudioFormat::SignedInt);
        if (DeviceListCurrent == 0)
        {
            QAudioDeviceInfo QAFI(QAudioDeviceInfo::defaultOutputDevice());
            if (!QAFI.isFormatSupported(QAF))
            {
                QAF = QAFI.nearestFormat(QAF);
            }
            mAudioOutput = new QAudioOutput(QAFI, QAF, 0);
        }
        else
        {
            QAudioDeviceInfo QAFI(QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)[DeviceListCurrent - 1]);
            if (!QAFI.isFormatSupported(QAF))
            {
                QAF = QAFI.nearestFormat(QAF);
            }
            mAudioOutput = new QAudioOutput(QAFI, QAF, 0);
        }
        mAudioOutput->setBufferSize(BufSize * SizeFactor);
        if (NotifyInterval > 0)
        {
            mAudioOutput->setNotifyInterval(NotifyInterval);
            mAudioOutput->blockSignals(false);
            QObject::connect(mAudioOutput, SIGNAL(notify()), this, SLOT(FillAudioBuffer()));
        }
        else
        {
            mAudioOutput->blockSignals(true);
        }
    }

    ///
    /// \brief AudioPlayerThread::FillAudioBuffer - Filling audio buffer
    ///
    void AudioPlayerThread::FillAudioBuffer()
    {
        Buffer_Mutex.lock();
        if (Working)
        {
            int ChunkSize = mAudioOutput->bytesFree();
            if (ChunkSize > TotalLength)
            {
                ChunkSize = TotalLength;
            }
            ChunkSize = ChunkSize - (ChunkSize % SizeFactor);

            QByteArray TempBuf(ChunkSize, 0);
            if (ChunkSize > 0)
            {
                if (BufQueue.size() > 0)
                {
                    int S = BufQueue.front().size();
                    QByteArray ChunkData = BufQueue.front();
                    for (int I = 0; I < ChunkSize; I++)
                    {
                        TempBuf[I] = ChunkData[AudioPointer];
                        AudioPointer++;
                        if (AudioPointer >= S)
                        {
                            AudioPointer = 0;
                            TotalLength -= S;
                            BufQueue.pop();
                            if (BufQueue.size() > 0)
                            {
                                S = BufQueue.front().size();
                                ChunkData = BufQueue.front();
                            }
                            else
                            {
                                S = 0;
                                I = ChunkSize;
                            }
                        }
                    }
                }
            }
            mIOOutput->write(TempBuf);
        }
        Buffer_Mutex.unlock();
    }

    ///
    /// \brief AudioPlayerThread::PutToBuffer - Putting buffer to queue
    /// \param Buf
    /// \param BufLen
    ///
    void AudioPlayerThread::PutToBuffer(char *Buf, int BufLen)
    {
        Buffer_Mutex.lock();
        BufQueue.push(QByteArray::fromRawData(Buf, BufLen));
        TotalLength += BufLen;
        Buffer_Mutex.unlock();
    }

    ///
    /// \brief AudioPlayerThread::PutToBuffer - Putting buffer to queue
    /// \param Buf
    /// \param BufLen
    ///
    void AudioPlayerThread::PutToBuffer(short *Buf, int BufLen)
    {
        int L = BufLen * 2;
        char* TempBuf = new char[L];

        for(int i = 0; i < BufLen; i++)
        {
            TempBuf[i * 2] = (char)(Buf[i] & 255);
            TempBuf[i * 2 + 1] = (char)(Buf[i] >> 8);
        }

        Buffer_Mutex.lock();
        BufQueue.push(QByteArray::fromRawData(TempBuf, L));
        TotalLength += L;
        Buffer_Mutex.unlock();
    }

    ///
    /// \brief AudioPlayerThread::PutSilenceToBuffer - Putting silence to queue
    /// \param Len
    ///
    void AudioPlayerThread::PutSilenceToBuffer(int Len)
    {
        int T = Len * SizeFactor;
        char *Dummy = new char[T];
        for (int I = 0; I < T; I++)
        {
            Dummy[I] = 0;
        }
        PutToBuffer(Dummy, T);
    }

    ///
    /// \brief AudioPlayerThread::PlayStart - Starting playing
    ///
    void AudioPlayerThread::PlayStart()
    {
        QByteArray TempBuf(BufSize * SizeFactor, 0);
        mIOOutput = mAudioOutput->start();
        mIOOutput->write(TempBuf);
        Working = true;
    }

    ///
    /// \brief AudioPlayerThread::PlayStop - Stopping playing and flushing queue
    ///
    void AudioPlayerThread::PlayStop()
    {
        Working = false;
        mAudioOutput->stop();
        mIOOutput = 0;
        RemoveHeapObjects(false);
    }

    ///
    /// \brief AudioPlayerThread::BufferFlush - Flushing buffer
    ///
    void AudioPlayerThread::BufferFlush()
    {
        Buffer_Mutex.lock();
        while (!BufQueue.empty())
        {
            BufQueue.pop();
        }
        TotalLength = 0;
        AudioPointer = 0;
        Buffer_Mutex.unlock();
    }

    ///
    /// \brief AudioPlayerThread::GetDeviceList - Getting device list
    /// \return
    ///
    string *AudioPlayerThread::GetDeviceList()
    {
        return DeviceList;
    }

    ///
    /// \brief AudioPlayerThread::SelectDevice - Selecting device by name
    /// \param DeviceName
    /// \return
    ///
    bool AudioPlayerThread::SelectDevice(string DeviceName)
    {
        DeviceListCurrent = 0;
        if ((DeviceName == ""))
        {
            return true;
        }
        for (int I = 0; I < DeviceListN; I++)
        {
            if (DeviceList[I] == DeviceName)
            {
                DeviceListCurrent = I;
                return true;
            }
        }
        return false;
    }

    ///
    /// \brief AudioPlayerThread::SelectDevice - Selecting device by item number on the device list
    /// \param DeviceNo
    /// \return
    ///
    bool AudioPlayerThread::SelectDevice(int DeviceNo)
    {
        if ((DeviceNo > DeviceListN) || (DeviceNo < 0))
        {
            DeviceListCurrent = 0;
            return false;
        }
        return true;
    }
}
