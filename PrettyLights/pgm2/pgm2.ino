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



int mapLED(int i)
{
  if(i<1)
  {
    return 0;
  }
  
  return i - 1;
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

uint32_t orange = strip.Color(255, 105, 0);
uint32_t blue = strip.Color(0, 0, 200);

int current_b = 0;

void pgm2()
{

  //oberteil:
  for (int idx = 0; idx < (sizeof(pgm2_leds) / sizeof(int)); idx ++)
  {
    
    strip.setPixelColor(mapLED(pgm2_leds[idx]), orange); //orange

    if (idx > 4)
    {
      strip.setPixelColor(mapLED(pgm2_leds[idx - 5]), strip.Color(0,0,0)); //off
    }

    if(idx>0)
    {
      strip.setPixelColor(mapLED(pgm2_leds[idx-1]), blue); //orange
    }
 
    if (BACK_TO_MAIN)
    {
      return;
    }
    strip.show();
    delay(DELAY_PGM2);
  }

  for (int idx = (sizeof(pgm2_leds) / sizeof(int))-5; idx < (sizeof(pgm2_leds) / sizeof(int)); idx ++)
  {
    strip.setPixelColor(mapLED(pgm2_leds[idx]), strip.Color(0,0,0)); //off
    strip.show();
    delay(DELAY_PGM2);
  }

  

  //dann ist man oben angekommen und nun geht es zurück. orange und aus
/*
  for (int idx = (sizeof(pgm2_leds) / sizeof(int) - 1); idx >= 0; idx -= 2)
  {
    strip.setPixelColor(pgm2_leds[idx] - 1, orange); //orange
    strip.setPixelColor(pgm2_leds[idx - 1] - 1, orange); //orange

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


