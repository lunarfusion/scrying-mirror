
#include "FastLED.h"
#define COLOR_ORDER GRB
#define NUM_LEDS_STRIP    50  // Strip of WS2811_400 bulbs with fiber optics attached
#define DATA_PIN 6
CRGB stripleds[NUM_LEDS_STRIP];

#define NUM_LEDS_BUTTLIT    4 // Four buttons backlit with separate componant single LEDs
#define DATA_PIN_BUTTLIT 2
CRGB buttlitleds[NUM_LEDS_BUTTLIT];

#define NUM_STRIPS 2
uint8_t gBrightness = 100;

#define buttonPin1 8 // Pin for button1 - Red
#define buttonPin2 9 // Pin for button2 - Purple
#define buttonPin3 10 // Pin for button3 - Blue
#define buttonPin4 11 // Pin for button4 - Green

int buttState1 = 0; 
int buttState2 = 0; 
int buttState3 = 0; 
int buttState4 = 0; 

uint8_t  thisfade = 2;                                        // How quickly does it fade? Lower = slower fade rate.
int       thishue = 40;                                       // Starting hue.
uint8_t   thisinc = 1;                                        // Incremental value for rotating hues
uint8_t   thissat = 255;                                      // The saturation, where 255 = brilliant colours.
uint8_t   thisbri = 255;                                      // Brightness of a sequence. Remember, max_bright is the overall limiter.
int       huediff = 100;                                      // Range of random #'s to use for hue
uint8_t thisdelay = 5;      

void addGlitter( fract8 chanceOfGlitter);

void setup() {
  delay(1000); // sanity delay
  FastLED.addLeds<WS2811_400,DATA_PIN, RGB>(stripleds, NUM_LEDS_STRIP).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2812B,DATA_PIN_BUTTLIT, GRB>(buttlitleds, NUM_LEDS_BUTTLIT).setCorrection( TypicalLEDStrip );
  pinMode(DATA_PIN, OUTPUT); // Fiber optic strip
  pinMode(DATA_PIN_BUTTLIT, OUTPUT); //4 Button lights
  pinMode(buttonPin1, INPUT_PULLUP); //Red Button input
  pinMode(buttonPin2, INPUT_PULLUP); //Purple Button input
  pinMode(buttonPin3, INPUT_PULLUP); //Blue Button input
  pinMode(buttonPin4, INPUT_PULLUP); //Green Button input
  buttlitleds[0] = CHSV(0, 255, 125); //RED
  buttlitleds[1] = CHSV(210, 255, 255); //PURPLE
  buttlitleds[2]  = CHSV(170, 255, 150); //BLUE
  buttlitleds[3] = CHSV(90, 255, 200); //GREEN
  FastLED.show();
}

void loop() {
  buttState1 = digitalRead(buttonPin1);
  buttState2 = digitalRead(buttonPin2);
  buttState3 = digitalRead(buttonPin3);
  buttState4 = digitalRead(buttonPin4);

//All Buttons Turn Rainbow Confetti On
if (buttState1 == LOW && buttState2 == LOW && buttState3 == LOW && buttState4 == LOW) 
{  
  fadeToBlackBy(stripleds, NUM_LEDS_STRIP, thisfade);                    // Low values = slower fade.
  int pos = random16(NUM_LEDS_STRIP);                               // Pick an LED at random.
  stripleds[pos] += CHSV((thishue + random16(huediff))/4 , thissat, thisbri);  // I use 12 bits for hue so that the hue increment isn't too quick.
  thishue = thishue + thisinc;                                // It increments here.

} else if(buttState1 == LOW && buttState2 == HIGH && buttState3 == HIGH && buttState4 == HIGH)
{
  FadeInOut(255, 0x00, 0x00); //button 1 red
}
else if (buttState2 == LOW && buttState1 == HIGH && buttState3 == HIGH && buttState4 == HIGH) 
{
  FadeInOut(random(155,200), 0, random(200, 255)); //button 2 purple
} 
else if (buttState3 == LOW && buttState1 == HIGH && buttState2 == HIGH && buttState4 == HIGH) 
{
  FadeInOut(0, 0, 255); //button 3 blue
}
else if (buttState4 == LOW && buttState1 == HIGH && buttState2 == HIGH && buttState3 == HIGH) 
{
  FadeInOut(0x07, 0xcb, 0x00); //button 4 green
}
else
{
  fill_solid(stripleds, NUM_LEDS_STRIP, CRGB::Black); // No buttons - no fiber optic lights
  FastLED[1].showLeds(gBrightness);
}  
  FastLED.show(); 
}

//Fades the colors in and out on the 4 button events
void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
     
  for(int k = 0; k < 256; k=k+1) { 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
     addGlitter(100);
    FastLED.show(55);  
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    FastLED.show(20);  
  }
}


void Twinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS_STRIP),red,green,blue);
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) { 
       setAll(0,0,0); 
     }
   }
  
  delay(SpeedDelay);
}

void showStrip() {
   FastLED.show();
}

//Fades the colors in and out on the 4 button events
void setPixel(int Pixel, byte red, byte green, byte blue) {
   stripleds[Pixel].r = red;
   stripleds[Pixel].g = green;
   stripleds[Pixel].b = blue;
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS_STRIP; i++ ) {
    setPixel(i, red, green, blue); 
  }
  FastLED.show(55);  
}


//glitter effect
void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    stripleds[ random16(NUM_LEDS_STRIP) ] += CRGB::White;}
}
