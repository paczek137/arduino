/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

File myFile;
char bufor[256];

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(10, OUTPUT);

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("/data/data2.txt", FILE_WRITE);

  int n = 0;
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to /data/data2.txt...");
    
    do {
      ltoa(++n, bufor,10);
      strcat(bufor, " kkkkk\r\n");
      myFile.print(bufor);
      //_delay_us(100);
      //Serial.print(bufor);
    } while (n < 10);
    myFile.println("testing 1, 2, 3.");
    
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening /data/data2.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("/data/data2.txt");
  if (myFile) {
    Serial.println("/data/data2.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening /data/data2.txt");
  }
}

void loop()
{
  // nothing happens after setup
}


