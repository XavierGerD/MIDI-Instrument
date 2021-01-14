void assignNotesToButtons(byte startingNote, byte startingOctave, byte scale[], byte n) {
  byte scaleCounter = 0;
  byte currentNote = findStartingPoint(startingNote, startingOctave);
  for (byte i = 0; i < numberOfButtonPins; i++) {
    buttons[i] = new Button(pins[i], currentNote);
    currentNote += scale[scaleCounter];
    scaleCounter++;
    if (scaleCounter == n) {
      scaleCounter = 0;
    }
    pinMode(pins[i], INPUT_PULLUP);
  }
}

byte findStartingPoint(byte startingNote, byte startingOctave) {
  return startingNote + 12 * startingOctave;
}
