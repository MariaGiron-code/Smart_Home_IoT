// Librería SPI: permite la comunicación SPI entre Arduino y el RFID
#include <SPI.h>
// Librería del módulo RFID RC522
#include <MFRC522.h>
#include <Servo.h>

                                                       // DEFINICIÓN DE LOS PINES 
//Pines para el sensor RFID
#define SS_PIN 10  // Pin SDA (SS) del RFID conectado al pin 10 del Arduino
#define RST_PIN 9   // Pin RST (Reset) del RFID conectado al pin 9 del Arduino

//Pines para actuadores 
#define SERVO_PIN 6 
#define LED_VERDE 3 
#define LED_ROJO 4 
#define BUZZER 5 // Buzzer de alarma
#define LUZ_CASA 2 

// Pines para sensores
#define PIR_PIN 7  //sensor de movimeinto
#define LDR_PIN A0 //sensor de luz (analógico)


// Objeto RFID indicando los pines a usar (SS y RST)
MFRC522 rfid(SS_PIN, RST_PIN);

// Objeto Servo 
Servo puerta;

// Definimos una varible para guardar el UID de la tarjeta de acceso del sensor.
// Este será el uni UID permitido para acceder a la casa
byte uidPermitido[] = {0xA3, 0x4F, 0x2C, 0x91};


                                                                  //CONFIGURACIÓN INICIAL
void setup() {

  // Iniciar la comunicación serial para mostrar los datos 
  Serial.begin(9600);

  // Inicia la comunicación SPI
  SPI.begin();

  // Inicializa el lector RFID RC522
  rfid.PCD_Init();

  // Configuración de los pines
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LUZ_CASA, OUTPUT);

  // Inicializar el servo
  puerta.attach(SERVO_PIN);
  puerta.write(0); // 0 cerrado - 1 abierto

  Serial.println("Sistema de control y seguridad iniciado ! ....");
}


void loop() {
                                                                   // LECTURA DEL SENSOR RFID
  // Verifica si hay una tarjeta cerca del lector y lee los datos de la tarjeta
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    
    if (tarjetaAutorizada()){
      accesoPermitido();
    
    }else{
      accesoDenegado(); 
    }
  } 

                                                                   // LECTURA DEL SENSOR PIR (movimiento)
  int movimiento = digitalRead(PIR_PIN); //señal de 0 a 1023
  
  if (movimiento == HIGH) {
    Serial.println("Movimiento detectado. !!! ALARMA ENCENDIDA !!!");
  }

                                                                   // LECTURA SENSOR LDR (luz)
  int luz = analogRead(LDR_PIN);
  // Umbral de luz (este valor se ajusta de acuerdo a las necesidades)
  int umbral = 500;

  if (luz < umbral) {
    // Ambiente oscuro (noche)
    Serial.println("Ya es de noche, las luces exteriores se encederan dentro de poco");

    // Encender luces
    digitalWrite(LUZ_CASA, HIGH);
  } else {
    // Ambiente iluminado (día)
    Serial.println("Es de día, apagando las luces exteriores");
    digitalWrite(LUZ_CASA, LOW);
  }
}


                                                                   // FUNCIONES DE ACCESO 
// Verifica si la tarjeta es la autorizada
bool tarjetaAutorizada() {
  // Recorremos cada byte del UID de la tarjeta y verificamos que sea el mismo para dar acceso
  for (byte i = 0; i < 4; i++) { 
    
    if (rfid.uid.uidByte[i] != uidPermitido[i]) {
      return false; // negamos acceso
    }
  }
  return true;  // acceso correcto
}

// Accion cuando el acceso es permitido
void accesoPermitido() {
  Serial.println("Acceso permitido, BIENVENIDO A CASA !!");

  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_ROJO, LOW);

  puerta.write(90); // Abre la puerta
  delay(3000);
  puerta.write(0);  // Cierra la puerta

  digitalWrite(LED_VERDE, LOW);
}

// Acciones cuando el acceso es denegado
void accesoDenegado() {
  Serial.println("Acceso denegado. USE LA CREDENCIAL DE ACCESO CORRECTA !!");

  digitalWrite(LED_ROJO, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(1000);
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED_ROJO, LOW);
}

// “El código inicializa la comunicación SPI, configura el módulo RFID RC522 y permite leer el UID de la tarjeta,
// si las consiciones se cumplen permite el acceso caso contrario lo niega. 
