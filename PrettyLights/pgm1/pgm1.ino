// Button hängt zwischen PB1 und PB2,
// LEDs an PB0.


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 0

#define BTN_INPUT 1
#define BTN_GND 2

volatile int mode = -1;//0 macht alles aus... 1-4 sind die Programme

boolean BACK_TO_MAIN = false;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(51, PIN, NEO_GRB + NEO_KHZ800);

#define DELAY_PGM1 10
#define DELAY_PGM2 130
#define DELAY_PGM3 200
#define DELAY_FLASH_PGM4 110





int leds_pgm1[] = {8,9,18,17, 11,12,13,14, 29,30,31,20, 26,25,24,23, 32,35,38,41,42,43,44,45,48,51};
/*
Programm 1.
jetzt : alle leds im Rainbow fade von: hell zu : sehr hell
Soll sich ändern auf fade von : dunkel( aus)l bis:  sehr hell und wieder fade out bis dunkel es leuchten NUr die folgenden leds:
Hose:
8,9,18,17
11,12,13,14
29,30,31,20
26,25,24,23
Top:
32,35,38,41,42,43,44,45,48,51

*/

void pgm1()
{
  //1-7 aus
  for (int i = 0; i < 7; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  //rainbow breathing (heller und dunkler)
  int count = sizeof(leds_pgm1)/sizeof(int);
  rainbow(leds_pgm1,DELAY_PGM1, count);
  if (BACK_TO_MAIN)
  {
    return;
  }
}


void setup()
{
  clock_prescale_set(clock_div_1);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}



void loop() {
    pgm1();
}



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow(int* _leds, uint8_t wait, int count) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<count; i++) {
      //strip.setPixelColor(lednumbers[_leds[i]], Wheel((i+j) & 255));
      strip.setPixelColor(_leds[i], Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
