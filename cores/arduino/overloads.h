#ifdef CONFIG_DAC

void analogWrite(enum dacPins pinNumber, int value);

#endif

// In c++ mode, we also provide analogReadResolution and analogWriteResolution getters
int analogReadResolution();
int analogWriteResolution();