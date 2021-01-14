class Button {
  public:
    Button(byte pin, byte note) {
      this -> pin = pin;
      this -> note = note;
    }
    byte pin;
    byte note;
    boolean lastState;
    boolean newState;

    void playNote() {
      if (newState) {
        midiEventPacket_t noteOn = {0x09, 0x90, note, 100};
        MidiUSB.sendMIDI(noteOn);
        MidiUSB.flush();
      } else {
        midiEventPacket_t noteOff = {0x08, 0x80, note, 100};
        MidiUSB.sendMIDI(noteOff);
        MidiUSB.flush();
      }

    }
};
