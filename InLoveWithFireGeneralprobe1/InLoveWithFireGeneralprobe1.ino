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


//Etage
//(ganz unten) A 24,25,26,27,28   9,10,11,12,13
//B 21,22,23,29,30,31,  8,14,15,16,18,19
//C 17,20,32,33,51,50
//D 34,35,49,48
//E 37,38,47,46
//F 39,40,45,44
//G 41,42,43

//rainbow für etagen
//rot,orange,gelb,grün,blau,indogo,violett

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



void setup()
{
  clock_prescale_set(clock_div_1);
  pinMode(BTN_GND, OUTPUT);
  digitalWrite(BTN_GND, LOW);

  pinMode(BTN_INPUT, INPUT_PULLUP);

  GIMSK = 0b00100000;    // turns on pin change interrupts
  PCMSK = 0b00000010;   // turn on interrupt on PB1
  sei();                 // enables interrupts

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
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
    //alles aus
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
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

void pgm1()
{
  //1-7 aus
  for (int i = 0; i < 7; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
  //rainbow breathing (heller und dunkler)
  rainbowCycle(DELAY_PGM1);
  if (BACK_TO_MAIN)
  {
    return;
  }
}

void pgm2()
{
  //8-31 dauernd an
  for (int i = 7; i < 31; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 200));
  }
  strip.show();
  if (BACK_TO_MAIN)
  {
    return;
  }

  //7 orange, dann wird 7 blau, dafür 6 orange... das läuft hoch: 7,6,5,4,3,2,1, [32,51], [33,50], [34,49],[35,48],[36,47],[37,46],[38,45][39,44],[40,43],[41,42]
  for (int i = 6; i > -1; i--)
  {
    for (int j = 0; j < 7; j++)
    {
      if (i == j)
      {
        strip.setPixelColor(j, strip.Color(255, 105, 0)); //orange
      }
      else if (i < j)
      {
        strip.setPixelColor(j, strip.Color(0, 0, 200)); //blue
      }
      else
      {
        strip.setPixelColor(j, strip.Color(0, 0, 0)); //off
      }
    }
    if (BACK_TO_MAIN)
    {
      return;
    }
    strip.show();
    delay(DELAY_PGM2);
  }

  //special leds blau
  for (int i = 0; i < 7; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 200)); //blau
  }


  //oberteil:
  for (int idx = 24; idx < (sizeof(lednumbers) / sizeof(int)); idx += 2)
  {
    strip.setPixelColor(lednumbers[idx] - 1, strip.Color(255, 105, 0)); //orange
    strip.setPixelColor(lednumbers[idx + 1] - 1, strip.Color(255, 105, 0)); //orange

    if (idx > 24)
    {
      strip.setPixelColor(lednumbers[idx - 1] - 1, strip.Color(0, 0, 200)); //blue
      strip.setPixelColor(lednumbers[idx - 2] - 1, strip.Color(0, 0, 200)); //blue
    }
    if (BACK_TO_MAIN)
    {
      return;
    }
    strip.show();
    delay(DELAY_PGM2);
  }

  //dann ist man oben angekommen und nun geht es zurück. orange und aus

  for (int idx = (sizeof(lednumbers) / sizeof(int) - 1); idx >= 24; idx -= 2)
  {
    strip.setPixelColor(lednumbers[idx] - 1, strip.Color(255, 105, 0)); //orange
    strip.setPixelColor(lednumbers[idx - 1] - 1, strip.Color(255, 105, 0)); //orange

    if (idx < (sizeof(lednumbers) / sizeof(int)) - 1 )
    {
      strip.setPixelColor(lednumbers[idx + 1] - 1, strip.Color(0, 0, 0)); //off
      strip.setPixelColor(lednumbers[idx + 2] - 1, strip.Color(0, 0, 0)); //off
    }
    if (BACK_TO_MAIN)
    {
      return;
    }
    strip.show();
    delay(DELAY_PGM2);
  }
  //untersten beiden LEDs vom Oberteil ausschalten
  strip.setPixelColor(lednumbers[25] - 1, strip.Color(0, 0, 0)); //off
  strip.setPixelColor(lednumbers[24] - 1, strip.Color(0, 0, 0)); //off


  for (int i = 0; i < 7; i++)
  {
    strip.setPixelColor(i, strip.Color(255, 105, 0)); //orange
    if (i > 0)
    {
      strip.setPixelColor(i - 1, strip.Color(0, 0, 0)); //off
    }
    if (BACK_TO_MAIN)
    {
      return;
    }
    strip.show();
    delay(DELAY_PGM2);
  }
}

void pgm3()
{
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
  //von unten nach oben innerhalb von 5 Sekunden, dann aus und von vorne starten
  int lednumbers_idx = 0;

  for (int i = 0; i < (sizeof(lengths) / sizeof(int)); i++)
  {
    for (int j = 0; j < lengths[i]; j++)
    {
      strip.setPixelColor(lednumbers[lednumbers_idx + j], strip.Color(0, 0, 200));
    }
    if (BACK_TO_MAIN)
    {
      return;
    }
    strip.show();
    lednumbers_idx += lengths[i];
    delay(DELAY_PGM3);
  }
}

void pgm4()
{
  //programm 2 vom letzten mal (cycling rainbow)
  //und zusätzlich 7-1 orangenen Blitz von unten nach oben
  //dann blitz direkt auf Oberteil weiter
  //und dann weiter Rainbow ohne 7-1

  //den Blitz alle 6 Sekunden machen

  rainbowCycle(1);
  if (BACK_TO_MAIN)
  {
    return;
  }
  //6 Sekunden sind um

  //7 orange, dann 6 orange... das läuft hoch: 7,6,5,4,3,2,1, [32,51], [33,50], [34,49],[35,48],[36,47],[37,46],[38,45][39,44],[40,43],[41,42]
  for (int i = 6; i > -1; i--)
  {
    for (int j = 0; j < 7; j++)
    {
      if (i == j)
      {
        strip.setPixelColor(j, strip.Color(255, 105, 0)); //orange
      }
      else
      {
        strip.setPixelColor(j, strip.Color(0, 0, 0)); //off
      }
    }
    if (BACK_TO_MAIN)
    {
      return;
    }
    strip.show();
    delay(DELAY_FLASH_PGM4);
  }

  //special leds aus
  for (int i = 0; i < 7; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); //aus
  }


  //oberteil:
  for (int idx = 24; idx < (sizeof(lednumbers) / sizeof(int)); idx += 2)
  {
    strip.setPixelColor(lednumbers[idx] - 1, strip.Color(255, 105, 0)); //orange
    strip.setPixelColor(lednumbers[idx + 1] - 1, strip.Color(255, 105, 0)); //orange

    if (idx > 24)
    {
      strip.setPixelColor(lednumbers[idx - 1] - 1, strip.Color(0, 0, 0)); //aus
      strip.setPixelColor(lednumbers[idx - 2] - 1, strip.Color(0, 0, 0)); //aus
    }
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


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels() - 7; i++) {
      strip.setPixelColor(lednumbers[i] - 1, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      if (BACK_TO_MAIN)
      {
        return;
      }
    }
    strip.show();
    delay(wait);
    if (BACK_TO_MAIN)
    {
      return;
    }
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

