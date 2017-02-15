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

int orange = strip.Color(255, 105, 0);



/*
programm 4

Fängt an mit Lauflicht farblich wie bisher. Immer nur 1-2 lichter an
lauflicht über
Schenkelblitz:
7,6,5,4,3,2,1 dann vorne im Kostüm hoch und hinten wieder runter,
32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50. 51.
erst wenn das letzte licht bei 51 im Top hinten angekommen ist fängt das Rainbow pulsing an

es pulsen rainbowfarbein bei hose, es wechseln sich pulsierend ab Sektion A und B:
A:
8,9,18,17
11,12,13,14
B:
29,30,31,20
26,25,24,23

pulsing im Top, es wechseln sich pulsierend ab Sektion A und B
A:
32,33,37,38,51,50,46,45,42

dann Sektion B
34,35,36,39,41,49,48,47,
mit feurigen Grüßen

*/

int pgm4_flash[] = { 7,6,5,4,3,2,1, 32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50, 51};
int pgm4_rainbowA[] = {32,33,37,38,51,50,46,45,42};
int pgm4_rainbowB[] = {34,35,36,39,41,49,48,47};

int mapLED(int i)
{
  if(i<1)
  {
    return 0;
  }
  
  return i - 1;
}

void pgm4()
{
  
  rainbow(pgm4_rainbowA,1,sizeof(pgm4_rainbowA)/sizeof(int));
  rainbow(pgm4_rainbowB,1,sizeof(pgm4_rainbowB)/sizeof(int));
      
  
  if (BACK_TO_MAIN)
  {
    return;
  }
  //6 Sekunden sind um
 
  //7 orange, dann 6 orange... das läuft hoch: 7,6,5,4,3,2,1, [32,51], [33,50], [34,49],[35,48],[36,47],[37,46],[38,45][39,44],[40,43],[41,42]
  for (int i = 0; i<sizeof(pgm4_flash)/sizeof(int);i++)
  {
      if(i>0)
      {
        strip.setPixelColor(mapLED(pgm4_flash[i-1]), strip.Color(0, 0, 0)); //off
      }
      strip.setPixelColor(mapLED(pgm4_flash[i]), orange); //orange
 
    if (BACK_TO_MAIN)
    {
      return;
    }
    strip.show();
    delay(DELAY_FLASH_PGM4);
  }

}

void setup()
{
  clock_prescale_set(clock_div_1);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}



void loop() {
    pgm4();
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
