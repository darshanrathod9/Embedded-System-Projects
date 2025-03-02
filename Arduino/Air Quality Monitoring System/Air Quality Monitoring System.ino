#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

LiquidCrystal_I2C lcd(0x27,16,2);

DHT dht(DHTPIN, DHTTYPE);

int mq2Pin = A1;    // Analog pin connected to the MQ2 sensor
int mq135Pin = A0;  // Analog pin connected to the MQ135 sensor

void setup() {
  Serial.begin(9600); // Initialize serial communication
  lcd.init();
  lcd.backlight();
  dht.begin();

  lcd.setCursor(4,0);
  lcd.print("INDOOR AIR");
  lcd.setCursor(0,1);
  lcd.print("QUALITY MONITOR");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read analog values from the sensors
  float t = dht.readTemperature();                // Read temperature as Fahrenheit (isFahrenheit = true)
  float h = dht.readHumidity();                   // Read temperature as Celsius (the default)

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Print sensor values to Serial Monitor
    lcd.setCursor(0, 0);
    lcd.print("Temperature ");
    lcd.setCursor(0, 1);
    lcd.print(t);
    lcd.setCursor(6, 1);
    lcd.write(1);
    lcd.setCursor(7, 1);
    lcd.print("C");
    Serial.print("Temp: ");
    Serial.println(t);
    delay(4000);
    lcd.clear();
  
    // Display Humidity on LCD
    lcd.setCursor(0, 0);
    lcd.print("Humidity ");
    lcd.setCursor(0, 1);
    lcd.print(h);
    lcd.print("%");
    Serial.print("Humidity: ");
    Serial.println(h);
    delay(4000);
    lcd.clear();

    int mq2Value = analogRead(mq2Pin);
                                               // Display MQ2 Gas Value on LCD and Serial Monitor
    Serial.print("MQ2 Gas Value: ");
    Serial.println(mq2Value);
    lcd.setCursor(0, 0);
    lcd.print("Flameables: ");
    lcd.setCursor(0, 1);
    lcd.print(mq2Value);
    lcd.setCursor(6, 1);
    lcd.print("ppm ");
    delay(4000);
    lcd.clear();


    int mq135Value = analogRead(mq135Pin);
                                                 // Display MQ135 Gas Value on LCD and Serial Monitor
    Serial.print("MQ135 Gas Value: ");
    Serial.println(mq135Value);
    lcd.setCursor(0, 0);
    lcd.print("Pollution ");
    lcd.print(mq135Value);
    lcd.setCursor(6, 1);
    lcd.print("ppm ");    
    delay(4000);
    lcd.clear();
}
