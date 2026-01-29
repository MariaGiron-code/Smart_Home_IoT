#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);      // Inicia comunicación serial
  SPI.begin();             // Inicia comunicación SPI
  rfid.PCD_Init();         // Inicia el lector RFID
  Serial.println("Acerca la tarjeta o llavero...");
}

void loop() {

  // Verifica si hay una tarjeta cerca
  if (!rfid.PICC_IsNewCardPresent()) return;

  // Lee la tarjeta
  if (!rfid.PICC_ReadCardSerial()) return;

  Serial.print("UID: ");

  // Recorre los bytes del UID
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }

  Serial.println();
  delay(1000);
}
