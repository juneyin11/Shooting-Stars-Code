#include <FastLED.h>
#define LED_PIN 9
#define NUM_LEDS 24

const int buttonPin = 11;
int buttonState = 1;

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);


  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);    // Set power limit of LED strip to 5V, 1500mA
  FastLED.clear(); 
  FastLED.show();
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

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == LOW) {
    Serial.println("turn on");
    fadeAnimation(0, 0, 200);
    delay(1000);
  }

//  else{
//    for (int i = 0; i < NUM_LEDS; i++){
//          leds[i] = CRGB(0, 0, 0);
//          FastLED.show();
//    delay(1000);
//  }
  }
    
