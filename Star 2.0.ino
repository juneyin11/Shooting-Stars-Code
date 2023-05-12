#include <VirtualWire.h>
#include <IRremote.h>
#include <Wire.h> 
#include <Adafruit_NeoPixel.h> // Include the neopixel library

int score = 0;
int RECV_PIN = 13; // Define the pin to which the IR receiver is connected
#define PIN 9 // For led lights
#define NUMPIXELS 27 // Popular NeoPixel ring size
int NUMS_LEDS = 8; // Number of LEDs in the strip
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mySoftwareSerial(5, 4);                    // Recieve Pin (RX), Transmit Pin (TX)
DFRobotDFPlayerMini myDFPlayer;

// #define DELAYVAL 500 // Time (in milliseconds) to pause between pixels
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600); // Initialize the serial communication
  irrecv.enableIRIn(); // Enable the IR receiver
  // Initialize the IO and ISR for RF transmitter
  vw_setup(2000); // Bits per sec
  //LED stuff
  // Serial.begin(115200);                                   // Mostly used for debugging
  // Serial.println(F("Starting 2.00b Simon!"));             // Print to serial monitor that we're starting!   
  // delay(1000);
  
  // if (!myDFPlayer.begin(mySoftwareSerial, true, false)) {              // Use softwareSerial to communicate with mp3. If we have trouble,
  //   Serial.println(F("Unable to begin:"));                // print to the serial monitor so we can see.
  //   Serial.println(F("1.Please recheck the connection!"));
  //   Serial.println(F("2.Please insert the SD card!"));
  // } else {
  //   Serial.println(F("DFPlayer Mini online."));          
  // }
  // myDFPlayer.volume(30);
  //   // Play the first mp3! Exciting stuff
  pixels.begin();
  pixels.setBrightness(10);
  pixels.show();
}

void playAudio(int s) {
  myDFPlayer.play(s);
}

void send (char *message) //for  RF transmitter
{
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx(); // Wait until the whole message is gone
}


// Stuff not to touch
int last_on_time = 0;
int last_off_time = 0;
bool reciever_on = false;

// the amount of time in milliseconds to wait until reciver is turned on again, this is currently being randomly set in the loop function
int next_wait_time = 0;
// the amount of milliseconds that the reciever should stay on before turning back off
int max_on_time = 1000;

void loop()
{
  int now = millis();

  if (reciever_on){
    // If the time diff between now and the last time it was turned on is greater than max_on_time, turn off the LEDS and wait a random amount of time to turn back on
    if (now - last_on_time >= max_on_time){
      // Serial.println("Whoops waited too long");
      pixels.clear();
      pixels.show();
      reciever_on = false;
      last_off_time = now;
      next_wait_time = random(0, 5000);
    }
  }
  // Serial.println(now);
  // Serial.println(next_wait_time);
  // Serial.println("looping");

  // If the reciever is off and the time difference between the last time it was turned off and now is greater than next_wait_time then turn on the reciver
  if (!reciever_on && now - last_off_time >= next_wait_time)
  {
    // Serial.println("On and waiting for signal");
    pixels.fill(pixels.Color(255, 255, 0));
    pixels.show();
    reciever_on = true;
    last_on_time = now;
  }

  if (reciever_on && irrecv.decode(&results)) // Check if a signal ireceived
  {
    Serial.println("Signal received");
    if (irrecv.decode(&results)) // Check if the received signal matches the "turn on" code
    {
      Serial.println(results.value, HEX);
      Serial.println("Signal Sent"); //sometimes not working   
      // Serial.println("sound played");
      send("Shot"); //RF transmitter
      pixels.clear();
      pixels.show();
      next_wait_time = random(0, 5000);
      reciever_on = false;
      last_off_time = now;
      // playAudio(1); // the address 0x0102 with the command 0x34 is sent 
      // delay(5000);
    }
    irrecv.resume();
  }
}
