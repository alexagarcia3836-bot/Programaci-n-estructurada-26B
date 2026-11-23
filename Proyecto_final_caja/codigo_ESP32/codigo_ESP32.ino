#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>


// 1. CONFIGURACIÓN DE RED WI-FI (PUNTO DE ACCESO LOCAL)

const char* SSID_AP = "Bote_Inteligente_ESP32";  // Nombre de la red Wi-Fi generada por el ESP32
const char* PASS_AP = "12345678";                // Contraseña de acceso (mínimo 8 caracteres)

WebServer server(80);  // Instancia del servidor web en el puerto HTTP estándar (80)
Servo actuadorTapa;    // Objeto para el control PWM del servomotor


// 2. DEFINICIÓN DE PINES Y PARÁMETROS DEL SISTEMA

const int PIN_TRIG = 26;   // Pin GPIO de disparo para el sensor ultrasónico HC-SR04
const int PIN_ECHO = 25;   // Pin GPIO de recepción de eco (protegido con divisor a 3.3V)
const int PIN_SERVO = 27;  // Pin GPIO de señal PWM para el servomotor SG90

const int DISTANCIA_UMBRAL_CM = 15;             // Distancia máxima de activación en centímetros
const int ANGULO_CERRADO = 0;                   // Posición angular de reposo (tapa cerrada)
const int ANGULO_ABIERTO = 90;                  // Posición angular de apertura (tapa levantada)
const unsigned long TIEMPO_APERTURA_MS = 3000;  // Tiempo que permanece abierta la tapa (3 segundos)

// 3. VARIABLES GLOBALES DE TELEMETRÍA

float ultimaDistancia = 0.0;  // Almacena la última lectura válida del sensor en cm
int contadorAperturas = 0;    // Registro acumulado de ciclos de apertura realizados


// 4. FUNCIONES DE ADQUISICIÓN Y CONTROL DE ACTUADORES


/**
 * @brief Mide la distancia frontal utilizando el sensor ultrasónico HC-SR04.
 * @return float Distancia en centímetros, o -1.0 si la lectura está fuera de rango/error.
 */
float obtenerDistanciaCm() {
  // Generación del pulso de disparo limpio de 10 microsegundos
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Lectura de duración del pulso alto en ECHO (timeout de 30 ms para evitar bloqueos)
  long duracion = pulseIn(PIN_ECHO, HIGH, 30000);

  // Manejo de errores: si no hay rebote o excede el rango físico del sensor
  if (duracion <= 0) {
    return -1.0;
  }

  // Cálculo de distancia: (tiempo * velocidad del sonido 0.0343 cm/us) / 2 (ida y vuelta)
  float distancia = (duracion * 0.0343) / 2.0;
  return (distancia > 400.0) ? -1.0 : distancia;
}

/**
 * @brief Ejecuta el ciclo completo de apertura, espera temporizada y cierre de la tapa.
 */
void abrirTapa() {
  actuadorTapa.write(ANGULO_ABIERTO);  // Mueve el servo a 90° (abierto)
  contadorAperturas++;                 // Incrementa el registro de uso
  delay(TIEMPO_APERTURA_MS);           // Mantiene la tapa abierta durante 3 segundos
  actuadorTapa.write(ANGULO_CERRADO);  // Regresa el servo a 0° (cerrado)
}


// 5. RUTAS Y MANEJADORES DEL SERVIDOR WEB (INTERFAZ GRÁFICA)

/**
 * @brief Construye y envía la interfaz web HTML al navegador del cliente conectado.
 */
void manejarPaginaPrincipal() {
  String html = "<!DOCTYPE html><html lang='es'><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<meta http-equiv='refresh' content='2'>";  // Actualización automática cada 2 segundos
  html += "<title>Panel Bote Inteligente</title>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;text-align:center;margin-top:40px;background:#f0f2f5;color:#333;}";
  html += ".card{background:#fff;padding:25px;border-radius:12px;max-width:340px;margin:auto;box-shadow:0 4px 12px rgba(0,0,0,0.1);}";
  html += "h2{color:#1a73e8;margin-bottom:20px;}";
  html += ".dato{font-size:18px;margin:12px 0;}";
  html += "button{background:#1a73e8;color:white;border:none;padding:14px 24px;font-size:16px;border-radius:8px;cursor:pointer;margin-top:15px;width:100%;}";
  html += "button:hover{background:#1557b0;}";
  html += "</style></head><body>";

  html += "<div class='card'>";
  html += "<h2>Bote Inteligente IoT</h2>";
  html += "<div class='dato'><b>Distancia actual:</b> " + String(ultimaDistancia, 1) + " cm</div>";
  html += "<div class='dato'><b>Aperturas totales:</b> " + String(contadorAperturas) + "</div>";
  html += "<a href='/abrir'><button>Abrir Tapa Manualmente</button></a>";
  html += "</div></body></html>";

  server.send(200, "text/html", html);
}

/**
 * @brief Endpoint HTTP (/abrir) que acciona la tapa de forma remota y redirige al inicio.
 */
void manejarAperturaManual() {
  abrirTapa();
  server.sendHeader("Location", "/");  // Redirección automática a la página principal
  server.send(303);
}

// ============================================================================
// 6. CONFIGURACIÓN INICIAL (SETUP) Y BUCLE PRINCIPAL (LOOP)
// ============================================================================

void setup() {
  Serial.begin(115200);

  // Configuración de modos de pines digitales
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Inicialización del servomotor SG90 (frecuencia 50 Hz, pulsos 500 us a 2400 us)
  actuadorTapa.setPeriodHertz(50);
  actuadorTapa.attach(PIN_SERVO, 500, 2400);
  actuadorTapa.write(ANGULO_CERRADO);  // Inicia en posición cerrada (0°)
  delay(500);

  // Configuración e inicio del Punto de Acceso Wi-Fi (SoftAP)
  WiFi.softAP(SSID_AP, PASS_AP);
  Serial.println("Servidor Wi-Fi iniciado correctamente.");
  Serial.print("Conéctate a la red 'Bote_Inteligente_ESP32' y abre: http://");
  Serial.println(WiFi.softAPIP());  // Dirección IP por defecto: 192.168.4.1

  // Registro de rutas HTTP y arranque del servidor
  server.on("/", manejarPaginaPrincipal);
  server.on("/abrir", manejarAperturaManual);
  server.begin();
}

void loop() {
  // Atiende peticiones entrantes de clientes conectados a la interfaz web
  server.handleClient();

  // Adquisición de distancia desde el sensor ultrasónico
  float dist = obtenerDistanciaCm();

  if (dist < 0) {
    // Manejo de error ante lecturas fuera de rango o rebote perdido
    Serial.println("[ADVERTENCIA] Lectura fuera de rango.");
  } else {
    ultimaDistancia = dist;
    Serial.print("Distancia: ");
    Serial.print(ultimaDistancia, 1);
    Serial.println(" cm");

    // Evaluación del umbral de proximidad (descarta ruido < 2 cm)
    if (ultimaDistancia > 2.0 && ultimaDistancia <= DISTANCIA_UMBRAL_CM) {
      abrirTapa();
    }
  }

  delay(100);  // Pausa breve para estabilidad de muestreo y atención Wi-Fi
}