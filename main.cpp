/*
    Código de um exemplo da documentação Arduino da biblioteca SD.h, com alterações para funcionar no teensy 4.1
    Se tiver sucesso, espera-se que no monitor serial se consiga ler o que se escreveu no cartão SD
*/

#include <Arduino.h>

#include <SPI.h>
#include <SD.h>

File data_file;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  data_file = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (data_file) {
    Serial.print("Writing to test.txt...");
    data_file.println("testing 1, 2, 3.");
    // close the file:
    data_file.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  data_file = SD.open("test.txt");
  if (data_file) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (data_file.available()) {
      Serial.write(data_file.read());
    }
    // close the file:
    data_file.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // nothing happens after setup
}