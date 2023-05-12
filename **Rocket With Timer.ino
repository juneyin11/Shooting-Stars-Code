#include <VirtualWire.h>
#include <Wire.h> 
#include <Adafruit_NeoPixel.h> // Include the neopixel library

// byte message[VW_MAX_MESSAGE_LEN]; // a buffer to store the incoming messages
// byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message
int score = 0;
#define PIN 9 // For led lights
#define NUMPIXELS 16 // Popular NeoPixel ring size
int NUMS_LEDS = 20; // Number of LEDs in the strip MAKE SURE GROUND CONNECTED TO BOTH POWER SOURCE AND ARDUINO GROUND
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long startTime;
unsigned long endTime;
uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;

void setup()
{
  startTime = millis();
  Serial.begin(9600);
  Serial.println("Device is ready");
  // Initialize the IO and ISR for RF receiver
  vw_setup(2000); // Bits per sec
  vw_set_rx_pin(2); // Set the receive pin to pin 2
  vw_rx_start(); // Start the receiver
  pixels.begin();
  pixels.setBrightness(100);
  pixels.show();
   bool gameStart = false;
   Serial.println("Ready!");
  while (!gameStart){ // Start the timer when first shot
    if (vw_get_message(buf, &buflen)){ // Check if a message has been received from RF receiver{
      gameStart = true;
      startTime = millis();
      Serial.println("Go!");
    } 
   // Serial.println("loop");
  }
}

int gameTime = 200000000;
bool gameOver = false;


void loop()
{
  if (gameOver){
    return;
  }
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;
  unsigned long remainingTime = gameTime - elapsedTime;
  if(currentTime%10 == 0){
  // Serial.println(remainingTime);
  }
  if (vw_get_message(buf, &buflen)) // Check if a message has been received from RF receiver
  {
    Serial.println("Received: ");
    for (int i = 0; i < buflen; i++) 
    {
      Serial.write(buf[i]); // Print the received message
    }
    Serial.println();
    pixels.setPixelColor(score, 255, 0, 0);
    pixels.show();
    score += 1;
    Serial.println("Score updated: " + String(score));
    // sound effect for adding a light #001
  }
  if (remainingTime == gameTime/2){
    Serial.println("Time half over");
    //sound effect "30 seconds left"
  }
  if (score == NUMS_LEDS/2){
    Serial.println("Halfway There!!!");
  }
  if (score == NUMS_LEDS){
    Serial.println("Game Won");
    // sound effect for "Game Won" #002
    pixels.clear();
    delay(100);
    pixels.setPixelColor(NUMS_LEDS, 255, 0, 0);
    delay(100);

  }
  if (remainingTime <= 0) {
    endTime = millis();
    gameOver = true;
    Serial.println("Game Over");
    // here you would add the speaker to make sure that you lost. Probably a voiceover
  }    
}
