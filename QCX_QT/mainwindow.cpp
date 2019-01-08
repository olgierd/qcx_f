#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    AR = new EdenClass::AudioRecorder();
    AP = new EdenClass::AudioPlayer();
    DevList = AR->GetDeviceList();
    SmpList = AR->GetPossibleSampleRates();

    EdenClass::ConfigFile CF;
    CF.FileLoad(Eden::ApplicationDirectory() + "config.txt");

    string AudioDevice = "Default";
    int AudioSampleRate = 22050;
    int AudioRefreshPeriod = 100;
    int AudioBufferSize = 250;
    bool AudioFile = false;
    string AudioFileName = "test.wav";
    string SerialName = "COM1";
    int SerialBaudrate = 9600;
    string SerialTest = "the quick brown fox jumps over the lazy dog";
    bool Info1 = false;
    bool Info2 = false;
    bool Info3 = false;

    CF.ParamGet("AudioDevice", AudioDevice);
    CF.ParamGet("AudioSampleRate", AudioSampleRate);
    CF.ParamGet("AudioRefreshPeriod", AudioRefreshPeriod);
    CF.ParamGet("AudioBufferSize", AudioBufferSize);
    CF.ParamGet("AudioFile", AudioFile);
    CF.ParamGet("AudioFileName", AudioFileName);
    CF.ParamGet("SerialName", SerialName);
    CF.ParamGet("SerialBaudrate", SerialBaudrate);
    CF.ParamGet("SerialTest", SerialTest);
    CF.ParamGet("Info1", Info1);
    CF.ParamGet("Info2", Info2);
    CF.ParamGet("Info3", Info3);



    ui->AudioDeviceC->clear();
    int I = 0;
    int II = -1;
    while (DevList[I] != "")
    {
        ui->AudioDeviceC->addItem(Eden::ToQStr(DevList[I]));
        if (DevList[I] == AudioDevice)
        {
            II = I;
        }
        I++;
    }
    if (II >= 0)
    {
        ui->AudioDeviceC->setCurrentIndex(II);
    }

    ui->AudioSampleRateC->clear();
    I = 0;
    II = -1;
    while (SmpList[I] != 0)
    {
        ui->AudioSampleRateC->addItem(Eden::ToQStr(SmpList[I]));
        if (SmpList[I] == AudioSampleRate)
        {
            II = I;
        }
        I++;
    }
    if (II >= 0)
    {
        ui->AudioSampleRateC->setCurrentIndex(II);
    }

    ui->AudioRefreshPeriodT->setText(Eden::ToQStr(AudioRefreshPeriod));
    ui->AudioBufferSizeT->setText(Eden::ToQStr(AudioBufferSize));
    ui->AudioFileC->setChecked(AudioFile);
    ui->AudioFileT->setText(Eden::ToQStr(AudioFileName));

    ui->SerialNameT->setText(Eden::ToQStr(SerialName));
    ui->SerialBaudrateT->setText(Eden::ToQStr(SerialBaudrate));
    ui->SerialTestT->setText(Eden::ToQStr(SerialTest));

    ui->Info1->setChecked(Info1);
    ui->Info2->setChecked(Info2);
    ui->Info3->setChecked(Info3);

    ProgTimer_ = NULL;
    ProgTimer__ = NULL;
    WF = NULL;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    EdenClass::ConfigFile CF;
    CF.ParamSet("AudioDevice", DevList[ui->AudioDeviceC->currentIndex()]);
    CF.ParamSet("AudioSampleRate", SmpList[ui->AudioSampleRateC->currentIndex()]);
    CF.ParamSet("AudioRefreshPeriod", Eden::ToInt(ui->AudioRefreshPeriodT->text()));
    CF.ParamSet("AudioBufferSize", Eden::ToInt(ui->AudioBufferSizeT->text()));
    CF.ParamSet("AudioFile", ui->AudioFileC->isChecked());
    CF.ParamSet("AudioFileName", Eden::ToStr(ui->AudioFileT->text()));
    CF.ParamSet("SerialName", Eden::ToStr(ui->SerialNameT->text()));
    CF.ParamSet("SerialBaudrate", Eden::ToInt(ui->SerialBaudrateT->text()));
    CF.ParamSet("SerialTest", Eden::ToStr(ui->SerialTestT->text()));
    CF.ParamSet("Info1", ui->Info1->isChecked());
    CF.ParamSet("Info2", ui->Info2->isChecked());
    CF.ParamSet("Info3", ui->Info3->isChecked());
    CF.FileSave(Eden::ApplicationDirectory() + "config.txt");
}

MainWindow::~MainWindow()
{
    delete DevList;
    delete SmpList;
    delete AR;
    delete AP;
    delete ui;
}

void MainWindow::Print(int MsgType, string X)
{
    bool ToPrint = ((MsgType == 0) || ((MsgType == 1) && (Info1)) || ((MsgType == 2) && (Info2)) || ((MsgType == 3) && (Info3)));
    if (ToPrint)
    {
        ui->Info->appendPlainText(Eden::ToQStr(X));
    }
}

///
/// \brief MainWindow::on_WorkStart_clicked - Starting audio processing
///
void MainWindow::on_WorkStart_clicked()
{
    Info1 = ui->Info1->isChecked();
    Info2 = ui->Info2->isChecked();
    Info3 = ui->Info3->isChecked();

    ui->SerialTestB->setEnabled(false);
    ui->WorkStart->setEnabled(false);
    ui->WorkStop->setEnabled(true);
    ui->Info->clear();
    Print(1, "Start");

    SerialOpen();

    SW.Reset();
    SW.Start();

    SampleRate = SmpList[ui->AudioSampleRateC->currentIndex()];

    if ((ui->AudioFileT->text() != "") && (ui->AudioFileC->isChecked()))
    {
        WF = new EdenClass::WaveFile();
        if (!WF->FileNew(Eden::ToStr(ui->AudioFileT->text()), SampleRate, true, false))
        {
            delete WF;
            WF = NULL;
        }
    }

    AR->SelectDevice(ui->AudioDeviceC->currentIndex());

    // 0 - Mono
    // 1 - Mid
    // 2 - Side
    // 3 - Left
    // 4 - Right
    AR->StereoMode = 0;

    AR->SetParams(SampleRate, 2, 1, (Eden::ToInt(ui->AudioBufferSizeT->text()) * SampleRate) / 1000, 0);

    if (ProgTimer__ != NULL)
    {
        //delete ProgTimer_;
        delete ProgTimer__;
    }

    ProgTimer_ = new ProgTimer(Eden::ToInt(ui->AudioRefreshPeriodT->text()), AR, AP);
    ProgTimer__ = new QThread(this);

    ProgTimer_->moveToThread(ProgTimer__);
    QObject::connect(ProgTimer_, SIGNAL(TimerEvent()), this, SLOT(AudioTimerTick()));
    ProgTimer__->start();

    AR->BufferFlush();
    AR->RecordStart();
}

///
/// \brief MainWindow::on_WorkStop_clicked - Stopping audio processing
///
void MainWindow::on_WorkStop_clicked()
{
    Print(1, "Stop");

    AR->RecordStop();
    SW.Stop();

    ProgTimer_->deleteLater();
    ProgTimer__->quit();
    ProgTimer__->wait();

    SerialClose();

    if (WF != NULL)
    {
        WF->FileClose();
        delete WF;
    }
    ui->SerialTestB->setEnabled(true);
    ui->WorkStart->setEnabled(true);
    ui->WorkStop->setEnabled(false);
}

///
/// \brief MainWindow::SerialOpen - Opening serial port, always 8-bit data, 1 stop bit and no parity
///
void MainWindow::SerialOpen()
{
    SerialLastMsg = "";
    SP.setPortName(ui->SerialNameT->text());
    SP.setBaudRate(Eden::ToInt(ui->SerialBaudrateT->text()));
    SP.setDataBits(QSerialPort::Data8);
    SP.setParity(QSerialPort::NoParity);
    SP.setStopBits(QSerialPort::OneStop);
    SP.setFlowControl(QSerialPort::NoFlowControl);
    SPOpen = SP.open(QIODevice::WriteOnly);
    if (!SPOpen)
    {
        Print(0, "Serial port error: " + Eden::ToStr(SP.errorString()));
    }
    else
    {
        Print(1, "Serial port open");
    }
}

///
/// \brief MainWindow::SerialMsg - Sending text message through serial port
/// \param Msg
///
void MainWindow::SerialMsg(string Msg)
{
    if ((SerialLastMsg == Msg) || (Msg == ""))
    {
        return;
    }
    SerialLastMsg = Msg;
    Print(3, "Serial port message: " + Msg);
    if (SPOpen)
    {
        SP.write(Msg.c_str(), Msg.length());
        if (!SP.waitForBytesWritten(1000))
        {
            Print(0, "Serial port error: " + Eden::ToStr(SP.errorString()));
        }
    }
}

///
/// \brief MainWindow::SerialClose - Closing serial port
///
void MainWindow::SerialClose()
{
    if (SPOpen)
    {
        SPOpen = false;
        SP.flush();
        SP.close();
        Print(3, "Serial port closed");
    }
}

///
/// \brief MainWindow::AudioTimerTick - Event invoked by timer tick
///
void MainWindow::AudioTimerTick()
{
    // Get number of unprocessed audio samples
    int BufLen = AR->GetAudioRemaining();
    if (BufLen == 0)
    {
        return;
    }

    Print(2, "Tick " + SW.TimeToString(SW.GetTime()) + ": " + Eden::ToStr(BufLen));

    // Aquire all encounted samples to array
    short * Buf__ = new short[BufLen];
    AR->GetFromBuffer16(BufLen, Buf__);


    // ########################################################################

    // Convert 16-bit signed values to float values int (-1, 1) range
    float * buf = new float[BufLen];
    for (int I = 0; I < BufLen; I++)
    {
        buf[I] = ((float)Buf__[I]) / 32768.0;
    }

    int len = BufLen;
    string Msg;


    rec = 0;
    i=1;
    last = -1;
    empty = 0;

    for(int x=0;x<len;x++) {
        // count empty samples, 20 samples ~< 1ms @ 22050 Hz sr
        if(buf[x] < 0.001 && buf[x] > -0.001) { empty++; }
        else { empty = 0; }
        if(empty > 20) { 	// 20 empty frames? send "Z" command
            reject = 4;
            symbol=0;
            first = 0;
            //!!sprintf(message, "Z");
            //!!sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen);
            SerialMsg("Z");
            break; }
    }

    for(i=0; i<len; i++) {		// for every sample in the buffer

        if(buf[i-1] < 0.0 && buf[i] >= 0.0) { // look for zero-crossing

            if(last >= 0) {
                rec++;
                diff = (i + (-buf[i-1]/(-buf[i-1]+buf[i])))-last; // interpolate
                freq = ((double)SampleRate)/diff;	// calculate frequency
                if(abs(freq-lastfreq) > 1) {
                    reject = 2; 	// wait two more 0-crossings before final measure (=~5ms @ 200 Hz)
                    lastfreq = freq;
                    newqrg = 1;
                }

                // check if difference is above 7Hz
                if(diff > 7 && reject == 0 && newqrg) {
                    if(first==0) { first = freq; }
                    //!!printf("%d: %.3f %.3f %.3f\n", symbol, diff, freq, (freq-first)/6.25);
                    //!!sprintf(message, "A%05dX", (int)((freq*10)+0.5));	// send via socket
                    //!!sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen);
                    Msg = "Info: ";
                    Msg += ("symbol=" + Eden::ToStr(symbol) + "   ");
                    Msg += ("diff=" + Eden::ToStr(diff) + "   ");
                    Msg += ("freq=" + Eden::ToStr(freq) + "   ");
                    Msg += ("(freq-first)/6.25=" + Eden::ToStr((freq-first)/6.25) + "   ");
                    Print(2, Msg);
                    SerialMsg("A" + Eden::ToStr((int)((freq*10)+0.5)) + "X");
                    symbol++;
                    newqrg = 0;
                }

                if(reject > 0) { reject--; }
            }
            last = i + (-buf[i-1]/(-buf[i-1]+buf[i]));
        }
    }



    delete[] buf;


    // ########################################################################

    // Write aquired samples for testing and debuging purpose
    if (WF != NULL)
    {
        WF->WriteM(Buf__, -1, BufLen);
    }

    delete[] Buf__;
}

///
/// \brief MainWindow::on_SerialTestB_clicked - Testing serial port
///
void MainWindow::on_SerialTestB_clicked()
{
    ui->Info->clear();
    SerialOpen();
    SerialMsg(Eden::ToStr(ui->SerialTestT->text()));
    SerialClose();
}
