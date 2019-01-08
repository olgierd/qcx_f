# FT8 for QCX Transceiver

## Introduction

### What is QCX?

QCX is an affordable, low power amateur radio transceiver kit designed and sold by [QRP Labs](1). It provides up to 5 watts of RF output power on a single [HF band](2), features a built-in CW keyer and offers a narrow RX filter.

### What is FT8?

[FT8](3) is a digital weak signal communication mode developed by Joe K1JT in 2017. It has since become a favourite digital mode for many radio amateurs, as it can be decoded with a very low signal-to-noise ratio - in practice, it's possible to perform a very long distance contacts using low TX power and modest antennas, even when propagation conditions are far from ideal.

### What is this project about?

QCX is a CW transceiver by design, but looking at the schematic, it's capable of transmitting many other digital (CW *is* digital!) modes involving frequency keying with no amplitude modulation. That's a **yes** for FT8, JT65, RTTY, Hell, but a **no** for PSK or SSTV. This project brings FT8 (and possibly other modes too - to be tested) support to QCX.

Code presented here consists of two parts: transceiver firmware and PC software. FT8 message encoding/decoding algorithms are quite complex (well, and all in Fortran), that's why I found it easier to let the "official" [WSJTX](4) application do the hard work, and only capture the output audio frequency and drive the transmitter accordingly.

## Transceiver firmware

Source code for the firmware was created using Arduino IDE.

### How to build & flash
1. Get [Si5351 library](6) - navigate to `Sketch → Include Library → Manage Libraries...` in Arduino IDE to add it.

2. QCX is using 20 MHz clock for ATmega instead of 16 MHz as Arduinos do, so you need to update `mini.build.f_cpu=16000000L` line in `hardware/arduino/avr/boards.txt` file to `mini.build.f_cpu=20000000L` - just remember to change it back whenever you need to program an original Arduino Mini with 16M clock!

**WARNING** - the next step will remove the original firmware from your QCX. I don't see why you shouldn't be able to flash it back, but I conducted all of my tests on a spare ATmega328P.

3. Choose Arduino Mini board and the programmer you have - I used USBASP and ISP connector to the left of LCD. Press CTRL-U and hopefully you should see welcome message on LCD.

### Usage and wiring

Use the encoder for tuning. Connect TTL serial port to the GPS connector in the right-bottom of the board. TX from PC into RX of QCX & GNDs.

### Features

* LCD works
* Upper sideband (USB) reception with encoder tuning (100 Hz step)
* Software serial

### Serial protocol instructions
* `A123X` - sets frequency 123 Hz above local oscillator and enables TX
* `Z` - disable TX
* `w` - tune 100 Hz down
* `e` - tune 100 Hz up

## PC Software - Linux

### Build and Run

`cd Linux; make`. The app is based on pulseaudio, it may be necessary to get some pulseaudio libraries to build it.

In order to run:
* `./qcx_f`
* Run `pavucontrol`, in Recording tab, set QCX_F input to _Monitor of (your sound output)_
* Run WSJTX and start transmitting, you should see some console output.
* In order to actually drive the QCX's transmitter, run `udpclient.py`. Modify the source code, set the serial port to point to your device (`/dev/ttyUSB0` by default).

## PC software - Python

Python version has been created as well.

Few additional libraries are required to run it. On Ubuntu/Debian, run `apt-get install python-pyaudio python-numpy python-serial` to install them.



## PC Software - Windows

TODO

## PC Software - Qt

There is GUI-based user friendly software, which uses Qt library. The compilation and running was tested on Windows 8 and Ubuntu Linux 16.04 LTS, authors not guarantees compile posibility on other operating systems.

After first program running you have to set audio device parameters:
* __Device__ - Phisical or virtual recording audio device.
* __Sample rate__ - Recording sample rate, the list of available sample rates may vary depending on audio device.
* __Refresh period__ - Timer interval in milliseconds, its event will occur. The longer refresh period will increase fluenty of audio recording and processing, but can cause timing jitter in controlling transmitter.
* __Buffer size__ - Audio device buffer size in milliseconds, it is recommended to be between slighty above than refresh period and below a few refresh period times. The value can be limited by audio device.
* __Audio file__ - File name, where recorded audio will be saved. The purpose of this is to test, if audio is recorded correctly.

Program can send transmitter control data through serial port. You can set the port name and baudrate:
* __Name__ - Name of serial port like COM1, COM2.
* __Baud rate__ - Number of bits per second.
* __Send text__ - Test port transmitting, the text must be received by device connected to the port.

The other serial port parameters are fixed to following:
* Data bits: 8
* Stop bits: 1
* Parity: none
* Data flow contol: none

In __Info__ box, you can choose from three kinds of messages to display. Error messages will be always displayed.

The __Start__ button starts audio processing and the _Stop_ button stops audio processing. None of settings will affect during audio processing. All parameters will be saved after program exit.

### Build and Run on Windows 8

At this description, there will be used following wilcards:

* __%MinGW%__ - Directory, where MinGW is installed (for example &quot;C:\Qt\Qt5.6\5.6\mingw49_32\&quot;).
* __%Binary%__ - Directory, where binary is placed after building (contains &quot;QCX_____QT.exe&quot;).

To compile QCX_____QT for Windows, you have to do following:

1. Install Qt Creator with Qt library and MinGW compiler.
2. Open QCX_____QT.pro in Qt Creator, click __Configure Project__ in project configuration screen if appears.
3. Build project as release.
4. Run following command: `%MinGW%\bin\windeployqt.exe --release %Binary%`.
5. Copy following files from %MinGW%\\bin\\ to %Binary%: libgcc_s_dw2-1.dll, libstdc*6.dll, libwinpthread-1.dll.
6. Remove all *.cpp and *.o files.

### Build and Run on Ubuntu Linux 16.04 LTS

At this description, there will be used following wilcards:

* __%Linuxdeployqt%__ - Directory, where Linuxdeployqt tool is placed.
* __%Binary%__ - Directory, where binary is placed after building (contains &quot;QCX_____QT&quot; executable file).

To compile QCX_____QT for Linux equipped with apt-get tool, you have to do following:

1. Upgrade installed packet using following: `sudo apt-get update and sudo apt-get upgrade`.
2. Install Qt library and Qt Creator using: `sudo apt-get install build-essential qtcreator qt5-default`.
3. Install multimedia extension for Qt using: `sudo apt-get install libqt5multimedia5 libqt5multimedia5-plugins libqt5multimediawidgets5`.
4. install serial port extension for Qt using: `sudo apt-get install libqt5serialport5 libqt5serialport5-dev`.
5. Download &quot;linuxdeployqt-continuous-x86_64.AppImage&quot; from https://github.com/probonopd/linuxdeployqt/releases and place it into %Linuxdeployqt%.
6. Open QCX_____QT.pro in Qt Creator, click __Configure Project__ in project configuration screen if appears.
7. Build project as release.
8. Run following command to build stanlone application: `'%Linuxdeployqt%/linuxdeployqt-continuous-x86_64.AppImage' '%Binary%/QCX_____QT'` -appimage.
9. Remove all *.h, *.cpp, *.o, .DirIcon, AppRun, default.desktop, default.png and Makefile files.

## Disclaimer

This is an experimental, unofficial piece of software with no support other than authors' best effort, whatever you do with it is at your own risk. No QCXes were destroyed or damaged during development. Perform tests using dummy load. Transmitting RF in amateur bands requires an appropriate licence.

[1]: https://qrp-labs.com/qcx.html
[2]: https://en.wikipedia.org/wiki/High_frequency
[3]: https://en.wikipedia.org/wiki/WSJT_(amateur_radio_software)#FT8
[4]: https://physics.princeton.edu/pulsar/k1jt/wsjtx.html
[5]: http://qrp-labs.com/images/qcx/assembly_A4.pdf
[6]: https://github.com/etherkit/Si5351Arduino
