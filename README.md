# 🏠 Smart_Home_IoT

Sistema IoT de Casa Inteligente con Seguridad y Control de Accesos. Este proyecto implementa un sistema completo de automatización doméstica que integra control de acceso mediante RFID, detección de luz ambiental, y conectividad a la nube para monitoreo remoto.

---

## ✨ Características Principales

El sistema ofrece un conjunto completo de funcionalidades diseñadas para proporcionar seguridad y comodidad en un hogar inteligente. La autenticación de usuarios se realiza mediante tecnología RFID, permitiendo que únicamente personas autorizadas puedan acceder a través de la puerta controlada por servo. El sensor de luz LDR monitorea constantemente el nivel de iluminación ambiental para activar automáticamente la iluminación interior cuando oscurece, proporcionando conveniencia y ahorro energético.

El sistema incluye indicadores visuales mediante LEDs de colores que proporcionan retroalimentación inmediata sobre el estado del acceso, mientras que el buzzer genera alertas sonoras en caso de intentos de acceso no autorizados. La integración con ESP32 permite la transmisión de datos a la plataforma ThingSpeak, donde se pueden visualizar métricas como niveles de luz, conteo de accesos permitidos y denegados, y el estado del sistema en tiempo real.

---

## 🗂️ Estructura del Proyecto

El repositorio está organizado en carpetas que agrupan los diferentes componentes y variantes del sistema. Cada directorio contiene el código fuente correspondiente a una configuración específica del hardware, facilitando la comprensión y modificación del proyecto según las necesidades del usuario.

```
Smart_Home/
├── README.md                          # Documentación principal del proyecto
│
├── home_code/
│   └── funcion.ino                    # Código principal del sistema (Arduino)
│                                      Incluye: RFID, servo, LDR, LEDs, buzzer
│
├── sketch_lectura_UID/
│   └── sketch_lectura_UID.ino         # Utilidad para leer UID de tarjetas RFID
│                                      Útil para registrar nuevas tarjetas
│
└── ConexionESP32/
    ├── Arduino/
    │   └── Arduino.ino                # Código Arduino con comunicación ESP32
    │                                      Envía datos al ESP32 cada 15 segundos
    │
    └── ESP32/
        └── ESP32.ino                  # Código ESP32 con WiFi y ThingSpeak
                                         Envía datos a la nube para monitoreo
```

---

## 🔧 Componentes de Hardware

El sistema está diseñado para funcionar con componentes electrónicos accesibles y ampliamente utilizados en proyectos de electrónica maker. El corazón del sistema de control de acceso es el módulo lector RFID MFRC522, compatible con tarjetas y llaveros comunes en el mercado. Este módulo se comunica con el microcontrolador mediante el protocolo SPI, permitiendo la lectura segura de identificadores únicos.

Un servo motor estándar (como el SG90) controla el mecanismo de apertura y cierre de la puerta, girando 180 grados para abrir y retornando a la posición cero para cerrar automáticamente después de dos segundos. El sensor LDR (Light Dependent Resistor) monitorea los niveles de luz ambiental, activando las luces interiores cuando el valor supera el umbral configurado. Los indicadores LED (verde y rojo) proporcionan retroalimentación visual instantánea sobre el estado de autorización, mientras que el buzzer genera tonos de alerta en caso de accesos denegados.

---

## 📡 Conectividad IoT

La integración con la nube se logra mediante el módulo ESP32, que proporciona capacidades WiFi integradas para la transmisión de datos. El sistema envía información periódica a ThingSpeak, una plataforma IoT que permite visualizar y analizar datos en tiempo real a través de gráficos y paneles personalizados.

Los campos transmitidos incluyen el valor actual del sensor LDR, el umbral configurado para detección de noche, un indicador booleano de si es de noche, el conteo acumulado de accesos permitidos, y el conteo de intentos de acceso denegados. Esta funcionalidad permite el monitoreo remoto del sistema, generando estadísticas útiles para análisis de seguridad y patrones de uso.

---

## 🚀 Cómo usar?

Para poner en funcionamiento el sistema, primero debe cargar el código correspondiente al microcontrolador que desea utilizar. Si únicamente desea probar el control de acceso local, el archivo `home_code/funcion.ino` es la opción más sencilla, ya que incluye todas las funcionalidades básicas sin necesidad de conectividad a internet.

Si desea implementar el sistema completo con monitoreo en la nube, debe cargar el código `ConexionESP32/Arduino/Arduino.ino` en un Arduino y el código `ConexionESP32/ESP32/ESP32.ino` en un ESP32. Asegúrese de configurar las credenciales de WiFi y el ID del canal de ThingSpeak según sus preferencias. El Arduino envía datos al ESP32 mediante comunicación serial, y el ESP32 los retransmite a la nube cada 15 segundos.

Para registrar nuevas tarjetas RFID en el sistema, utilice el utilitario `sketch_lectura_UID/sketch_lectura_UID.ino`, que imprimirá en el monitor serial el UID de cualquier tarjeta que acerque al lector. Este UID debe agregarse al array `uidPermitido` en el código principal para autorizar el acceso.

---

## 📊 Monitoreo en ThingSpeak

Una vez configurada la conexión a internet, puede visualizar los datos de su sistema en ThingSpeak accediendo al canal configurado. Los gráficos muestran la evolución temporal de los niveles de luz, permitiendo analizar patrones de iluminación a lo largo del día. El conteo de accesos proporciona información sobre el uso del sistema, mientras que los intentos denegados pueden indicar intentos de acceso no autorizados que merecen atención.

