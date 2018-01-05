#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputTDM            tdm1;           //xy=359,317
//AudioRecordQueue         queue13;        //xy=620,615
//AudioRecordQueue         queue14;        //xy=620,649
//AudioRecordQueue         queue15;        //xy=620,682
//AudioRecordQueue         queue12;        //xy=621,581
//AudioRecordQueue         queue16;        //xy=622,716
//AudioRecordQueue         queue9;         //xy=623,480
//AudioRecordQueue         queue10;        //xy=623,514
//AudioRecordQueue         queue11;        //xy=623,547
//AudioRecordQueue         queue4;         //xy=624,315
//AudioRecordQueue         queue5;         //xy=624,348
//AudioRecordQueue         queue6;         //xy=624,381
//AudioRecordQueue         queue7;         //xy=624,414
//AudioRecordQueue         queue3;         //xy=625,281
//AudioRecordQueue         queue8;         //xy=625,447
AudioRecordQueue         queue2;         //xy=626,248
AudioRecordQueue         queue1;         //xy=627,214
AudioOutputUSB           usb1;           //xy=629,126
AudioConnection          patchCord1(tdm1, 0, queue1, 0);
AudioConnection          patchCord2(tdm1, 0, usb1, 0);
AudioConnection          patchCord3(tdm1, 0, usb1, 1);
AudioConnection          patchCord4(tdm1, 1, queue2, 0);
//AudioConnection          patchCord5(tdm1, 2, queue3, 0);
//AudioConnection          patchCord6(tdm1, 3, queue4, 0);
//AudioConnection          patchCord7(tdm1, 4, queue5, 0);
//AudioConnection          patchCord8(tdm1, 5, queue6, 0);
//AudioConnection          patchCord9(tdm1, 6, queue7, 0);
//AudioConnection          patchCord10(tdm1, 7, queue8, 0);
//AudioConnection          patchCord11(tdm1, 8, queue9, 0);
//AudioConnection          patchCord12(tdm1, 9, queue10, 0);
//AudioConnection          patchCord13(tdm1, 10, queue11, 0);
//AudioConnection          patchCord14(tdm1, 11, queue12, 0);
//AudioConnection          patchCord15(tdm1, 12, queue13, 0);
//AudioConnection          patchCord16(tdm1, 13, queue14, 0);
//AudioConnection          patchCord17(tdm1, 14, queue15, 0);
//AudioConnection          patchCord18(tdm1, 15, queue16, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=369,479
// GUItool: end automatically generated code

void setup() {
  
  Serial.begin(9600);

  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory(60);

  // Enable the audio shield, select input, and enable output
  sgtl5000_1.enable();

  while(!Serial);
  Serial.println("Testing tdm1 channel 1 most significant 16 bits to USB");

}

void loop() {
  // put your main code here, to run repeatedly:

}
