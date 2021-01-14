#define menusLength 6
#define settingsLength 5
#define sensorLength 4
#define scaleLength 14
#define startingNoteLength 12
#define startingOctaveLength 9
#define sensitivityLength 4

Menu settingsMenuItems[settingsLength] = {
  Menu("Sensor", 1, navigateToSubmenu),
  Menu("Mode", 2, navigateToSubmenu),
  Menu("Start Note", 3, navigateToSubmenu),
  Menu("Start Octave", 4, navigateToSubmenu),
  Menu("Sensitivity", 5, navigateToSubmenu)
};

Menu sensorModeMenuItems[sensorLength] = {
  Menu("Velocity", 0, changeSetting),
  Menu("Mod Wheel", 1, changeSetting),
  Menu("Pitch Bend", 2, changeSetting),
  Menu("Octave Shift", 3, changeSetting)
};

Menu scaleMenuItems[scaleLength] = {
  Menu("Chromatic", 0, changeScale),
  Menu("Ionian", 1, changeScale),
  Menu("Dorian", 2, changeScale),
  Menu("Phryigian", 3, changeScale),
  Menu("Lydian", 4, changeScale),
  Menu("Mixolydian", 5, changeScale),
  Menu("Aeolian", 6, changeScale),
  Menu("Locrian", 7, changeScale),
  Menu("Melodic Minor", 8, changeScale),
  Menu("Penta. Maj.", 9, changeScale),
  Menu("Penta. Min.", 10, changeScale),
  Menu("Whole Tone", 11, changeScale),
  Menu("Octa. 2, 1", 12, changeScale),
  Menu("Octa. 1, 2", 13, changeScale),
};

Menu startingNoteMenuItems[startingNoteLength] = {
  Menu("C", 0, changeStartingNote),
  Menu("Db", 1, changeStartingNote),
  Menu("D", 2, changeStartingNote),
  Menu("Eb", 3, changeStartingNote),
  Menu("E", 4, changeStartingNote),
  Menu("F", 5, changeStartingNote),
  Menu("Gb", 6, changeStartingNote),
  Menu("G", 7, changeStartingNote),
  Menu("Ab", 8, changeStartingNote),
  Menu("A", 9, changeStartingNote),
  Menu("Bb", 10, changeStartingNote),
  Menu("B", 11, changeStartingNote),
};

Menu startingOctaveMenuItems[startingOctaveLength] = {
  Menu("0", 0, changeStartingOctave),
  Menu("1", 1, changeStartingOctave),
  Menu("2", 2, changeStartingOctave),
  Menu("3", 3, changeStartingOctave),
  Menu("4", 4, changeStartingOctave),
  Menu("5", 5, changeStartingOctave),
  Menu("6", 6, changeStartingOctave),
  Menu("7", 7, changeStartingOctave),
  Menu("8", 8, changeStartingOctave),
};

Menu sensitivityMenuItems[sensitivityLength] = {
  Menu("Low", 0, changeSensitivity),
  Menu("Medium", 1, changeSensitivity),
  Menu("High", 2, changeSensitivity),
  Menu("Very High", 3, changeSensitivity),
};

Menu* allMenus[menusLength] = {settingsMenuItems, sensorModeMenuItems, scaleMenuItems, startingNoteMenuItems, startingOctaveMenuItems, sensitivityMenuItems};

byte currentMenu = 0;
byte firstMenuItemIndex = 0;
byte currentSelectorPosition = 0;

byte menuLengths[menusLength] = {settingsLength, sensorLength, scaleLength, startingNoteLength, startingOctaveLength, sensitivityLength};
byte arraySize = menuLengths[currentMenu];

byte selectedItemBackground = 0x1335;
byte menuTextSize = 3;
byte menuItemOffset = 45;

void navigateToSubmenu(byte target) {
  currentMenu = target;
  firstMenuItemIndex = 0;
  currentSelectorPosition = 0;
}

void changeSetting(byte setting) {
  sensorMode = setting;
  Serial.println(setting);
  navigateToSubmenu(0);
}

void changeScale(byte note) {
  currentScale = getSelectedItemIndex();
  assignNotesToButtons(currentStartingNote, currentStartingOctave, scales[currentScale].scale, scaleLengths[currentScale]);
  navigateToSubmenu(0);
}

void changeStartingNote(byte note) {
  currentStartingNote = note;
  assignNotesToButtons(currentStartingNote, currentStartingOctave, scales[currentScale].scale, scaleLengths[currentScale]);
  navigateToSubmenu(0);
}

void changeStartingOctave(byte octave) {
  currentStartingOctave = octave;
  assignNotesToButtons(currentStartingNote, currentStartingOctave, scales[currentScale].scale, scaleLengths[currentScale]);
  navigateToSubmenu(0);
}

void changeSensitivity(byte setting) {
  currentSensitivity = setting;
  assignNotesToButtons(currentStartingNote, currentStartingOctave, scales[currentScale].scale, scaleLengths[currentScale]);
  navigateToSubmenu(0);
}

byte getSelectedItemIndex() {
  byte cursorPosition = currentSelectorPosition / menuItemOffset;
  byte indexOfFirstShownElement = getIndexOfFirstShownElement();
  return cursorPosition + indexOfFirstShownElement;
}

void drawSelector() {
  tft.fillRect(0, currentSelectorPosition, 240, 45, selectedItemBackground);
  tft.drawRect(0, currentSelectorPosition, 240, 45, selectedItemBackground);
}

void drawBackground(uint16_t color) {
  tft.fillRect(-1, -1, 242, 137, color);
  tft.drawRect(-1, -1, 242, 137, color);
}

byte getIndexOfFirstShownElement() {
  return firstMenuItemIndex + 2 <= arraySize ?  firstMenuItemIndex  : arraySize - 3;
}

void drawMenu(uint16_t color) {
  drawBackground(ST77XX_BLACK);
  drawSelector();
  byte offsetCounter = 0;
  byte indexOfFirstShownElement = getIndexOfFirstShownElement();

  for (byte i = indexOfFirstShownElement; i < arraySize; i++) {
    tft.setCursor(5, offsetCounter * menuItemOffset + 10);
    tft.setTextColor(color);
    tft.setTextSize(menuTextSize);
    tft.print(allMenus[currentMenu][i].menuName);
    offsetCounter++;
  }
}

boolean lastButtonState1 = false;
boolean newButtonState1 = false;

boolean lastButtonState2 = false;
boolean newButtonState2 = false;

boolean lastButtonState3 = false;
boolean newButtonState3 = false;

void ManageNavigationButtons() {
  newButtonState1 = digitalRead(menuUp);
  if (newButtonState1 != lastButtonState1) {
    if (newButtonState1) {
      if (currentSelectorPosition == menuItemOffset * 2 && firstMenuItemIndex < arraySize - 3) {
        firstMenuItemIndex++;
      }

      if (currentSelectorPosition < menuItemOffset * 2) {
        currentSelectorPosition += menuItemOffset;
      }

      drawMenu(ST77XX_WHITE);
    }

    lastButtonState1 = newButtonState1;
  }

  newButtonState2 = digitalRead(menuSelect);
  if (newButtonState2 != lastButtonState2) {
    if (newButtonState2) {
      Menu currentItem = allMenus[currentMenu][getSelectedItemIndex()];
      //Serial.println(allMenus[currentMenu][getSelectedItemIndex()].menuName);
      currentItem.onAction(currentItem.submenuTarget);
      arraySize = menuLengths[currentMenu];
      drawMenu(ST77XX_WHITE);
    }

    lastButtonState2 = newButtonState2;
  }


  newButtonState3 = digitalRead(menuDown);
  if (newButtonState3 != lastButtonState3) {
    if (newButtonState3) {
      if (currentSelectorPosition == 0 && firstMenuItemIndex > 0) {
        firstMenuItemIndex--;
      }

      if (currentSelectorPosition > 0) {
        currentSelectorPosition -= menuItemOffset;
      }

      drawMenu(ST77XX_WHITE);
    }
    lastButtonState3 = newButtonState3;
  }
}
