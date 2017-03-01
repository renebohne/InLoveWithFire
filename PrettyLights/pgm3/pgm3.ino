// Button hängt zwischen PB1 und PB2,
// LEDs an PB0.


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(51, PIN, NEO_GRB + NEO_KHZ800);

#define DELAY_PGM1 10
#define DELAY_PGM2 130
#define DELAY_PGM3 200
#define DELAY_FLASH_PGM4 110



  uint32_t tuerkis = strip.Color(0, 0x1F, 0x1F);  
  uint32_t rot = strip.Color(200, 0, 0);


/*

Programm 3:

Farbe:grün/türkis. Hose füllt sich von unten nach oben in 2 parallelen Streifen auf.
es gehen gleichzeitig an und bleiben an:
8,11,29,26
dann dazu:
19,12,30,25
dann dazu:
18,13,31,24
dann dazu:
17,14,20,23

Dann zu oberteil,
32 und 51,
33 und 50,
34 und 49,
36 u 48,
37 und 47
38 und 46
39 und 45
40 und 44
41  und 43
42wird und bleibt rot


Programmierung Oberteil bleibt gleich  wie bisher aber  gleiche Farbe wie Hose, also läuft voll von vorn und hinten gleichzeitig nach oben/Schulter und geht dann aus, ausser 42, bleibt rot
1-2.sec Pause dann 2 mal hintereinander der schnelle Schenkelblitz von programm 2,
7,6,5,4,3,2,1 und nochmal  7,6,5,4,3,2,1
2 sec pause
danach wieder von Angfang an auffüllen ,wenns geht jetzt in einer anderen Farbe:, z.B 1 mal auffüllen in blau dann grün, und wieder türkis


*/

int mapLED(int i)
{
  if(i<1)
  {
    return 0;
  }
  
  return i - 1;
}

void schenkelblitz()
{
  
  strip.setPixelColor(mapLED(7), rot);
  strip.show();
  delay(DELAY_FLASH_PGM4);
  strip.setPixelColor(mapLED(6), rot);
  strip.show();
  delay(DELAY_FLASH_PGM4);
  strip.setPixelColor(mapLED(5), rot);
  strip.show();
  delay(DELAY_FLASH_PGM4);
  strip.setPixelColor(mapLED(4), rot);
  strip.show();
  delay(DELAY_FLASH_PGM4);
  strip.setPixelColor(mapLED(3), rot);
  strip.show();
  delay(DELAY_FLASH_PGM4);
  strip.setPixelColor(mapLED(2), rot);
  strip.show();
  delay(DELAY_FLASH_PGM4);
  strip.setPixelColor(mapLED(1), rot);
  strip.show();
  delay(DELAY_FLASH_PGM4);
}

void pgm3()
{
  /*
  //5 Sekunden alles aus
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();


  //5 Sekunden warten
  for (int i = 0; i < 20; i++)
  {
    delay(250);
    if (BACK_TO_MAIN)
    {
      return;
    }
  }
  */

  


  strip.setPixelColor(mapLED(8), tuerkis);
  strip.setPixelColor(mapLED(11), tuerkis);
  strip.setPixelColor(mapLED(29), tuerkis);
  strip.setPixelColor(mapLED(26), tuerkis);  
  strip.show();
  delay(DELAY_PGM3);

  strip.setPixelColor(mapLED(19), tuerkis);
  strip.setPixelColor(mapLED(12), tuerkis);
  strip.setPixelColor(mapLED(30), tuerkis);
  strip.setPixelColor(mapLED(25), tuerkis);  
  strip.show();
  delay(DELAY_PGM3);

  strip.setPixelColor(mapLED(18), tuerkis);
  strip.setPixelColor(mapLED(13), tuerkis);
  strip.setPixelColor(mapLED(31), tuerkis);
  strip.setPixelColor(mapLED(24), tuerkis);  
  strip.show();
  delay(DELAY_PGM3);


  strip.setPixelColor(mapLED(17), tuerkis);
  strip.setPixelColor(mapLED(14), tuerkis);
  strip.setPixelColor(mapLED(20), tuerkis);
  strip.setPixelColor(mapLED(23), tuerkis);  
  strip.show();
  delay(DELAY_PGM3);

  strip.setPixelColor(mapLED(32), tuerkis);
  strip.setPixelColor(mapLED(51), tuerkis);
  strip.show();
  delay(DELAY_PGM3);

  strip.setPixelColor(mapLED(33), tuerkis);
  strip.setPixelColor(mapLED(50), tuerkis);
  strip.show();
  delay(DELAY_PGM3);
  
  strip.setPixelColor(mapLED(34), tuerkis);
  strip.setPixelColor(mapLED(49), tuerkis);
  strip.show();
  delay(DELAY_PGM3);

  strip.setPixelColor(mapLED(36), tuerkis);
  strip.setPixelColor(mapLED(48), tuerkis);
  strip.show();
  delay(DELAY_PGM3);
  
  strip.setPixelColor(mapLED(37), tuerkis);
  strip.setPixelColor(mapLED(47), tuerkis);
  strip.show();
  delay(DELAY_PGM3);

  strip.setPixelColor(mapLED(38), tuerkis);
  strip.setPixelColor(mapLED(46), tuerkis);
  strip.show();
  delay(DELAY_PGM3);
  
  strip.setPixelColor(mapLED(39), tuerkis);
  strip.setPixelColor(mapLED(45), tuerkis);
  strip.show();
  delay(DELAY_PGM3);

  strip.setPixelColor(mapLED(40), tuerkis);
  strip.setPixelColor(mapLED(44), tuerkis);
  strip.show();
  delay(DELAY_PGM3);

  strip.setPixelColor(mapLED(41), tuerkis);
  strip.setPixelColor(mapLED(43), tuerkis);
  strip.show();
  delay(DELAY_PGM3);
  
  strip.setPixelColor(mapLED(42), rot);
  strip.show();
  delay(DELAY_PGM3);
  
}



void setup()
{
  //clock_prescale_set(clock_div_1);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}



void loop() {
    strip.clear();
    strip.show();
    pgm3();    
    delay(2000);
    strip.clear();
    strip.show();
    schenkelblitz();
    delay(200);
    strip.clear();
    strip.show();
    schenkelblitz();
    strip.clear();
    strip.show();
    delay(2000);
    
}



