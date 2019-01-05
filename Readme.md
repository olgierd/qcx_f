# FT8 TX firmware/software for QRP Labs QCX Transceiver

## Introduction

### What is QCX?

QCX is an affordable, low power amateur radio transceiver kit designed and sold by [QRP Labs](1). It provides up to 5 watts of RF output power on a single [HF band](2), features a built-in CW keyer and offers a narrow RX filter.

### What is FT8?

[FT8](3) is a digital weak signal communication mode developed by Joe K1JT in 2017. It has since become a favourite digital mode for many radio amateurs, as it can be decoded with a very low signal-to-noise ratio - in practice, it's possible to perform a very long distance contacts using low TX power and modest antennas, even when propagation conditions are far from ideal.

### What is this project about?

QCX is a CW transceiver by design, but looking at the schematic, it's capable of transmitting many other digital (CW *is* digital!) modes involving frequency keying with no amplitude modulation. That's a **yes** for FT8, JT65, RTTY, Hell, but a **no** for PSK or SSTV. This project brings FT8 (and possibly other modes too - to be tested) support to QCX.

Code presented here consists of two parts: transceiver firmware and PC software. FT8 message encoding/decoding algorithms are quite complex (well, and all in Fortran), that's why I found it easier to let the "official" [WSJTX](4) application take care of that, and only capture the output audio frequency and drive the transmitter accordingly.

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
* In order to actually drive the QCX's transmitter, run `udpclient.py`. Modify the source, according to the serial port you use (`/dev/ttyUSB0` by default).


[1]: https://qrp-labs.com/qcx.html
[2]: https://en.wikipedia.org/wiki/High_frequency
[3]: https://en.wikipedia.org/wiki/WSJT_(amateur_radio_software)#FT8
[4]: https://physics.princeton.edu/pulsar/k1jt/wsjtx.html
[5]: http://qrp-labs.com/images/qcx/assembly_A4.pdf
[6]: https://github.com/etherkit/Si5351Arduino
