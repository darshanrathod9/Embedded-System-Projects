#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define pin connections
const int mq6Pin = A0;                // MQ6 sensor connected to analog pin A0
const int flameSensorPin = A1;        // Flame sensor connected to analog pin A1
const int pumpPin = 8;                // Pump connected to digital pin 8
const int ledPin = 9;                 // LED connected to digital pin 9
const int buzzerPin = 10;             // Buzzer connected to digital pin 10

// Define threshold values for detection
const int gasThreshold = 600;  
const int flameThreshold = 200;  

LiquidCrystal_I2C lcd(0x27, 16, 2);  


void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set pin modes
  pinMode(mq6Pin, INPUT);
  pinMode(flameSensorPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize pump, LED, and buzzer as off
  digitalWrite(pumpPin, HIGH);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
}

void loop() {
  // Read sensor values
  int gasValue = analogRead(mq6Pin);
  int flameValue = analogRead(flameSensorPin);

  // Print sensor values to the serial monitor
  Serial.print("Gas Value: ");
  Serial.print(gasValue);
  Serial.print("  Flame Value: ");
  Serial.println(flameValue);

  // Check if gas level exceeds threshold
  if (gasValue > gasThreshold) {
    Serial.println("LPG Gas Detected!");
    digitalWrite(ledPin, HIGH);             // Turn on LED
    lcd.setCursor(0, 0);
    lcd.print("Gas: Detected    ");
    digitalWrite(buzzerPin, HIGH);          // Activate buzzer
  } else {
    digitalWrite(ledPin, LOW);              // Turn off LED
    lcd.setCursor(0, 0);
    lcd.print("Gas: Not Detected");
    digitalWrite(buzzerPin, LOW);           // Deactivate buzzer
  }

  // Check if flame is detected
  if (flameValue < flameThreshold) {
    Serial.println("Flame Detected!");
    lcd.setCursor(0, 1);
    lcd.print("Fire: Detected   ");
    digitalWrite(pumpPin, LOW);             // Activate pump
    digitalWrite(buzzerPin, HIGH);          // Activate buzzer
    delay(3000);                            // Keep pump on for 5 seconds (adjust as needed)
    digitalWrite(pumpPin, HIGH);
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Fire: Not Detected");
  }

  // Small delay before the next loop
  delay(1000);
}
