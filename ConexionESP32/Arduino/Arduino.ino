#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Ticker.h>
#include <SoftwareSerial.h>

// --- PINES ---
#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 6
#define LED_VERDE 3
#define LED_ROJO 4
#define BUZZER 5
#define LUZ_CASA 2
#define LDR_PIN A0

SoftwareSerial espSerial(0, 1);

MFRC522 rfid(SS_PIN, RST_PIN);
Servo puerta;

// --- VARIABLES ---
byte uidPermitido[] = {0xF3, 0xFE, 0xFD, 0x20};
int umbralLuz = 100;

int accesosPermitidos = 0;
int accesosDenegados = 0;

unsigned long ultimoEnvio = 0;
const unsigned long intervaloEnvio = 15000;

// --- PROTOTIPO ---
void cerrarPuerta();
void comunicarESP32();

Ticker temporizadorPuerta(cerrarPuerta, 2000, 1);

void setup() {
  Serial.begin(9600);    // Debug PC
  espSerial.begin(9600);   // Comunicación ESP32

  SPI.begin();
  rfid.PCD_Init();

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LUZ_CASA, OUTPUT);

  puerta.attach(SERVO_PIN);
  puerta.write(0);

  Serial.println("Sistema iniciado");
}

void loop() {
  temporizadorPuerta.update();

  // --- LDR ---
  int luzActual = analogRead(LDR_PIN);
  bool esDeNoche = (luzActual > umbralLuz);
  digitalWrite(LUZ_CASA, esDeNoche ? HIGH : LOW);

  // --- RFID ---
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    if (tarjetaAutorizada()) {
      accesoPermitido();
    } else {
      accesoDenegado();
    }
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }

  // --- ENVÍO A ESP32 ---
  if (millis() - ultimoEnvio >= intervaloEnvio) {
    comunicarESP32(luzActual, esDeNoche);
    ultimoEnvio = millis();
  }
}

// ---------------- FUNCIONES ----------------

void cerrarPuerta() {
  puerta.write(0);
  digitalWrite(LED_VERDE, LOW);
}

bool tarjetaAutorizada() {
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != uidPermitido[i]) return false;
  }
  return true;
}

void accesoPermitido() {
  accesosPermitidos++;
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_ROJO, LOW);
  puerta.write(180);
  temporizadorPuerta.start();
}

void accesoDenegado() {
  accesosDenegados++;
  digitalWrite(LED_ROJO, HIGH);
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, HIGH); delay(100);
    digitalWrite(BUZZER, LOW); delay(100);
  }
  digitalWrite(LED_ROJO, LOW);
}

void comunicarESP32(int ldr, bool noche) {
  espSerial.print(ldr);
  espSerial.print(",");
  espSerial.print(umbralLuz);
  espSerial.print(",");
  espSerial.print(noche ? 1 : 0);
  espSerial.print(",");
  espSerial.print(accesosPermitidos);
  espSerial.print(",");
  espSerial.println(accesosDenegados);

  Serial.println("Datos enviados al ESP32");
}
