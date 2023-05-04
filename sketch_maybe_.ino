#include <Arduino.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "PinDefinitionsAndMore.h" // Define macros for input and output pin etc.
#include <IRremote.hpp>
#include <FastLED.h>

#define buttonPin 11
#define LED_PIN 9

#define NUM_LEDS 24
int buttonState = 1;

CRGB leds[NUM_LEDS];

SoftwareSerial mySoftwareSerial(3, 2);                    // Recieve Pin (RX), Transmit Pin (TX)
DFRobotDFPlayerMini myDFPlayer; 

void setup() {
  // put your setup code here, to run once:
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);

    mySoftwareSerial.begin(9600);

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

//    delay(1000);
  
    if (!myDFPlayer.begin(mySoftwareSerial, true, false)) {              // Use softwareSerial to communicate with mp3. If we have trouble,
      Serial.println(F("Unable to begin:"));                // print to the serial monitor so we can see.
      Serial.println(F("1.Please recheck the connection!"));
      Serial.println(F("2.Please insert the SD card!"));
    } else {
      Serial.println(F("DFPlayer Mini online."));          
    }

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);    // Set power limit of LED strip to 5V, 1500mA
    FastLED.clear(); 
    FastLED.show();

}

uint8_t sCommand = 0x34;
uint8_t sRepeats = 4;

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

void playAudio(int s) {
  myDFPlayer.play(s);
}

void fadeAnimation(int red, int green, int blue){
  float r, g, b;

  // FADE IN
  for(int i = 0; i <= 255; i++) {
    r = (i/256.0)*red;
    g = (i/256.0)*green;
    b = (i/256.0)*blue;
    fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    FastLED.show();
    delay(10);

  }

  // FADE OUT
  for(int i = 255; i >= 0; i--) {
    r = (i/256.0)*red;
    g = (i/256.0)*green;
    b = (i/256.0)*blue;
    fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
    FastLED.show();
    delay(3);
  }
}


void loop() {
  // put your main code here, to run repeatedly:
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
    // turn LED on:
//      Serial.print(buttonState);
      sending(); // the address 0x0102 with the command 0x34 is sent
      myDFPlayer.volume(30);                                  // Set volume value. From 0 to 30
      playAudio(1); // the address 0x0102 with the command 0x34 is sent 
      Serial.println("turn on");
      fadeAnimation(0, 0, 200);//BLUE
      delay(1000); // wait for one second
  } 
}
