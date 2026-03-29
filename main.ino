#define RGB_RED_PIN 11
#define RGB_BLUE_PIN 10
#define RGB_GREEN_PIN 9

#define BUZZER_PIN 3

#define POT_A0 A0  // speed knob
#define POT_A1 A1  // color shift knob
#define POT_A2 A2  // brightness/volume knob

#include "pitches.h"

float t = 0;

//PLACE YOUR MELODY AND DURATIONS HERE-----------------
int melody[] = {
  NOTE_G4, NOTE_AS4, NOTE_C5, NOTE_D5, NOTE_D5, NOTE_C5, NOTE_DS5, NOTE_D5,
  NOTE_D5, NOTE_C5, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_C5, NOTE_D5,

  NOTE_D5, NOTE_D5, NOTE_DS5, NOTE_D5, NOTE_DS5,
  NOTE_DS5, NOTE_DS5, NOTE_C5, NOTE_DS5, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_D5, NOTE_AS4, NOTE_D5,
  NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_A4, NOTE_A4, NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_AS4, NOTE_G4, NOTE_AS4,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_F4,

  NOTE_C5,
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_D5,
  NOTE_F5, NOTE_D5, NOTE_F5, NOTE_F5, NOTE_D5, NOTE_F5, NOTE_D5, NOTE_D5, NOTE_C5, NOTE_D5,
  NOTE_C5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_AS4, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_D5,

  NOTE_DS5,
  NOTE_DS5, NOTE_DS5, NOTE_C5, NOTE_DS5, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_D5,
  NOTE_AS4, NOTE_D5, NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_A4, NOTE_A4, NOTE_AS4, NOTE_A4,
  NOTE_AS4, NOTE_AS4, NOTE_G4, NOTE_AS4, NOTE_G4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_F4,

  REST
};

int durations[] = {
  4,3,4,4,2,3,4,1,
  3,4,2,4,3,4,1,
  3,4,1,

  4,3,4,4,4,
  8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,

  4,
  8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,

  4,
  8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,

  1
};
//------------------------------------------------------------------------
int melodyLength = sizeof(melody) / sizeof(int);
void setRGB(int r, int g, int b) {
  analogWrite(RGB_RED_PIN, constrain(r, 0, 255));
  analogWrite(RGB_GREEN_PIN, constrain(g, 0, 255));
  analogWrite(RGB_BLUE_PIN, constrain(b, 0, 255));
}

void smoothRGB(int r, int g, int b) {
  static int lastR = 0, lastG = 0, lastB = 0;

  for (int i = 0; i < 8; i++) { 
    int sr = lastR + (r - lastR) * i / 8;
    int sg = lastG + (g - lastG) * i / 8;
    int sb = lastB + (b - lastB) * i / 8;

    setRGB(sr, sg, sb);
  }

  lastR = r;
  lastG = g;
  lastB = b;
}

void setup() {
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  for (int note = 0; note < melodyLength; note++) {

    int a0 = analogRead(POT_A0);
    int a1 = analogRead(POT_A1);
    int a2 = analogRead(POT_A2);

    float speed = map(a0, 0, 1023, 60, 300);
    float volume = a2 / 1023.0;
    int colorShift = map(a1, 0, 1023, 0, 255);

    int noteFreq = melody[note];
    int r = 0, g = 0, b = 0;

    if (noteFreq != REST) {

      int pitch = constrain(noteFreq, 200, 1000);
      int hue = map(pitch, 200, 1000, 0, 255);
      hue = (hue + colorShift) % 256;

      if (hue < 85) {
        r = 255 - hue * 3;
        g = hue * 3;
        b = 0;
      } else if (hue < 170) {
        hue -= 85;
        r = 0;
        g = 255 - hue * 3;
        b = hue * 3;
      } else {
        hue -= 170;
        r = hue * 3;
        g = 0;
        b = 255 - hue * 3;
      }
      int brightness = map(a2, 0, 1023, 180, 255);

      r = (r * brightness) / 255;
      g = (g * brightness) / 255;
      b = (b * brightness) / 255;
      float boost = 2.5;
      r = constrain(r * boost, 0, 255);
      g = constrain(g * boost, 0, 255);
      b = constrain(b * boost, 0, 255);
      float pulse = 0.7 + 0.3 * (sin(t * 6.28) + 1.0) / 2.0;
      r *= pulse;
      g *= pulse;
      b *= pulse;
    }

    smoothRGB(r, g, b);

    int duration = speed * (1000 / durations[note]) / 200;

    if (volume > 0.05 && noteFreq != REST) {
      tone(BUZZER_PIN, noteFreq, duration);
    }

    delay(duration * 1.1);
    noTone(BUZZER_PIN);

    t += 0.05;
Serial.print("Note:");Serial.print(noteFreq);Serial.print(" ");
Serial.print("R:"); Serial.print(r); Serial.print(" ");
Serial.print("G:"); Serial.print(g); Serial.print(" ");
Serial.print("B:"); Serial.print(b); Serial.print(" ");
Serial.print("A0:"); Serial.print(a0); Serial.print(" ");
Serial.print("A1:"); Serial.print(a1); Serial.print(" ");
Serial.print("A2:"); Serial.println(a2);
  }
}
