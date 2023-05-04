// --------------- INITIAL CODE FOR IR ---------------
#include <Arduino.h>

#define DISABLE_CODE_FOR_RECEIVER // Disables restarting receiver after each send. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not used.
//#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
//#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition

#include "PinDefinitionsAndMore.h" // Define macros for input and output pin etc.
#include <IRremote.hpp>
const int buttonPin = 11;
int buttonState = 1;
// --------------- END INITIAL CODE FOR IR ---------------

// --------------- INITIAL CODE FOR LEDS ---------------
#include <FastLED.h>
#define LED_PIN 9
#define NUM_LEDS 24

// REDUNDANT const int buttonPin = 11;
// REDUNDANT //int buttonState = 1;

CRGB leds[NUM_LEDS];
// --------------- END INITIAL CODE FOR LEDS ---------------

// --------------- INITIAL CODE FOR SOUND ---------------
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
// REDUNDANT #define buttonPin 11
// REDUNDANT int buttonState = 1;

SoftwareSerial mySoftwareSerial(4, 5);                    // Recieve Pin (RX), Transmit Pin (TX)
DFRobotDFPlayerMini myDFPlayer;                           // Create myDFPlayer of the DFPlayer Class
// --------------- END INITIAL CODE FOR SOUND ---------------


void setup() {
  // --------------- CODE FOR IR ---------------
  pinMode(buttonPin, INPUT_PULLUP);
    
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
    Serial.print(F("Send IR signals at pin "));
    Serial.println(IR_SEND_PIN);

    /*
     * The IR library setup. That's all!
     */
//    IrSender.begin(); // Start with IR_SEND_PIN as send pin and if NO_LED_FEEDBACK_CODE is NOT defined, enable feedback LED at default feedback LED pin
    IrSender.begin(DISABLE_LED_FEEDBACK); // Start with IR_SEND_PIN as send pin and disable feedback LED at default feedback LED pin
   // --------------- END CODE FOR IR ---------------

// --------------- CODE FOR LEDS ---------------
  /* REDUNDANT
   *  pinMode(buttonPin, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
   */


  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);    // Set power limit of LED strip to 5V, 1500mA
  FastLED.clear(); 
  FastLED.show();
// --------------- END CODE FOR LEDS ---------------

// --------------- CODE FOR SOUND ---------------
// REDUNDANT pinMode(buttonPin, INPUT_PULLUP);
// REDUNDANT pinMode(LED_BUILTIN, OUTPUT);
  
  mySoftwareSerial.begin(9600);                           // Initiate communication with the DFPlayer
  // REDUNDANT Serial.begin(115200);                                   // Mostly used for debugging
  Serial.println(F("Starting 2.00b Simon!"));             // Print to serial monitor that we're starting!       
  
  delay(1000);
  
  if (!myDFPlayer.begin(mySoftwareSerial, true, false)) {              // Use softwareSerial to communicate with mp3. If we have trouble,
    Serial.println(F("Unable to begin:"));                // print to the serial monitor so we can see.
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
  } else {
    Serial.println(F("DFPlayer Mini online."));          
  }
// --------------- END CODE FOR SOUND ---------------
}

uint8_t sCommand = 0x34;
uint8_t sRepeats = 4;

void loop() {
buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
      // IR
      sending(); // the address 0x0102 with the command 0x34 is sent
      // SOUND
       myDFPlayer.volume(20); // Set volume value. From 0 to 30
       playAudio(1); // the address 0x0102 with the command 0x34 is sent  
      // LEDS
       fadeAnimation(0, 0, 200);
      
      delay(1000); // wait for one second
  } 
}

// HELPER FOR LEDS
void fadeAnimation(int red, int green, int blue){
  float r, g, b;

  // FADE IN
  for(int i = 0; i <= 255; i++) {
    r = (i/256.0)*red;
    g = (i/256.0)*green;
    b = (i/256.0)*blue;
    fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    FastLED.show();
    delay(15);

  }

  // FADE OUT
  for(int i = 255; i >= 0; i--) {
    r = (i/256.0)*red;
    g = (i/256.0)*green;
    b = (i/256.0)*blue;
    fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    FastLED.show();
    delay(5);
  }
}

// HELPER FOR IR
void sending(){
    Serial.println();
    Serial.print(F("Send now: address=0x00, command=0x"));
    Serial.print(sCommand, HEX);
    Serial.print(F(", repeats="));
    Serial.print(sRepeats);
    Serial.println();

    Serial.println(F("Send standard NEC with 8 bit address"));
    Serial.flush();

    // Receiver output for the first loop must be: Protocol=NEC Address=0x102 Command=0x34 Raw-Data=0xCB340102 (32 bits)
    IrSender.sendNEC(0x102, sCommand, sRepeats);

    /*
     * Increment send values
     */
//    sCommand += 0x11;
    sRepeats++;
    // clip repeats at 4
    if (sRepeats > 4) {
        sRepeats = 4;
    }

    delay(1000);  // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
}

// HELPER FOR SOUND
void playAudio(int s) {
  myDFPlayer.play(s);
}
