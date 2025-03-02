#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

// Pin Definitions
#define SS_PIN         D0   // SDA pin for RFID to GPIO16
#define SDA            D1   // SDA 
#define SCL            D2   // SCL
#define RST_PIN        D3   // Reset pin for RFID (GPIO0)
#define BUZZER         D4   // BUZZER 
#define RELAY_PIN      D8   // Relay connected to GPIO15


// Initialize LCD (I2C address 0x27; change if your address is different)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialize RFID
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize Relay
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Ensure pump is off initially
  
  // Initialize LCD
  Wire.begin(SDA, SCL);
  lcd.init();
  lcd.backlight();
  lcd.print("Petrol Pump");
  delay(2000);
  lcd.clear();
  lcd.print("Scan RFID");

  // Initialize RFID
  SPI.begin();
  rfid.PCD_Init();

}

void loop() {
  // Check if a new RFID card is present
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;
  
    // Read the UID of the RFID card
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) 
    {
      uid += String(rfid.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
  
    // Print UID to Serial Monitor
    Serial.println("Card UID: " + uid);
  
    // Check if the card is authorized
    lcd.clear();
    if (uid == "441AF2")  // 3C9529
    { // Replace "ABCD1234" with your authorized UID
      lcd.print("Authorized User");
      delay(2000); // Wait 2 seconds before turning on the pump
  
      lcd.clear();
      lcd.print("Turning ON Pump");
      delay(2000);
      
      lcd.clear();
      lcd.print("Pump ON");
      digitalWrite(RELAY_PIN, LOW); // Turn pump ON
      delay(3000); // Pump runs for 3 seconds
      digitalWrite(RELAY_PIN, HIGH);  // Turn pump OFF
  
      lcd.clear();
      lcd.print("Pump OFF");
      delay(2000); // Display "Pump OFF" for 2 seconds
    } 
    else 
    {
      lcd.print("Unauthorized");
      digitalWrite(BUZZER, HIGH);  // Turn BUZZER ON
      delay(2000); // Display "Unauthorized" for 2 seconds
      digitalWrite(BUZZER, LOW);  // Turn BUZZER OFF
    }
  
    // Reset LCD to initial state
    lcd.clear();
    lcd.print("Scan RFID");
    

  // Halt the card to avoid reading the same card repeatedly
  rfid.PICC_HaltA();
}
