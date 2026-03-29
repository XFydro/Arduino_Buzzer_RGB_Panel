#include "pitches.h"
#define RGB_RED_PIN 11
#define RGB_BLUE_PIN 10
#define RGB_GREEN_PIN 9

#define BUZZER_PIN 3

#define POT_A0 A0
#define POT_A1 A1
#define POT_A2 A2
float t = 0;

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
  4, 3, 4, 4, 2, 3, 4, 1,
  3, 4, 2, 4, 3, 4, 1,
  3, 4, 1,

  4, 3, 4, 4, 4,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8,

  4,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8,

  4,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8,

  1
};

int melodyLength = sizeof(melody) / sizeof(int);

void setup()
{
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);
}

void setRGB(int r, int g, int b)
{
  analogWrite(RGB_RED_PIN, constrain(r, 0, 255));
  analogWrite(RGB_GREEN_PIN, constrain(g, 0, 255));
  analogWrite(RGB_BLUE_PIN, constrain(b, 0, 255));
}

void loop()
{

  for (int note = 0; note < melodyLength; note++) {

    int a0 = analogRead(POT_A0);
    int a1 = analogRead(POT_A1);
    int a2 = analogRead(POT_A2);

    float speed = map(a0, 0, 1023, 50, 400);
    float volume = a2 / 1023.0;

    t += speed / 1000.0;
    int noteFreq = melody[note];

    int r = 0, g = 0, b = 0;

    if (noteFreq != REST) {

      int pitch = constrain(noteFreq, 200, 1000);
      int hue = map(pitch, 200, 1000, 0, 255);

      if (hue < 85) {
        r = 255 - hue * 3;
        g = hue * 3;
        b = 0;
      }
      else if (hue < 170) {
        hue -= 85;
        r = 0;
        g = 255 - hue * 3;
        b = hue * 3;
      }
      else {
        hue -= 170;
        r = hue * 3;
        g = 0;
        b = 255 - hue * 3;
      }
      int brightness = map(noteFreq, 200, 1000, 80, 255);
      brightness = constrain(brightness, 0, 255);

      r = (r * brightness) / 255+50;
      g = (g * brightness) / 255+50;
      b = (b * brightness) / 255 +50;
    }

    setRGB(r, g, b);
    int duration = 1000 / durations[note];

    if (volume > 0.05 && melody[note] != REST) {
      tone(BUZZER_PIN, melody[note], duration);
    }

    delay(duration * 1.30);
    noTone(BUZZER_PIN);

Serial.print("Note:"); Serial.print(noteFreq); Serial.print(" ");
Serial.print("R:"); Serial.print(r); Serial.print(" ");
Serial.print("G:"); Serial.print(g); Serial.print(" ");
Serial.print("B:"); Serial.print(b); Serial.print(" ");
Serial.print("A0:"); Serial.print(a0); Serial.print(" ");
Serial.print("A1:"); Serial.print(a1); Serial.print(" ");
Serial.print("A2:"); Serial.println(a2);
  }
}
