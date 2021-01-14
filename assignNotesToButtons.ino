void assignNotesToButtons(byte scale[], byte n) {

  byte scaleCounter = 0;
  byte currentNote = startingNote;
  for (byte i = 0; i < sizeof(pins); i++) {
    buttons[i] = new Button(pins[i], currentNote);
    currentNote += scale[scaleCounter];
    scaleCounter++;
    if (scaleCounter == n) {
      scaleCounter = 0;
    }
    pinMode(pins[i], INPUT_PULLUP);
  }
}

void findScaleAndAssign(byte scaleNumber) {
  
}
