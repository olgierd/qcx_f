#include "audiorecorderthread.h"

namespace EdenClass
{
    AudioRecorderThread::AudioRecorderThread()
    {
        TempBuf8L = 1;
        TempBuf8 = new char[1];

        mAudioInput = 0;
        mIOInput = 0;
        Working = false;
        TotalLength = 0;
        AudioPointer = 0;

        QList<QAudioDeviceInfo> DevInfo = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
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

    AudioRecorderThread::~AudioRecorderThread()
    {
        RemoveHeapObjects(true);
        delete[] DeviceList;
        delete[] TempBuf8;
    }

    ///
    /// \brief AudioRecorderThread::RemoveHeapObjects - Removing existing objects on heap
    /// \param Aud
    ///
    void AudioRecorderThread::RemoveHeapObjects(bool Aud)
    {
        Working = false;
        if (Aud)
        {
            if (mAudioInput != 0)
            {
                delete mAudioInput;
                mAudioInput = 0;
            }
        }
        if (mIOInput != 0)
        {
            delete mIOInput;
            mIOInput = 0;
        }
        delete[] TempBuf8;
        TempBuf8 = new char[1];
        TempBuf8L = 1;
    }

    ///
    /// \brief AudioRecorderThread::GetPossibleSampleRates - Getting all possible sample rates
    /// \return
    ///
    int *AudioRecorderThread::GetPossibleSampleRates()
    {
        QList<int> XXX;
        if (DeviceListCurrent == 0)
        {
            QAudioDeviceInfo QAFI(QAudioDeviceInfo::defaultInputDevice());
            XXX = QAFI.supportedSampleRates();
        }
        else
        {
            QAudioDeviceInfo QAFI(QAudioDeviceInfo::availableDevices(QAudio::AudioInput)[DeviceListCurrent - 1]);
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
    /// \brief AudioRecorderThread::SetParams - Setting recording parameters including buffer size and timer interval
    /// \param SampleRate
    /// \param SampleSize
    /// \param Channels
    /// \param AudioBufSize
    /// \param AudioBufInterval
    ///
    void AudioRecorderThread::SetParams(int SampleRate, int SampleSize, int Channels, int AudioBufSize, int AudioBufInterval)
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
            QAudioDeviceInfo QAFI(QAudioDeviceInfo::defaultInputDevice());
            if (!QAFI.isFormatSupported(QAF))
            {
                QAF = QAFI.nearestFormat(QAF);
            }
            mAudioInput = new QAudioInput(QAFI, QAF, 0);
        }
        else
        {
            QAudioDeviceInfo QAFI(QAudioDeviceInfo::availableDevices(QAudio::AudioInput)[DeviceListCurrent - 1]);
            if (!QAFI.isFormatSupported(QAF))
            {
                QAF = QAFI.nearestFormat(QAF);
            }
            mAudioInput = new QAudioInput(QAFI, QAF, 0);
        }
        mAudioInput->setBufferSize(BufSize * SizeFactor);
        if (AudioBufInterval > 0)
        {
            mAudioInput->blockSignals(false);
            mAudioInput->setNotifyInterval(NotifyInterval);
            QObject::connect(mAudioInput, SIGNAL(notify()), this, SLOT(FillAudioBuffer()));
        }
        else
        {
            mAudioInput->blockSignals(true);
        }

    }

    ///
    /// \brief AudioRecorderThread::RecordStart - Starting recording
    ///
    void AudioRecorderThread::RecordStart()
    {
        mIOInput = mAudioInput->start();
        mIOInput->readAll();
        Working = true;
    }

    ///
    /// \brief AudioRecorderThread::RecordStop - Stopping recording
    ///
    void AudioRecorderThread::RecordStop()
    {
        Working = false;
        mAudioInput->stop();
        mIOInput = 0;
        RemoveHeapObjects(false);
    }

    ///
    /// \brief AudioRecorderThread::FillAudioBuffer - Reading data from device to buffer
    ///
    void AudioRecorderThread::FillAudioBuffer()
    {
        if (Working)
        {
            Buffer_Mutex.lock();
            QByteArray TempBufQ = mIOInput->readAll();
            int TempBufL = TempBufQ.size();
            if (TempBufL > 0)
            {
                BufQueue.push(TempBufQ);
                TotalLength += TempBufL;
            }
            Buffer_Mutex.unlock();
        }
    }

    ///
    /// \brief AudioRecorderThread::GetAudioRemaining - Count of not read samples
    /// \return
    ///
    int AudioRecorderThread::GetAudioRemaining()
    {
        Buffer_Mutex.lock();
        int X = ((TotalLength - AudioPointer) / SizeFactor);
        Buffer_Mutex.unlock();
        return X;
    }

    ///
    /// \brief AudioRecorderThread::GetFromBuffer8 - Getting 8-bit sound from buffer
    /// \param BufLen
    /// \param TempBuf
    /// \return
    ///
    char *AudioRecorderThread::GetFromBuffer8(int BufLen, char *TempBuf)
    {
        Buffer_Mutex.lock();
        if (TotalLength <= 0)
        {
            TotalLength = 0;
        }

        BufLen = BufLen * SizeFactor;
        if (TempBuf == NULL)
        {
            TempBuf = new char[BufLen];
        }
        if (BufLen > (TotalLength - AudioPointer))
        {
            for (int I = (TotalLength - AudioPointer); I < BufLen; I++)
            {
                TempBuf[I] = 0;
            }
            BufLen = TotalLength - AudioPointer;
        }
        if (TotalLength == 0)
        {
            return TempBuf;
        }

        for (int I = 0; I < BufLen; I++)
        {
            TempBuf[I] = BufQueue.front()[AudioPointer];
            AudioPointer++;
            if (AudioPointer == BufQueue.front().size())
            {
                AudioPointer = 0;
                TotalLength -= BufQueue.front().size();
                BufQueue.pop();
                if (TotalLength == 0)
                {
                    BufLen = I - 1;
                }
            }
        }

        Buffer_Mutex.unlock();
        return TempBuf;
    }

    ///
    /// \brief AudioRecorderThread::GetFromBuffer16 - Getting 8-bit sound from buffer
    /// \param BufLen
    /// \param TempBuf
    /// \return
    ///
    short *AudioRecorderThread::GetFromBuffer16(int BufLen, short *TempBuf)
    {
        if (TempBuf8L < (BufLen * SizeFactor))
        {
            delete[] TempBuf8;
            TempBuf8L = (BufLen * SizeFactor);
            TempBuf8 = new char[TempBuf8L];
        }
        TempBuf8 = GetFromBuffer8(BufLen, TempBuf8);

        BufLen = BufLen * SizeFactor / 2;
        if (TempBuf == NULL)
        {
            TempBuf = new short[BufLen];
        }
        for (int I = 0; I < BufLen; I++)
        {
            TempBuf[I] = (TempBuf8[(I << 1) + 1] << 8) + ((unsigned char)TempBuf8[(I << 1)]);
        }
        return TempBuf;
    }

    ///
    /// \brief AudioRecorderThread::BufferFlush - Flushing buffer
    ///
    void AudioRecorderThread::BufferFlush()
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
    /// \brief AudioRecorderThread::GetDeviceList - Getting device list
    /// \return
    ///
    string *AudioRecorderThread::GetDeviceList()
    {
        return DeviceList;
    }

    ///
    /// \brief AudioRecorderThread::SelectDevice - Selecting device by name
    /// \param DeviceName
    /// \return
    ///
    bool AudioRecorderThread::SelectDevice(string DeviceName)
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
    /// \brief AudioRecorderThread::SelectDevice - Selecting device by item number on the device list
    /// \param DeviceNo
    /// \return
    ///
    bool AudioRecorderThread::SelectDevice(int DeviceNo)
    {
        if ((DeviceNo > DeviceListN) || (DeviceNo < 0))
        {
            DeviceListCurrent = 0;
            return false;
        }
        return true;
    }
}
