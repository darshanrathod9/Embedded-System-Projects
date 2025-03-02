#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const int sensorPin = A0;      // Pin where the IR sensor is connected
const int threshold = 500;     // Threshold value for object detection
const int buzzerPin = 8;       // Pin where the buzzer is connected
Servo myServo;                 // Create a Servo object
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16x2 display

unsigned long previousMillis = 0; // Variable to store the previous time
const unsigned long interval = 60000; // Interval for 1 minute (in milliseconds)
bool countdownCompleted = false;  // Flag to indicate if countdown has completed
bool objectDetected = false;      // Flag to indicate if an object is detected
int secondsRemaining = 60;        // Initial number of seconds remaining

void setup() {
  Serial.begin(9600);             // Start the serial communication
  myServo.attach(9);              // Attach the Servo signal pin to digital pin 9
  myServo.write(0);               // Initialize the servo to 0 degrees
  lcd.init();                     // Initialize the LCD
  lcd.backlight();                // Turn on the backlight
  pinMode(buzzerPin, OUTPUT);     // Set the buzzer pin as an output
  digitalWrite(buzzerPin, LOW);   // Initialize the buzzer to be off
  lcd.setCursor(4, 0);
  lcd.print("Welcome to");
  delay(1000);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Medicine");
  lcd.setCursor(4, 1);
  lcd.print("Reminder");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);            // Set the cursor to the first row
  lcd.print("Countdown: ");       // Print the text
  lcd.setCursor(7, 1);            // Set the cursor to the second row
  lcd.print(secondsRemaining);    // Print initial time
}

void loop() {
  unsigned long currentMillis = millis(); // Get the current time
  
  // Countdown timer
  if (!countdownCompleted) {
    if (currentMillis - previousMillis >= 1000) { // Update every second
      previousMillis = currentMillis;
      if (secondsRemaining > 0) {
        secondsRemaining--; // Decrement the remaining seconds
        lcd.setCursor(7, 1); // Set the cursor to the second row
        lcd.print("    "); // Clear previous seconds
        lcd.setCursor(7, 1); // Set the cursor to the second row
        lcd.print(secondsRemaining); // Print remaining seconds
      } else {
        countdownCompleted = true; // Set the countdown completed flag
        digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
      }
    }
  }

  // Object detection
  int sensorValue = analogRead(sensorPin); // Read the value from the sensor
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);             // Print the sensor value to the serial monitor
  
  if (sensorValue < threshold) {
    Serial.println("Object detected!");
    objectDetected = true;
  } else {
    Serial.println("No object detected.");
    objectDetected = false;
  }

  // Move servo and reset countdown only if the countdown is completed and an object is detected
  if (countdownCompleted && objectDetected) {
    myServo.write(90); // Move the servo to 90 degrees
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    delay(1000); // Delay to allow the servo to move
    myServo.write(0); // Move the servo back to 0 degrees
    
    // Reset the countdown
    secondsRemaining = 60; // Reset the countdown timer
    countdownCompleted = false; // Restart the countdown
    previousMillis = currentMillis; // Reset the previousMillis for the next countdown
    lcd.setCursor(0, 0); // Set the cursor to the first row
    lcd.print("Countdown: "); // Print the text
    lcd.setCursor(7, 1); // Set the cursor to the second row
    lcd.print("    "); // Clear previous seconds
    lcd.setCursor(7, 1); // Set the cursor to the second row
    lcd.print(secondsRemaining); // Print initial time
  }
}
