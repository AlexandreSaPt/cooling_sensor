/*
    Código de um exemplo da documentação Arduino da biblioteca SD.h, com alterações para funcionar no teensy 4.1
    Se tiver sucesso, espera-se que no monitor serial se consiga ler o que se escreveu no cartão SD
*/

#include <Arduino.h>

#define BOSCHx2_A A14
#define BOSCHx2_B A15

#define BOSCHx4_A A16
#define BOSCHx4_B A17
#define BOSCHx4_C A13
#define BOSCHx4_D A12

#define BOSCH_pressure_A A10
#define BOSCH_temp_A A11

#define BOSCH_pressure_B A1
#define BOSCH_temp_B A0

#define BOSCH_pressure_C A3
#define BOSCH_temp_C A2

#define BOSCH_pressure_D A5
#define BOSCH_temp_D A4

#define BOSCH_pressure_E A7
#define BOSCH_temp_E A6

#define BOSCH_pressure_F A9
#define BOSCH_temp_F A8






void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Serial monitor Working");
  analogReadResolution(12);
}

void loop() {

    int value;

    value = analogRead(BOSCHx2_A);
    Serial.print("BOSCHx2_A: ");
    Serial.println(value);

    value = analogRead(BOSCHx2_B);
    Serial.print("BOSCHx2_B: ");
    Serial.println(value);

    Serial.println();
    delay(100);

    value = analogRead(BOSCHx4_A);
    Serial.print("BOSCHx4_A: ");
    Serial.println(value);

    value = analogRead(BOSCHx4_B);
    Serial.print("BOSCHx4_B: ");
    Serial.println(value);

    value = analogRead(BOSCHx4_C);
    Serial.print("BOSCHx4_C: ");
    Serial.println(value);

    value = analogRead(BOSCHx4_D);
    Serial.print("BOSCHx4_D: ");
    Serial.println(value);

    Serial.println();
    delay(100);

    value = analogRead(BOSCH_pressure_A);
    Serial.print("BOSCH_pressure_A: ");
    Serial.println(value);

    value = analogRead(BOSCH_temp_A);
    Serial.print("BOSCH_temp_A: ");
    Serial.println(value);

    Serial.println();
    delay(100);

    value = analogRead(BOSCH_pressure_B);
    Serial.print("BOSCH_pressure_B: ");
    Serial.println(value);

    value = analogRead(BOSCH_temp_B);
    Serial.print("BOSCH_temp_B: ");
    Serial.println(value);

    Serial.println();
    delay(100);

    value = analogRead(BOSCH_pressure_C);
    Serial.print("BOSCH_pressure_C: ");
    Serial.println(value);

    value = analogRead(BOSCH_temp_C);
    Serial.print("BOSCH_temp_C: ");
    Serial.println(value);

    Serial.println();
    delay(100);

    value = analogRead(BOSCH_pressure_D);
    Serial.print("BOSCH_pressure_D: ");
    Serial.println(value);

    value = analogRead(BOSCH_temp_D);
    Serial.print("BOSCH_temp_D: ");
    Serial.println(value);

    Serial.println();
    delay(100);

    value = analogRead(BOSCH_pressure_E);
    Serial.print("BOSCH_pressure_E: ");
    Serial.println(value);

    value = analogRead(BOSCH_temp_E);
    Serial.print("BOSCH_temp_E: ");
    Serial.println(value);

    Serial.println();
    delay(100);

    value = analogRead(BOSCH_pressure_F);
    Serial.print("BOSCH_pressure_F: ");
    Serial.println(value);

    value = analogRead(BOSCH_temp_F);
    Serial.print("BOSCH_temp_F: ");
    Serial.println(value);

    Serial.println("-------------------------------------");

    delay(1000);

    
}