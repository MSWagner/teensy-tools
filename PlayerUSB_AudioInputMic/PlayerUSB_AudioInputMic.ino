#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=360,206
AudioOutputUSB           usb1;           //xy=531,206
AudioConnection          patchCord1(i2s1, 0, usb1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=369,250
// GUItool: end automatically generated code

// which input on the audio shield will be used?
//const int myInput = AUDIO_INPUT_LINEIN;
const int myInput = AUDIO_INPUT_MIC;

void setup() {
  Serial.begin(9600);
  
  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory(60);

  // Enable the audio shield, select input, and enable output
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.volume(0.5);
  sgtl5000_1.micGain(0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
