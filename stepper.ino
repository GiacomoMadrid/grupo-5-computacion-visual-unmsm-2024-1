#include <Stepper.h>

// Pines para el módulo ULN2003 (control de motor paso a paso)
#define IN1 2   // Pin 2 conectado a IN1 del módulo ULN2003
#define IN2 3   // Pin 3 conectado a IN2 del módulo ULN2003
#define IN3 4   // Pin 4 conectado a IN3 del módulo ULN2003
#define IN4 5   // Pin 5 conectado a IN4 del módulo ULN2003

// Pines para el módulo L298N (control de motor DC)
#define EN3 9   // Pin 9 conectado a EN3 del módulo L298N
#define EN4 10  // Pin 10 conectado a EN4 del módulo L298N
#define ENB 11  // Pin 11 conectado a ENB del módulo L298N

// Constantes
#define MAX_STEPS 2048  // Número máximo de pasos para el motor paso a paso

// Variables
int steps = 0;  // Contador de pasos para el motor paso a paso
int v1, v2, v3, v4;  // Variables para los valores de los pines de control del motor paso a paso

void setup() {
  // Configuración de los pines del motor paso a paso como salidas
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configuración de los pines del motor DC como salidas
  pinMode(EN3, OUTPUT);
  pinMode(EN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Inicialización del puerto serial para la comunicación
  Serial.begin(9600);

  // Configuración inicial del motor DC
  digitalWrite(ENB, HIGH);  // Habilitar el motor DC
  digitalWrite(EN3, HIGH);  // Establecer dirección del motor DC
  digitalWrite(EN4, LOW);   // Establecer dirección del motor DC
}

void loop() {
  // Reiniciar el contador de pasos si alcanza 4
  steps = (steps == 4) ? 0 : steps;

  // Establecer los valores de los pines de control del motor paso a paso
  v1 = (steps == 3) ? HIGH : LOW;
  v2 = (steps == 2) ? HIGH : LOW;
  v3 = (steps == 1) ? HIGH : LOW;
  v4 = (steps == 0) ? HIGH : LOW;

  // Aplicar los valores a los pines de control del motor paso a paso
  digitalWrite(IN1, v1);
  digitalWrite(IN2, v2);
  digitalWrite(IN3, v3);
  digitalWrite(IN4, v4);

  // Incrementar el contador de pasos
  steps += 1;

  // Esperar 2 milisegundos antes de la siguiente iteración para controlar la velocidad del motor paso a paso
  delay(2);
}
