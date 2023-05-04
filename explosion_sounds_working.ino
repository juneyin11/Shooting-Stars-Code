/***************************************************

 2.00b Toobers! 
 
 JustSomeNoise.ino

 A quick test to make sure the DFPlayer is working
 
 Created for the 2023 MIT 2.00b Toy Product Design Class
 Questions?: hit us up on #ask-the-tas on Slack
 Cheers, [Victor Hung](vhung@mit.edu)
 MIT License
 
 Parts adapted from the DFPlayer sample,
 by [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 ****************************************************/

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#define buttonPin 11
int buttonState = 1;

SoftwareSerial mySoftwareSerial(4, 5);                    // Recieve Pin (RX), Transmit Pin (TX)
DFRobotDFPlayerMini myDFPlayer;                           // Create myDFPlayer of the DFPlayer Class

/*
 * setup
 * --
 * The setup() function is called when a sketch starts. Use it to initialize variables, pin modes, start using libraries, etc. 
 * This function will only run once, after each powerup or reset of the Arduino board.
 */
void setup() {

    pinMode(buttonPin, INPUT_PULLUP);
    
    pinMode(LED_BUILTIN, OUTPUT);
  
  mySoftwareSerial.begin(9600);                           // Initiate communication with the DFPlayer
  Serial.begin(115200);                                   // Mostly used for debugging
  Serial.println(F("Starting 2.00b Simon!"));             // Print to serial monitor that we're starting!       
  
  delay(1000);
  
  if (!myDFPlayer.begin(mySoftwareSerial, true, false)) {              // Use softwareSerial to communicate with mp3. If we have trouble,
    Serial.println(F("Unable to begin:"));                // print to the serial monitor so we can see.
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
  } else {
    Serial.println(F("DFPlayer Mini online."));          
  }
  
                                             // Play the first mp3! Exciting stuff
}

void loop() {                                             // Empty for this test, as we aren't taking in any feedback

  buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
    // turn LED on:
      Serial.print("in loop");
      myDFPlayer.volume(30);                                  // Set volume value. From 0 to 30
      playAudio(1); // the address 0x0102 with the command 0x34 is sent 
      delay(1000); // wait for one second
  } 
}

void playAudio(int s) {
  myDFPlayer.play(s);
}
