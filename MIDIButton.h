class Button {
  public:
    Button(byte pin, byte note) {
      this -> pin = pin;
      this -> note = note;
    }
    byte pin;
    byte note;
    byte lastPlayedNote;
    boolean lastState;
    boolean newState;

    void playNote(byte sensorMode, byte potVal, byte sensorOctaveShift) {
      byte noteVelocity = sensorMode == 0 ? potVal : 127;
      byte noteToSend = note;
      if (sensorMode == 3) {
        noteToSend = note + 12 * sensorOctaveShift;
      }
      if (newState) {
        if (sensorMode == 3) {
          lastPlayedNote = noteToSend;
        }
        midiEventPacket_t noteOn = {0x09, 0x90, noteToSend, noteVelocity};
        MidiUSB.sendMIDI(noteOn);
        MidiUSB.flush();
      } else {
        if (sensorMode == 3) {
          lastPlayedNote = 0;
        }
        if (lastPlayedNote) {
          midiEventPacket_t noteOff = {0x08, 0x80, lastPlayedNote, noteVelocity};
        }
        midiEventPacket_t noteOff = {0x08, 0x80, noteToSend, noteVelocity};
        MidiUSB.sendMIDI(noteOff);
        MidiUSB.flush();
      }
    }
};
