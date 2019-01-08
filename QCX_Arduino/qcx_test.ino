// de SQ3SWF 2018

#include <Wire.h>             // for Si5351
#include <LiquidCrystal.h>  // HD44780 support
#include <SoftwareSerial.h> // for PC communication
#include "si5351.h"           // PLL control

#define BAUDRATE 115200       // serial port baudrate
#define BASEFREQ 140740000ULL // default Frequency
#define XTAL_FREQ 27005750    // Si5351 27MHz xtal exact frequency
#define TUNING_STEP 1000       // encoder tuning step - 1000 = 100 Hz

// Pinout
#define ENC_LEFT 6            // Encoder - rotating left
#define ENC_RIGHT 7           // Encoder - rotating right
#define RX 8                  // TX/RX switching
#define TXPIN 9               // SoftwareSerial TX (not on GPS header!) / unused
#define KEY_OUT 10            // TX enable - power to final transistors
#define RXPIN 13              // SoftwareSerial RX
#define KEY 17                // straight key lever on PCB

// LCD Pinout
#define LCD_RS 18
#define LCD_EN 4
#define LCD_D4 0
#define LCD_D5 1
#define LCD_D6 2
#define LCD_D7 3

// objects
Si5351 si5351;
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
SoftwareSerial ser =  SoftwareSerial(RXPIN, TXPIN);

// global variables
unsigned long long freq = BASEFREQ;
unsigned long long pll_freq = 50*freq;
char key;
uint16_t freq_add;


// disable TWI/I2C - allow sharing SDA and LCD_RS line on the same pin
void twi_disable(void) 
{
  TWCR &= ~(_BV(TWEN) | _BV(TWIE) | _BV(TWEA));
  digitalWrite(SDA, 0);
  pinMode(SDA, OUTPUT);
}

void setup() {
  
  pinMode(ENC_LEFT, INPUT_PULLUP);
  pinMode(ENC_RIGHT, INPUT_PULLUP);
  pinMode(RXPIN, INPUT);
  pinMode(KEY, INPUT);
  pinMode(RX, OUTPUT);
  pinMode(KEY_OUT, OUTPUT);

  // RX LOW = receive mode, quiet startup
  digitalWrite(RX, LOW);

  // init LCD
  lcd.begin(16, 2);
  lcd.print("QCX Custom");
  lcd.setCursor(13,1);
  lcd.print("^_^");

  // init DDS
  twi_disable();
  si5351.init(SI5351_CRYSTAL_LOAD_8PF, XTAL_FREQ, 0);
  si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_2MA);
  si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_2MA);
  setFreq();

  // init Serial port
  ser.begin(BAUDRATE);
  
}

// sets RX frequency, generates 90° I/Q signals
void setFreq() {

  digitalWrite(RX, LOW);
  si5351.set_freq_manual(freq*10, pll_freq*10, SI5351_CLK0);
  si5351.set_freq_manual(freq*10, pll_freq*10, SI5351_CLK1);
  si5351.set_phase(SI5351_CLK0, 0);
  si5351.set_phase(SI5351_CLK1, 50);

  si5351.pll_reset(SI5351_PLLA);
  digitalWrite(RX, HIGH);

}

// tuning - encoder rotation
void encoderChange(uint8_t dir) {
  freq += dir?TUNING_STEP:-TUNING_STEP;
  setFreq();
  lcd.setCursor(15,0); 
  lcd.print(dir?'>':'<'); 
  printFreq(freq);
  while(!digitalRead(ENC_RIGHT)||!digitalRead(ENC_LEFT)) { delay(1); }; 
}

// print frequency on lCD
void printFreq(unsigned long long tmp){
  twi_disable();
  lcd.setCursor(0,1);
  lcd.print("            ");
  lcd.setCursor(0,1);
  lcd.print((uint32_t)tmp/10);
}


void loop() {
  
  if(!digitalRead(ENC_LEFT)) { encoderChange(0); }
  if(!digitalRead(ENC_RIGHT)) { encoderChange(1); }

//  if(digitalRead(KEY)) { digitalWrite(RX, LOW); }
//  else { digitalWrite(RX, HIGH); }

  if (ser.available()>0) { 
    key = ser.read();

    // serial tuning
    if(key=='w') { encoderChange(0); }
    if(key=='e') { encoderChange(1); }

    // start receiving new TX freq
    if(key=='A') freq_add=0;

    // receive single digit
    if(key>='0' && key<='9') { freq_add *= 10; freq_add += key-'0'; }

    // finish receiving, set new freq and enable TX
    if(key == 'X') { 
      si5351.set_freq_manual((freq+freq_add)*10, pll_freq*10, SI5351_CLK2); 
      digitalWrite(RX, LOW); 
      digitalWrite(KEY_OUT, HIGH);
      printFreq(freq+freq_add); 
      lcd.print(" TX"); 
    }

    // finish transmission - disable TX, enable RX, disable CLK2 output of Si5351
    if(key == 'Z') { 
      digitalWrite(KEY_OUT, LOW);
      delay(20);
      digitalWrite(RX, HIGH); si5351.output_enable(SI5351_CLK2, 0); printFreq(freq); }

  }
  
}

