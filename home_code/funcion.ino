#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Ticker.h>

// --- DEFINICIÓN DE PINES ---
#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 6
#define LED_VERDE 3
#define LED_ROJO 4
#define BUZZER 5
#define LUZ_CASA 2
#define LDR_PIN A0

// --- OBJETOS ---
MFRC522 rfid(SS_PIN, RST_PIN);
Servo puerta;

// --- VARIABLES ---
byte uidPermitido[] = { 0xF3, 0xFE, 0xFD, 0x20 };
int umbralLuz = 100;       
int estadoPIRAnterior = LOW;

// --- ALARMA ---
unsigned long tiempoAlarmaInicio = 0;
const unsigned long DURACION_ALARMA = 1000;
bool alarmaActiva = false;

// --- PROTOTIPO ---
void cerrarPuerta();

// --- TICKER ---
Ticker temporizadorPuerta(cerrarPuerta, 2000, 1);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LUZ_CASA, OUTPUT);
  pinMode(LDR_PIN, INPUT);

  puerta.attach(SERVO_PIN);
  puerta.write(0);

  Serial.println("Sistema de seguridad y control iniciado.....");
  Serial.println("----------------------------------");
}

void loop() {

  
  temporizadorPuerta.update();

  // ---------- LECTURA LDR ----------
  int luzActual = analogRead(LDR_PIN);

  // IMPORTANTE!
  // valor ALTO = oscuro
  Serial.print("LDR: ");
  Serial.print(luzActual);
  delay(1000);
  Serial.print(" | Umbral: ");
  Serial.print(umbralLuz);
  Serial.print(" | Estado: ");
  delay(1000);
  

  bool esDeNoche = (luzActual > umbralLuz);
  
  if (esDeNoche) {
    Serial.println("NOCHE");
    digitalWrite(LUZ_CASA, HIGH);
    
  } else {
    Serial.println("DIA");
    digitalWrite(LUZ_CASA, LOW);
    
  }


  // ---------- RFID ----------
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    if (tarjetaAutorizada()) {
      accesoPermitido();
    } else {
      accesoDenegado();
    }
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }

}

// ---------------- FUNCIONES ----------------

void cerrarPuerta() {
  Serial.println(">> Cerrando puerta automáticamente");
  puerta.write(0);
  digitalWrite(LED_VERDE, LOW);
}

bool tarjetaAutorizada() {
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != uidPermitido[i]) {
      return false;
    }
  }
  return true;
}

void accesoPermitido() {
  Serial.println(">> Acceso permitido");

  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_ROJO, LOW);
  puerta.write(180);

  temporizadorPuerta.start();
}

void accesoDenegado() {
  Serial.println(">> Acceso DENEGADO");

  digitalWrite(LED_ROJO, HIGH);
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, HIGH); delay(100);
    digitalWrite(BUZZER, LOW); delay(100);
  }
  digitalWrite(LED_ROJO, LOW);
}

