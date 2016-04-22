/*
  LiquidCrystal Library - Serial Input

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch displays text sent over the serial port
  (e.g. from the Serial Monitor) on an attached LCD.

  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http://www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 22 Nov 2010
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystalSerial
*/

// include the library code:
#include <LiquidCrystal.h>
#include <avr/interrupt.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28);
int __counter = 0;

bool started;
bool stopped;
//arduino runs the setup function first, then the loop function below
void setup() {
  pinMode(24, OUTPUT); //K
  pinMode(26, OUTPUT); //A
  pinMode(54, OUTPUT); //VSS
  pinMode(52, OUTPUT); //VDD
  pinMode(50, OUTPUT); //Contrasty pin

  // digitalWrite(50, LOW);
  digitalWrite(24, LOW); //Backlight
  digitalWrite(26, HIGH); //Backlight
  digitalWrite(54, LOW); //GND
  digitalWrite(52, HIGH); //VDD
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize the serial communications:
  Serial.begin(9600);
  // Timer0 is used for millis() - we'll just interrupt
  // in the middle and call the compA
  OCR0A = 0x01;
  TIMSK0 |= _BV(OCIE0A);

  started = false;
  stopped = false;
}

SIGNAL(TIMER0_COMPA_vect) 
{
   __counter++;
   if (__counter > 14){
      digitalWrite(50,HIGH);
      __counter = 0;
   }
   else if (__counter > 3){
      digitalWrite(50, LOW);
   }
}
String start_hour="-";
String start_minutes;
int strt_hr;
int strt_min;
unsigned long milli;

void startTimer(){
  //Serial.print(start_hour);
  if( Serial.available()>0){
    String message = Serial.readString();
    if (message.startsWith("t")) {
      start_hour = message.substring(1,3);
      start_minutes = message.substring(3,5); 
      strt_hr = start_hour.toInt();
      strt_min = start_minutes.toInt();
      
    } 
    if(message.startsWith("start")){
      lcd.print(start_hour+':'+start_minutes);
      milli = millis();
      stopped = false;
      }
     if(message.startsWith("stop")){
        stopped = true;
      } 
   }
 }
 void changeTime(){
  
  strt_min+=1;
   Serial.print(start_minutes);
  if (strt_min==60){
      strt_min= 0;
      strt_hr+= 1;
      start_minutes = String(strt_min);
    }
    start_minutes = String(strt_min);
    start_hour = String(strt_hr);
   if(strt_hr == 24){
     strt_hr=0;
    }
   if(strt_hr<10){
       start_hour= "0"+String(strt_hr);
      }
   if(strt_min <10){
       start_minutes= "0"+String(strt_min);
      }
   
      lcd.clear();
     lcd.print(start_hour+':'+start_minutes);
    
  }
//Here is where your code goes. Arduino runs this function in an infinite loop after running the setup function

void loop() {
  
  //return cursor to starting position
  lcd.home();

  //clear the lcd screen
  //lcd.clear();
  //displays a String on the lcd screen. You can also print a String object
  //lcd.print("hello world");
  startTimer();
  unsigned long milli2 = millis();
  if( milli2-milli >=1000){
    if(stopped == false){
      changeTime();
      
    }
    milli=milli2;
    }
  
 // lcd.print("Time: ");
  lcd.setCursor(0, 1);
  
  
  
  
  
  //lcd.print("Time: ");
  //String message = Serial.readString();
  // read a string sent from the computer
  // lcd.print(message);//prints hello world to the serial monitor
  
  //sleep for 1 second
 // delay(1000);
}


