#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x3F address i2c

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SIGN LANGUAGE");

  delay(2000);
  pinMode(13, OUTPUT);
  pinMode(A1, INPUT);  
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}

void loop() {
  int s1 = analogRead(A1);
  int s2 = analogRead(A2);
  int s3 = analogRead(A3);

  // Assume threshold values for the flex sensors
  bool flexS1 = s1 < 35;
  bool flexS2 = s2 > 0;
  bool flexS3 = s3 < 210;

  Serial.println(s1);
  Serial.println(s2);
  Serial.println(s3);

  int gesture = (flexS1 << 2) | (flexS2 << 1) | flexS3;

  Serial.print("Gesture: ");
  Serial.println(gesture);

  lcd.clear();
  lcd.setCursor(0, 1);
  digitalWrite(13, LOW); // Reset LED state

  switch (gesture) {
    case 0:
      Serial.println("Nothing");
      lcd.print("    NOTHING        ");
      break;
    case 1:
      Serial.println("Gesture 1: Washroom");
      lcd.print("   WASHROOM        ");
      break;
    case 2:
      Serial.println("Gesture 2: Drink Water");
      lcd.print("  DRINK WATER      ");
      break;
    case 3:
      Serial.println("Gesture 3: Food");
      lcd.print("      FOOD         ");
      break;
    case 4:
      Serial.println("Gesture 4: Fruit");
      lcd.print("      FRUIT        ");
      break;
    case 5:
      Serial.println("Gesture 5: Sleep");
      lcd.print("      SLEEP        ");
      break;
    case 6:
      Serial.println("Gesture 6: Help");
      lcd.print("      HELP         ");
      break;
    case 7:
      Serial.println("Gesture 7: Call");
      lcd.print("      CALL         ");
      digitalWrite(13, HIGH); // For example, turning on the LED for CALL gesture
      break;
    default:
      Serial.println("Unknown Gesture");
      lcd.print("UNKNOWN GESTURE");
      break;
  }
delay(500);
}
