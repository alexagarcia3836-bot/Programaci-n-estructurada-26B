# Bote de Basura Inteligente IoT (Apertura sin Contacto + Interfaz Web Wi-Fi)

**Proyecto Integrador:** Circuitos Electrónicos y Programación Estructurada (2026-B)[cite: 5]  
**Institución:** Universidad de Guadalajara · Centro Universitario de Tlajomulco · Licenciatura en Ingeniería Biomédica  
**Docente:** José Alberto Aguirre León

## Integrantes del Equipo
# **[America Citlalli Tamayo Avina]** – [] #
# **[Brisa Alexa Garcia Villalba]** – [alexagarcia3836-bot] #


---

## 1. Descripción del Problema y Alcance

### Planteamiento del problema
En entornos clínicos, de laboratorio y domésticos, el contacto físico con las tapas de los contenedores de desechos incrementa el riesgo de contaminación cruzada. Se requiere un mecanismo autónomo, higiénico y monitoreable de forma remota sin depender de infraestructura de red externa.

### Objetivo General y Alcance Medible
Desarrollar un prototipo funcional de contenedor de residuos inteligente alimentado de forma autónoma mediante Power Bank 5 V, integrando control local por proximidad y monitoreo web mediante Wi-Fi:
# **Detección sin contacto:** Activación automática al detectar un objeto a una distancia de 15 cm (filtrando ruido 2 cm y lecturas fuera de rango > 400 cm).
# **Respuesta mecánica:** Apertura en < 0.5 s, retención abierta durante 3.0 s y cierre automático calibrado sin forzar los topes del servomotor.
# **Funcionalidad Extra (Interfaz Web Local SoftAP):** Generación de punto de acceso Wi-Fi propio (`Bote_Inteligente_ESP32`) con servidor HTTP en la dirección `192.168.4.1` para visualizar en tiempo real la distancia medida, el contador acumulado de aperturas y ejecutar apertura manual remota. #

---

## 2. Lista de Materiales (BOM) y Costos
# 1 ESP32  38 Pines ESP WROOM 32 Módulo Wifi + Bluetooth Micro USB (Tipo B) ($150.00 - $220.00)
# Sensor Ultrasónico HC-SR04 ($50.00)
# Servomotor Micro SG90 ($45.00 - $70.00)
# Protoboard (400 puntos) ($45.00)
# 3 resistecnias de 1KO ($15.00)
# Cables ($34.00 - $40.00)
# Power bank ($400.00)
# Estructura ($40.00) 

## 3. Instrucciones de Armado y Ejecución

### A. Armado del Circuito y Calibración Mecánica
1. **Protección de niveles lógicos:** Conectar el pin `ECHO` del HC-SR04 a una resistencia de 1kO en serie con una de 2 en serie de 1kO a `GND`, enviando el nodo intermedio ($3.33\text{ V}$) al `GPIO 25` del ESP32.
2. **Alimentación y filtrado:** Conectar `VCC` del HC-SR04 y el cable rojo del SG90 a `VIN` 5 V, colocando el capacitor electrolítico entre `VIN` y `GND` respetando polaridad.
3. **Ajuste de palanca:** Energizar el sistema sin la palanca blanca montada en el eje del servo para que se posicione en `ANGULO_CERRADO`. Con la tapa del bote cerrada manualmente, encajar la palanca en el eje sin forzar el tope mecánico y asegurar con su tornillo.

### B. Compilación y Carga del Firmware (Arduino IDE / C++)
1. Instalar el paquete de placas **ESP32 by Espressif Systems** en Arduino IDE.
2. Instalar la librería **`ESP32Servo`** (las librerías `WiFi.h` y `WebServer.h` vienen incluidas en el núcleo del ESP32).
3. Abrir el archivo [`src/bote_inteligente_web/bote_inteligente_web.ino`](src/bote_inteligente_web/bote_inteligente_web.ino).
4. Seleccionar **ESP32 Dev Module**, velocidad de carga **115200** y el puerto **COM** activo.
5. Desconectar temporalmente el cable rojo (`VIN`) del servomotor durante la subida para evitar caídas de tensión, cargar el código y reconectar el servo.

### C. Uso de la Interfaz Web Local (Funcionalidad Extra)
1. Energizar el ESP32 con la Power Bank.
2. Desde un teléfono móvil o computadora, conectarse a la red Wi-Fi **`Bote_Inteligente_ESP32`** (contraseña: **`12345678`**).
3. Abrir un navegador web e ingresar a **`http://192.168.4.1`**.
4. Visualizar la telemetría en vivo (distancia en cm y contador de aperturas) o presionar **"Abrir Tapa Manualmente"** para accionar el mecanismo de forma remota.