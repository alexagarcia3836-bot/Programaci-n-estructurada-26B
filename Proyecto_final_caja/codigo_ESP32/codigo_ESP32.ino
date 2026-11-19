#include <ESP32Servo.h>

// Definición de pines según el circuito
const int trigPin = 26;
const int echoPin = 25;
const int servoPin = 27;

Servo miServo;

// Variables para medir distancia
long duracion;
int distancia;

// Configuración de ángulos y distancia de activación
const int distanciaActivacion = 15; // Abre la tapa si detecta algo a menos de 15 cm
const int anguloCerrado = 0;        // Posición de tapa cerrada
const int anguloAbierto = 150;      // Posición de tapa abierta

void setup() {
  Serial.begin(115200);
  
  // Configuración de pines del sensor ultrasónico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Inicialización del servomotor con la tapa cerrada
  miServo.attach(servoPin);
  miServo.write(anguloCerrado);
  delay(500);
}

void loop() {
  // Enviar pulso ultrasónico
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir tiempo del eco con límite de 30,000 us para evitar lecturas en 0
  duracion = pulseIn(echoPin, HIGH, 30000);
  distancia = duracion * 0.034 / 2;

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // Abre la tapa si detecta entre 3 cm y 15 cm
  if (distancia > 2 && distancia <= distanciaActivacion) {
    miServo.write(anguloAbierto); // Abre la tapa
    delay(3000);                  // Mantiene la tapa abierta 3 segundos
  } else {
    miServo.write(anguloCerrado); // Cierra la tapa
  }

  delay(200); // Pausa entre mediciones
}