/*
    Código de um exemplo da documentação Arduino da biblioteca SD.h, com alterações para funcionar no teensy 4.1
    Se tiver sucesso, espera-se que no monitor serial se consiga ler o que se escreveu no cartão SD
*/

#include <Arduino.h>

#define BOSCHx2_A A0
#define BOSCHx2_B A1

#define BOSCHx4_A A2
#define BOSCHx4_B A3
#define BOSCHx4_C A4
#define BOSCHx4_D A5

#define BOSCH_pressure_A A6
#define BOSCH_temp_A A7

#define BOSCH_pressure_B A8
#define BOSCH_temp_B A9

#define BOSCH_pressure_C A10
#define BOSCH_temp_C A11

#define BOSCH_pressure_D A12
#define BOSCH_temp_D A13

#define BOSCH_pressure_E A14
#define BOSCH_temp_E A15

#define BOSCH_pressure_F A16
#define BOSCH_temp_F A17






void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
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

    value = analogRead(BOSCH_pressure_A);
    Serial.print("BOSCH_pressure_A: ");
    Serial.println(value);

    value = analogRead(BOSCH_temp_A);
    Serial.print("BOSCH_temp_A: ");
    Serial.println(value);

    value = analogRead(BOSCH_pressure_B);
    Serial.print("BOSCH_pressure_B: ");
    Serial.println(value);

    value = analogRead(BOSCH_temp_B);
    Serial.print("BOSCH_temp_B: ");
    Serial.println(value);

    value = analogRead(BOSCH_pressure_C);
    Serial.print("BOSCH_pressure_C: ");
    Serial.println(value);

    value = analogRead(BOSCH_temp_C);
    Serial.print("BOSCH_temp_C: ");
    Serial.println(value);

    value = analogRead(BOSCH_pressure_D);
    Serial.print("BOSCH_pressure_D: ");
    Serial.println(value);
value = analogRead(BOSCH_temp_D);
    value = analogRead(BOSCH_temp_D);
    Serial.print("BOSCH_temp_D: ");
    Serial.println(value);

    value = analogRead(BOSCH_pressure_E);
    Serial.print("BOSCH_pressure_E: ");
    Serial.println(value);

    value = analogRead(BOSCH_temp_E);
    Serial.print("BOSCH_temp_E: ");
    Serial.println(value);

    value = analogRead(BOSCH_pressure_F);
    Serial.print("BOSCH_pressure_F: ");
    Serial.println(value);

    value = analogRead(BOSCH_temp_F);
    Serial.print("BOSCH_temp_F: ");
    Serial.println(value);
value = analogRead(BOSCH_temp_D);
    Serial.println("-------------------------------------");

    delay(1000);

    
}