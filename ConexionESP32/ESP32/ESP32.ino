#include <WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "CAMPUS_EPN";
const char* password = "politecnica**";

unsigned long channelID = 3242590;
const char* apiKey = "J4WKYXKTEX9X5LD0";

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RX, TX);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  ThingSpeak.begin(client);
}

void loop() {
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    Serial.println("Recibido: " + data);

    int c1 = data.indexOf(',');
    int c2 = data.indexOf(',', c1 + 1);
    int c3 = data.indexOf(',', c2 + 1);
    int c4 = data.indexOf(',', c3 + 1);

    int ldr = data.substring(0, c1).toInt();
    int umbral = data.substring(c1 + 1, c2).toInt();
    int noche = data.substring(c2 + 1, c3).toInt();
    int permitidos = data.substring(c3 + 1, c4).toInt();
    int denegados = data.substring(c4 + 1).toInt();

    ThingSpeak.setField(1, ldr);
    ThingSpeak.setField(2, umbral);
    ThingSpeak.setField(3, noche);
    ThingSpeak.setField(4, permitidos);
    ThingSpeak.setField(5, denegados);

    int x = ThingSpeak.writeFields(channelID, apiKey);

    if (x == 200) {
      Serial.println("Datos enviados a ThingSpeak");
    } else {
      Serial.println("Error ThingSpeak: " + String(x));
    }
  }
}
