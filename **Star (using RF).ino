#include <VirtualWire.h>
#include <IRremote.h>
#include <Wire.h> 
#include <LCDIC2.h>
#include <Adafruit_NeoPixel.h> // Include the neopixel library

int score = 0;
int RECV_PIN = 13; // Define the pin to which the IR receiver is connected
#define PIN 9 // For led lights
#define NUMPIXELS 16 // Popular NeoPixel ring size
int NUMS_LEDS = 8; // Number of LEDs in the strip
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

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
  pixels.begin();
  pixels.setBrightness(10);
  pixels.show();
}

// void changeLEDS(uint8_t val)
// {
//   for (int i = 0; i < NUM_LEDS; i++) // Loop through all the LEDs
//       {
//         digitalWrite(LED_PIN[i], val); // Turn on the current LED
//       }
// }

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
      Serial.println("Whoops waited too long");
      pixels.clear();
      pixels.show();
      reciever_on = false;
      last_off_time = now;
      next_wait_time = random(0, 5000);
    }
  }

  // If the reciever is off and the time difference between the last time it was turned off and now is greater than next_wait_time then turn on the reciver
  if (!reciever_on && now - last_off_time >= next_wait_time)
  {
    Serial.println("On and waiting for signal");
    pixels.fill(pixels.Color(255, 255, 0));
    pixels.show();
    reciever_on = true;
    last_on_time = now;
  }

  if (reciever_on && irrecv.decode(&results)) // Check if a signal ireceived
  {
    Serial.println("Signal received");
    if (results.value == 0xA7ADEA2B) // Check if the received signal matches the "turn on" code
    {
      send("Shot"); //RF transmitter
      Serial.println("Signal Sent");
      pixels.clear();
      pixels.show();
      next_wait_time = random(0, 5000);
      reciever_on = false;
      last_off_time = now;
    }
    irrecv.resume();
  }
}
