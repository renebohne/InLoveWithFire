// Button hängt zwischen PB1 und PB2,
// LEDs an PB0.


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 0

#define BTN_INPUT 1
#define BTN_GND 2

Adafruit_NeoPixel strip = Adafruit_NeoPixel(51, PIN, NEO_GRB + NEO_KHZ800);

uint32_t tuerkis = strip.Color(0, 0x1F, 0x1F);  
uint32_t rot = strip.Color(200, 0, 0);
uint32_t orange = strip.Color(255, 105, 0);



volatile int mode = -1;//0 macht alles aus... 1-4 sind die Programme

boolean BACK_TO_MAIN = false;


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

int leds_pgm1[] = {8,9,18,17, 11,12,13,14, 29,30,31,20, 26,25,24,23, 32,35,38,41,42,43,44,45,48,51};
int pgm2_leds[] = { 19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,  32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,  20,21,22,23,24,25,26,27,28,29};
int pgm4_flash[] = { 7,6,5,4,3,2,1, 32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50, 51};
int pgm4_rainbowA[] = {32,33,37,38,51,50,46,45,42};
int pgm4_rainbowB[] = {34,35,36,39,41,49,48,47};


void clearLEDs()//alles aus
{
   //alles aus
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
}


void setup()
{
  clock_prescale_set(clock_div_1);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  pinMode(BTN_GND, OUTPUT);
  digitalWrite(BTN_GND, LOW);

  pinMode(BTN_INPUT, INPUT_PULLUP);

  cli();
  GIMSK = 0b00100000;    // turns on pin change interrupts
  PCMSK = 0b00000010;   // turn on interrupt on PB1
  sei();                 // enables interrupts


}



void loop() {
  if (BACK_TO_MAIN)
  {
    BACK_TO_MAIN = false;
  }
  if (mode == -1)
  {
    pgm0();
    mode = 0;
  }
  else if (mode == 1)
  {
    pgm1();
  }
  else if (mode == 2)
  {
    pgm2();
  }
  else if (mode == 3)
  {
    pgm3();
  }
  else if (mode == 4)
  {
    pgm4();
  }
  else
  {
   clearLEDs();
  }
}


void pgm0()
{
  //alle an
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 200));
  }
  strip.show();
  delay(1000);//warte 1 Sekunde (1000 Millisekunden)

  //alle aus
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

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

void _pgm3()
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


void pgm3()
{
    strip.clear();
    strip.show();
    _pgm3();    
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
void pgm4()
{
  
  rainbow(pgm4_rainbowA,1,sizeof(pgm4_rainbowA)/sizeof(int));
  clearLEDs();
  rainbow(pgm4_rainbowB,1,sizeof(pgm4_rainbowB)/sizeof(int));
      clearLEDs();
  
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
      strip.setPixelColor(mapLED(_leds[i]), Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
//variables to keep track of the timing of recent interrupts
volatile unsigned long button_time = 0;
volatile unsigned long last_button_time = 0;
//pin change interrupt was activated in setup()
ISR(PCINT0_vect)
{
  button_time = millis();

  if (button_time - last_button_time > 250)
  {

    int state = digitalRead(BTN_INPUT);
    if (state == LOW)
    {
      while (digitalRead(BTN_INPUT) == LOW)
      {
        //debounce
      }

      mode++;
      if (mode > 4)
      {
        mode = 0;
      }

      last_button_time = button_time;
      BACK_TO_MAIN = true;
    }
  }
}
