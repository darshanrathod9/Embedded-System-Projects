#include <Servo.h>

Servo E_servo;
Servo W_servo;

#define E_RED       0
#define E_YELLOW    1
#define E_GREEN     2

#define W_RED       3
#define W_YELLOW    4
#define W_GREEN     5

#define N_RED       6
#define N_YELLOW    7
#define N_GREEN     8

#define S_RED       9
#define S_YELLOW    12
#define S_GREEN     13

#define E_SERVO     10
#define W_SERVO     11

void setup() {
  // put your setup code here, to run once:
  E_servo.attach(E_SERVO);
  W_servo.attach(W_SERVO);

  E_servo.write(0);
  W_servo.write(0);

  pinMode(E_RED, OUTPUT);
  pinMode(E_YELLOW, OUTPUT);
  pinMode(E_GREEN, OUTPUT);
  
  pinMode(W_RED, OUTPUT);
  pinMode(W_YELLOW, OUTPUT);
  pinMode(W_GREEN, OUTPUT);

  pinMode(N_RED, OUTPUT);
  pinMode(N_YELLOW, OUTPUT);
  pinMode(N_GREEN, OUTPUT);

  pinMode(S_RED, OUTPUT);
  pinMode(S_YELLOW, OUTPUT);
  pinMode(S_GREEN, OUTPUT);

  pinMode(E_SERVO, OUTPUT);
  pinMode(W_SERVO, OUTPUT);

  digitalWrite(E_RED, HIGH);
  digitalWrite(W_RED, HIGH);
  digitalWrite(N_RED, HIGH);
  digitalWrite(S_RED, HIGH);

  digitalWrite(E_YELLOW, LOW);  
  digitalWrite(W_YELLOW, LOW);
  digitalWrite(N_YELLOW, LOW);
  digitalWrite(S_YELLOW, LOW);

  digitalWrite(E_GREEN, LOW);
  digitalWrite(W_GREEN, LOW);
  digitalWrite(N_GREEN, LOW);
  digitalWrite(S_GREEN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(E_RED, LOW);
  digitalWrite(E_YELLOW, HIGH);
  delay(1000);
  digitalWrite(E_YELLOW, LOW);
  digitalWrite(E_GREEN, HIGH);
  E_servo.write(90);
  delay(4000);
  digitalWrite(E_GREEN, LOW);
  digitalWrite(E_RED, HIGH);  
  E_servo.write(0);  
  delay(500);

  digitalWrite(S_RED, LOW);
  digitalWrite(S_YELLOW, HIGH);
  delay(1000);
  digitalWrite(S_YELLOW, LOW);
  digitalWrite(S_GREEN, HIGH);
  delay(4000);
  digitalWrite(S_GREEN, LOW);
  digitalWrite(S_RED, HIGH);  
  delay(500);

  digitalWrite(W_RED, LOW);
  digitalWrite(W_YELLOW, HIGH);
  delay(1000);
  digitalWrite(W_YELLOW, LOW);
  digitalWrite(W_GREEN, HIGH);
  W_servo.write(90);
  delay(4000);
  digitalWrite(W_GREEN, LOW);
  digitalWrite(W_RED, HIGH);  
  W_servo.write(0);
  delay(500);

  digitalWrite(N_RED, LOW);
  digitalWrite(N_YELLOW, HIGH);
  delay(1000);
  digitalWrite(N_YELLOW, LOW);
  digitalWrite(N_GREEN, HIGH);
  delay(4000);
  digitalWrite(N_GREEN, LOW);
  digitalWrite(N_RED, HIGH);  
  delay(500);

}
