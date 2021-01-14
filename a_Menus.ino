#define settingsLength 3
#define sensorLength 4
#define scaleLength 14

Menu settingsMenuItems[settingsLength] = {
  Menu("Sensor Mode", 1, navigateToSubmenu),
  Menu("Scale", 2, navigateToSubmenu),
  Menu("Config", 2, navigateToSubmenu)
};

Menu sensorModeMenuItems[sensorLength] = {
  Menu("Velocity", 0, changeSetting),
  Menu("Mod Wheel", 0, changeSetting),
  Menu("Pitch Bend", 0, changeSetting),
  Menu("Octave Shift", 0, changeSetting)
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

Menu* allMenus[3] = {settingsMenuItems, sensorModeMenuItems, scaleMenuItems};

byte currentMenu = 0;
byte firstMenuItemIndex = 0;
byte currentSelectorPosition = 0;

byte menuLengths[3] = {settingsLength, sensorLength, scaleLength};
byte arraySize = menuLengths[currentMenu];

byte selectedItemBackground = 0x1335;
byte menuTextSize = 3;
byte menuItemOffset = 45;

void navigateToSubmenu(byte target) {
  currentMenu = target;
  firstMenuItemIndex = 0;
  currentSelectorPosition = 0;
}

void changeSetting(byte setting, byte value) {
  Serial.println("Changing setting");
  navigateToSubmenu(0);
}

void changeScale(byte scale) {
  currentScale = getSelectedItemIndex();
  Serial.println(currentScale);
  assignNotesToButtons(scales[currentScale].scale, scaleLengths[currentScale]);
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
