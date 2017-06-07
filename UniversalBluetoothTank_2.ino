/*
  ========================================================================================
  AUTHOR: Ricardo Ferro (www.ricksmod.com)
  DATE: Aug 1, 2015
  LICENSE: Public domain (use at your own risk)
  CONTACT: ZeroBeatPro@gmail.com
  NOTES:Im using pro mini
  http://www.ricksmod.com
  =========================================================================================
*/
#include "MotorDriver.h"
int tankspeed=100;

MotorDriver motor;
void boardinfo (void);
#include <SoftwareSerial.h>
//Helper macro DEBUG------------------------------------------------------------------
#define LINE(name,val) Serial.print(name); Serial.print("\t"); Serial.println(val);
//End Macro Debug----------------------------------------------------------------------
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // mega
//Input Custom Pins here
#define rxPin 50
#define txPin 51
#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)//Uno,Pro mini
#define rxPin 4
#define txPin 2
#elif defined(__AVR_ATmega32U4__) // leonardo,Yun,Micro
#define rxPin 11
#define txPin 10

//#else //168, 328, something else
//input custom pin here
#endif

SoftwareSerial BTSerial(rxPin, txPin); // RX | TX

void setup()
{
  // initialize
  motor.begin();
  pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  digitalWrite(9, HIGH);//Connect 34 to pin 9
  Serial.begin(9600);  // HC-05 default speed in HC-05 COMMAND MODE
  //Serial.begin(57600); //arduino  HC-05 default speed in HC-05 Communication Mode. PART 1
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // mega
  //Input Custom Pins here

#elif defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)//Uno,Pro mini
  Serial.println("Serial connection made to Arduino Uno/ProMini");
  Serial.println("make sure the baud rate is set to 34000 and line endings is set to be Both NL & CR");
  Serial.println(" AT+ORGL ");
  Serial.println(" AT+ROLE=0 ");
  Serial.println(" AT+POLAR=1,0 ");
  Serial.println(" AT+UART=115200,0,0 ");
  Serial.println(" AT+INIT   ");
  Serial.println("In order, these commands tell the module to reset to factory settings, ");
  Serial.println("Switch to slave role (transparent serial bridge), set pin 32 low on Bluetooth connection, ");
  Serial.println("change baud rate to match the Arduino Uno programming rate, and to initialize. ");
  Serial.println("Disconnect 3.3v from the Key pin and cycle the power to the module. It will now be running at 115200 baud and be in pairing mode. ");
#elif defined(__AVR_ATmega32U4__) // leonardo,Yun,Micro
  Serial1.begin(38400); // Bluetooth communications
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for Arduino Micro
  }
  //Serial.println("Serial connection made (Arduino --> USB)");

  while (!Serial1)
  {
    ; // wait for serial1 port to connect. Needed for Arduino Micro
  }

  //Serial.println("Serial 1 connection made (Arduino <--> Bluetooth)");
  //#else //168, 328, something else
  //input custom pin here
#endif



  boardinfo();//Detects Board
  Serial.println("Enter AT commands:");
  //BTSerial.begin(38400);  // HC-05 default speed in AT command more
  //BTSerial.begin(57600);  // HC-05 default speed in HC-05 Communication Mode.
  BTSerial.begin(9600);  // HC-05 default speed in HC-05 Communication Mode.

}

void loop()
{
  char recvChar;
  while (1)
  {
    //check if there's any data sent from the remote bluetooth shield
    if (BTSerial.available())
    {
      recvChar = BTSerial.read();

      if (recvChar == '1')
      {
        // digitalWrite(led,HIGH);

        motor.speed(0, tankspeed);            // set motor0 to speed 100
        motor.speed(1, -(tankspeed));            // set motor0 to speed 100
        //motor.speed(0, -100);           // set motor0 to speed -100
        Serial.println(" AT+INIT   ");


      }
      else if (recvChar == '2')
      {
        motor.speed(0, 69);           // set motor0 to speed -100
        motor.speed(1, 69);           // set motor0 to speed -100


      }
      else if (recvChar == '3')
      {
        motor.speed(0, -100);           // set motor0 to speed -100
        motor.speed(1, 100);           // set motor0 to speed -100
      }
      else if (recvChar == '4')
      {

        motor.speed(0, -69);           // set motor0 to speed -100
        motor.speed(1, -69);           // set motor0 to speed -100
      }
      else if (recvChar == '5')
      {
        motor.brake(0);                 // brake
        motor.brake(1);                 // brake
        Serial.println(" AT+INIT   ");
      }
      else if (recvChar == '6')
      {
        motor.stop(0);                  // stop
        motor.stop(1);                  // stop
      }
      else if (recvChar == '7')
      {
        // digitalWrite(led,HIGH);

        int tankspeed = 100;


      }
      else if (recvChar == '8')
      {
        // digitalWrite(led,HIGH);

        int tankspeed = 70;


      }

      else
      {
      }
    }
  }
  //  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (BTSerial.available())
    Serial.write(BTSerial.read());
  //
  //  // Keep reading from Arduino Serial Monitor and send to HC-05
  //if (Serial.available())
  //BTSerial.write(Serial.read());
}

void boardinfo(void)
{
  //-------------------------------------------------DEBUG---------------------------------------------------------------------------
  Serial.println("\n===============================================Board Type Info====================================================");
  Serial.println("\nCPU:");
#if defined(__AVR_ATmega168__)
  // Arduino Diecimila and older
  LINE("__AVR_ATmega168__", __AVR_ATmega168__);
#endif

#if defined(__AVR_ATmega328P__)
  // Arduino Duemilanove and Uno
  LINE("__AVR_ATmega328P__", __AVR_ATmega328P__);
#endif

#if defined(__AVR_ATmega2560__)
  // Arduino Mega 2560
  LINE("__AVR_ATmega2560__", __AVR_ATmega2560__);
#endif

#if defined(__AVR_ATmega1280__)
  // Arduino Mega
  LINE("__AVR_ATmega1280__", __AVR_ATmega1280__);
#endif

#if defined(__AVR_ATmega32U4__)
  // Arduino Leonardo
  LINE("__AVR_ATmega32U4__", __AVR_ATmega32U4__);
#endif

#if defined(__SAM3X8E__)
  // Arduino Due
  LINE("__SAM3X8E__", __SAM3X8E__);
#endif

  // Determine board type
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__)
  Serial.println(" Arduino Uno");
#elif defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
  Serial.println("Arduino Mega");
#elif defined(__AVR_ATmega32U4__)
  Serial.println("Arduino Leonardo");
#elif defined(__SAM3X8E__)
  Serial.println("Arduino Due");
#else
  Serial.println("Unknown");
#endif

  Serial.println("\nArduino:");

  // Arduino SW version
  LINE("ARDUINO", ARDUINO);

  // Oscillator frequency
  LINE("F_CPU", F_CPU);
  Serial.println("\n=================================================Board Type Info====================================================");
  //-----------------------------------------------------DEBUG END-------------------------------
}
