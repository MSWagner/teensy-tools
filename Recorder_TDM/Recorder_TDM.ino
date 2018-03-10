// Record TDM input sound as raw data (16ch, Signed 24-bit little-endian PCM, 44.1kHz, little-endian) to a SD card.
// Note: Format is modified so that mics align their 24 bit data to 32 bit slots, half of the 16 bit
// channels end up being the 8 low bits, and 8 more zeros which are discarded.
//
// Hardware:
//   Mic Pin 6  (SD_O)  to Teensy Pin  9 SCLK  (Output, 11.3 MHz, Checked)
//   Mic Pin 8  (SCK_I) to Teensy Pin 13 SDATA (Input, 11.3 Mbit/sec)
//   Mic Pin 10 (WS_I)  to Teensy Pin 23 FS    (Output, 44100 Hz, Checked)
//
//   Mic GND Pins (1, 3, 5, 7 and 9) to Teensy GND
//   Mic VDD to Teensy 3.3V

//   At startup of the ICS-52000, the start of the frame sync (WS_I) signal should be delayed from the start of the serial clock (SCK_I) by at
//   least 10 ms. This enables the microphone’s internal circuits to completely initialize before starting the synchronization sequence
//   with other microphones in the TDM array. This delay can be implemented either by enabling the WS output (FS) on the clock master at
//   least 10 ms after the SCK_I is enabled, or by externally controlling the signals given to the ICS-52000s.
//   
//
// This example code is in the public domain.

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
AudioConnection          patchCord1(tdm1, 0, queue1, 0);
AudioConnection          patchCord2(tdm1, 1, queue2, 0);
//AudioConnection          patchCord3(tdm1, 2, queue3, 0);
//AudioConnection          patchCord4(tdm1, 3, queue4, 0);
//AudioConnection          patchCord5(tdm1, 4, queue5, 0);
//AudioConnection          patchCord6(tdm1, 5, queue6, 0);
//AudioConnection          patchCord7(tdm1, 6, queue7, 0);
//AudioConnection          patchCord8(tdm1, 7, queue8, 0);
//AudioConnection          patchCord9(tdm1, 8, queue9, 0);
//AudioConnection          patchCord10(tdm1, 9, queue10, 0);
//AudioConnection          patchCord11(tdm1, 10, queue11, 0);
//AudioConnection          patchCord12(tdm1, 11, queue12, 0);
//AudioConnection          patchCord13(tdm1, 12, queue13, 0);
//AudioConnection          patchCord14(tdm1, 13, queue14, 0);
//AudioConnection          patchCord15(tdm1, 14, queue15, 0);
//AudioConnection          patchCord16(tdm1, 15, queue16, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=369,479
// GUItool: end automatically generated code

// Use these with the Teensy Audio Shield
//#define SDCARD_CS_PIN    10
//#define SDCARD_MOSI_PIN  7
//#define SDCARD_SCK_PIN   14

// Use these with the Teensy 3.5 & 3.6 SD card
#define SDCARD_CS_PIN    BUILTIN_SDCARD
#define SDCARD_MOSI_PIN  11  // not actually used
#define SDCARD_SCK_PIN   13  // not actually used

// Use these for the SD+Wiz820 or other adaptors
//#define SDCARD_CS_PIN    4
//#define SDCARD_MOSI_PIN  11
//#define SDCARD_SCK_PIN   13

// Remember which mode we're doing
int mode = 0;  // 0=stopped, 1=recording
int choice = 0;
int sample_number = 0;

unsigned int tsamplemillis = 11000;

String typeofsound;

// The file where data is recorded
File frec;

void setup() {
  
  Serial.begin(9600);

  // Audio connections require memory, and the record queue
  // uses this memory to buffer incoming audio.
  AudioMemory(512);

  // Initialize the SD card
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here if no SD card, but print a message
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }

  while(!Serial);

  Serial.println("Welcome to the beta version of Viband's TDM sound recorder, 11 second clips are going to be recorded, be \ncareful introducing a new type of sound already introduced since previous samples will be overwritten.");
  Serial.println("\nNote: SD Library uses short 8.3 names (12 characters). Filename (i.e baby/baby24) can't be longer.");
}

void loop() {

  if(choice==0){
  Serial.println("\nIntroduce new type of sound (i.e baby): ");
  while(!Serial.available());
  typeofsound = Serial.readString();
  sample_number = 0;
  record(typeofsound,sample_number);
  } else if(choice==1){
    sample_number++;
    record(typeofsound,sample_number);
  } else {
    //Do nothing
  }

  Serial.print("\nIntroduce 1 to take another sample or 0 to introduce a new type of sound: ");
  while(!Serial.available());
  choice = Serial.readString().toInt();
  Serial.println(choice);
    
}

void record(String type, int number){
  Serial.println("Recording " + type + String(number) + "...");
  elapsedMillis recordingTime = 0;
  String sname = type + number + ".RAW";
  String path = type + "/" + type + number + ".RAW";
  startRecording(sname, path, type);  
  while(recordingTime<tsamplemillis) continueRecording();
  stopRecording(); 
}

void startRecording(String sname, String path, String type) {
  
  int str_len = path.length()+1;
  char charpath[str_len];
  path.toCharArray(charpath,str_len);

  if (SD.exists(charpath)) {
    // The SD library writes new data to the end of the
    // file, so to start a new recording, the old file
    // must be deleted before new data is written.
    SD.remove(charpath);
  }

  str_len = type.length()+1;
  char chartype[str_len];
  type.toCharArray(chartype,str_len);

  if(!SD.exists(chartype)){
    SD.mkdir(chartype);
  }
  
  frec = SD.open(charpath, FILE_WRITE);
  if (frec) {
    Serial.println("File Open");
    queue1.begin();
    queue2.begin();
//    queue3.begin();
//    queue4.begin();
//    queue5.begin();
//    queue6.begin();
//    queue7.begin();
//    queue8.begin();
//    queue9.begin();
//    queue10.begin();
//    queue11.begin();
//    queue12.begin();
//    queue13.begin();
//    queue14.begin();
//    queue15.begin();
//    queue16.begin();
    mode = 1;
  }
}

void continueRecording() {
  if (queue1.available() >= 2 && queue2.available() >=2) {
    uint16_t buffer1[128];
    uint16_t buffer2[128];
    memcpy(buffer1, queue1.readBuffer(), 256);
    memcpy(buffer2, queue2.readBuffer(), 256);
    queue1.freeBuffer();
    queue2.freeBuffer();
    for(int i = 0; i < 128; i ++){
        frec.write(highByte(buffer2[i])); // LSB
        frec.write(lowByte(buffer1[i])); // Middle Byte
        frec.write(highByte(buffer1[i])); // MSB       
        //frec.write(lowByte(buffer2[i])); // Zeros
    }
  }
}

void stopRecording() {
  Serial.println("Finished recording.");
  queue1.end();
  queue2.end();
  queue1.clear();
  queue2.clear();
  frec.close();  
  mode = 0;
}

