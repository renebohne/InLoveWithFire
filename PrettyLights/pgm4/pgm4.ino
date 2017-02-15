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

int pgm4_leds[] = { 19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,  32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,  20,21,22,23,24,25,26,27,28,29};




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

void pgm4()
{
  
  rainbow(pgm4_rainbowA,1);
  rainbow(pgm4_rainbowB,1);
      
  
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
        strip.setPixelColor(pgm4_flash[i-1], strip.Color(0, 0, 0)); //off
      }
      strip.setPixelColor(pgm4_flash[i], orange); //orange
 
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

void rainbow(int* _leds, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<sizeof(_leds)/sizeof(int); i++) {
      strip.setPixelColor(_leds[i], Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
