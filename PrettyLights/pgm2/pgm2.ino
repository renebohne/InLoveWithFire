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



//das sind die Nummern der LEDs von unten nach oben - natürliche Zähleweise von 1 an
int lednumbers[] = {11, 26, 10, 12, 25, 27, 9, 13, 24, 28, 8, 14, 23, 29, 19, 15, 22, 30, 18, 16, 21, 31, 17, 20, 32, 51, 50, 33, 49, 34, 48, 35, 47, 36, 46, 37, 45, 38, 44, 39, 43, 40, 42, 41};

//das sind die Anzahlen der LEDs pro Layer (eine Reihe)
int lengths[] = {2, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

//das sind die Anzahlen der LEDs pro Etage für den Regenbogen(mehrere Reihen möglich)
int length_etagen[] = {10, 12, 6, 4, 4, 4, 3};

void layerByLayer()
{
  int lednumbers_idx = 0;

  for (int i = 0; i < (sizeof(lengths) / sizeof(int)); i++)
  {
    for (int j = 0; j < lengths[i]; j++)
    {
      strip.setPixelColor(lednumbers[lednumbers_idx + j], strip.Color(0, 0, 200));
    }
    strip.show();
    lednumbers_idx += lengths[i];
  }
}

int pgm2_leds[] = { 19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,  32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,  20,21,22,23,24,25,26,27,28,29};

/*

Programm 2:

super ist das lauflicht blau über orange das rot vor glüht!

farbe bleibt gleich, das lauflicht startet aber nicht bei 7 sondern bei:
Bein1:
 19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1
ins oberteil vorne über schulter oben und hinten wieder runter :

32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51
weiter zum anderen Bein:
20,21,22,23,24,25,26,27,28,29
weiter in bein1
19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1
ins oberteil vorne über schulter oben und hinten wieder runter :

32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51
weiter zum anderen Bein:
20,21,22,23,24,25,26,27,28,29

Die leds wandern hoch und gehen wieder aus, es sind immer nur ca 5 leds an.

*/

void pgm2()
{

  //oberteil:
  for (int idx = 0; idx < (sizeof(pgm2_leds) / sizeof(int)); idx += 2)
  {
    strip.setPixelColor(pgm2_leds[idx] - 1, strip.Color(255, 105, 0)); //orange
    strip.setPixelColor(pgm2_leds[idx + 1] - 1, strip.Color(255, 105, 0)); //orange

    if (idx > 24)
    {
      strip.setPixelColor(pgm2_leds[idx - 1] - 1, strip.Color(0, 0, 200)); //blue
      strip.setPixelColor(pgm2_leds[idx - 2] - 1, strip.Color(0, 0, 200)); //blue
    }
    if (BACK_TO_MAIN)
    {
      return;
    }
    strip.show();
    delay(DELAY_PGM2);
  }

/*
  //dann ist man oben angekommen und nun geht es zurück. orange und aus

  for (int idx = (sizeof(pgm2_leds) / sizeof(int) - 1); idx >= 0; idx -= 2)
  {
    strip.setPixelColor(pgm2_leds[idx] - 1, strip.Color(255, 105, 0)); //orange
    strip.setPixelColor(pgm2_leds[idx - 1] - 1, strip.Color(255, 105, 0)); //orange

    if (idx < (sizeof(pgm2_leds) / sizeof(int)) - 1 )
    {
      strip.setPixelColor(pgm2_leds[idx + 1] - 1, strip.Color(0, 0, 0)); //off
      strip.setPixelColor(pgm2_leds[idx + 2] - 1, strip.Color(0, 0, 0)); //off
    }
    if (BACK_TO_MAIN)
    {
      return;
    }
    strip.show();
    delay(DELAY_PGM2);
  }
*/

}







void setup()
{
  clock_prescale_set(clock_div_1);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}



void loop() {
    pgm2();
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

void rainbow(int* _leds, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<sizeof(_leds)/sizeof(int); i++) {
      strip.setPixelColor(lednumbers[_leds[i]], Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
