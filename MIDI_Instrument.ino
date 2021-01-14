#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>
#include "MIDIButton.h"
#include "Menu.h"
#include "Scales.h"
#include "StartingNote.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define TFT_CS         18
#define TFT_RST        19
#define TFT_DC         20

#define menuUp 23
#define menuSelect 22
#define menuDown 21
#define sensorPin 6

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

byte startingNote = 60;

byte pins[] = {5, 6, 7, 8, 9, 10, 11, 12};
Button *buttons[sizeof(pins)];

byte chromatic[] = {1};
byte ionian[] = {2, 2, 1, 2, 2, 2, 1};
byte dorian[] = {2, 1, 2, 2, 2, 1, 2};
byte phrygian[] = {1, 2, 2, 2, 1, 2, 2};
byte lydian[] = {2, 2, 2, 1, 2, 2, 1};
byte mixolydian[] = {2, 2, 1, 2, 2, 1, 2};
byte aeolian[] = {2, 1, 2, 2, 1, 2};
byte locrian[] = {1, 2, 2, 2, 1, 2, 2};
byte melodicMinor[] = {2, 1, 2, 2, 1, 3, 1};
byte pentatonicMajor[] = {2, 2, 3, 2, 3};
byte pentatonicMinor[] = {3, 2, 2, 3, 2};
byte wholeTone[] = {2};
byte octatonic21[] = {2, 1};
byte octatonic12[] = {1, 2};

Scale scales[14] = {
  Scale("Chromatic", chromatic),
  Scale("Ionian", ionian),
  Scale("Dorian", dorian),
  Scale("Phryigian", phrygian),
  Scale("Lydian", lydian),
  Scale("Mixolydian", mixolydian),
  Scale("Aeolian", aeolian),
  Scale("Locrian", locrian),
  Scale("Melodic Minor", melodicMinor),
  Scale("Pentatonic Major", pentatonicMajor),
  Scale("Pentatonic Minor", pentatonicMinor),
  Scale("Whole Tone", wholeTone),
  Scale("Octatonic 1-2", octatonic12),
  Scale("Octatonic 2-1", octatonic21),
};
byte scaleLengths[] = {1, 7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 1, 2, 2 };
byte currentScale = 0;
byte currentStartingNote = 0;
byte currentStartingOctave = 4;

byte sensorOctaveShift = 0;

// Sensor modes:
// 0 = Velocity
// 1 = Mod Wheel
// 2 = Pitch Bend
// 3 = Octave Shift
byte sensorMode = 1;
byte currentSensitivity = 0;
byte sensorSensitivities[4] = {200, 175, 150, 125};

uint8_t potVal;

int currentVal;
int nextVal;

void setup() {
  Serial.begin(9600);
  pinMode(menuUp, INPUT_PULLUP);
  pinMode(menuSelect, INPUT_PULLUP);
  pinMode(menuDown, INPUT_PULLUP);
  tft.init(135, 240);
  tft.setRotation(1);
  drawMenu(ST77XX_WHITE);
  assignNotesToButtons(currentStartingNote, currentStartingOctave, scales[currentScale].scale, scaleLengths[currentScale]);
}

void loop() {
  for (byte i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
    buttons[i]->newState = digitalRead(buttons[i]->pin);
    if (buttons[i]->lastState != buttons[i]->newState) {
      buttons[i]->playNote(sensorMode, potVal, sensorOctaveShift);
      buttons[i]->lastState = digitalRead(buttons[i]->pin);
    }
  }

  potVal = map(analogRead(sensorPin), 95, sensorSensitivities[currentSensitivity], 0, 127);
  currentVal = potVal;
  if (currentVal != nextVal) {
    midiEventPacket_t ccChange = {0x0B, 0xB0, 1, potVal};
    midiEventPacket_t pitchBendChange = {0x0B, 0xE0, 1, potVal};
    if (sensorMode == 1) {
      MidiUSB.sendMIDI(ccChange);
    }
    if (sensorMode == 2) {
      MidiUSB.sendMIDI(pitchBendChange);
    }
    if (sensorMode == 3) {
      sensorOctaveShift = map(analogRead(sensorPin), 95, sensorSensitivities[currentSensitivity], 0, 3);
    }
    MidiUSB.flush();
    nextVal = currentVal;
  }
  ManageNavigationButtons();
}
