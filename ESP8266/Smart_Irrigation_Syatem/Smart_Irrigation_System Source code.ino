#define BLYNK_TEMPLATE_ID "TMPL3sypUUlv5"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation"
#define BLYNK_AUTH_TOKEN "wfg689dqml08XDFv7EYjsp_FN2pIFlp8"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//#include <DHT.h>
//
//#define DHTPIN 2    // Digital pin connected to the DHT sensor
//#define DHTTYPE DHT11   // DHT 11

//DHT dht(DHTPIN, DHTTYPE);

#define SOIL_MOISTURE_PIN     A0   // Analog pin for soil moisture sensor
#define PUMP_PIN              D0            // Digital pin for controlling the pump
#define SOIL_MOISTURE_VPIN    V0  // Virtual pin for soil moisture
#define THRESHOLD_VPIN        V1      // Virtual pin for threshold value
#define PUMP_SWITCH_VPIN      V2    // Virtual pin for manual pump control
#define HUMID                 V3
#define TEMP                  V4

char ssid[] = "HARSH";
char pass[] = "HARSH123";

BlynkTimer timer;

bool isPumpOn = false;  // Variable to track pump status
int thresholdMoisture;  // Variable to store threshold moisture value

void sendSensorData()
{
    int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
    Serial.print("Soil Moisture: ");
    Serial.println(soilMoisture);
 
    int soilMoisturePercentage = map(soilMoisture, 400, 1023, 100, 0);
    Serial.print("Soil Moisture Percentage: ");
    Serial.println(soilMoisturePercentage);

    Blynk.virtualWrite(SOIL_MOISTURE_VPIN, soilMoisturePercentage);

    if (soilMoisturePercentage < thresholdMoisture && !isPumpOn)
    {
        digitalWrite(PUMP_PIN, HIGH);
        Serial.println("Soil moisture is below the threshold! Pump turned on.");
        Blynk.logEvent("moisture_alert", "Soil moisture is below the threshold!");
        Blynk.virtualWrite(PUMP_SWITCH_VPIN, 1);
        delay(2000);  // Pump runs for 2 seconds
        digitalWrite(PUMP_PIN, LOW);
        Blynk.virtualWrite(PUMP_SWITCH_VPIN, 0);
        Serial.println("Pump turned off after 2 seconds.");
    }
}

BLYNK_WRITE(THRESHOLD_VPIN)
{
    thresholdMoisture = param.asInt();
    Serial.print("New threshold moisture value: ");
    Serial.println(thresholdMoisture);
}

BLYNK_WRITE(PUMP_SWITCH_VPIN)
{
    isPumpOn = param.asInt();
    if (isPumpOn) {
        digitalWrite(PUMP_PIN, HIGH);
        Serial.println("Pump manually turned ON");
        Blynk.virtualWrite(PUMP_SWITCH_VPIN, 1);
        delay(2000);  // Pump runs for 2 seconds
        digitalWrite(PUMP_PIN, LOW);
        Blynk.virtualWrite(PUMP_SWITCH_VPIN, 0);
        isPumpOn = false;  // Reset the manual control state after turning off the pump
        Blynk.virtualWrite(PUMP_SWITCH_VPIN, isPumpOn);  // Update the app to show the pump is off
        Serial.println("Pump turned off after 2 seconds.");
    } else {
        digitalWrite(PUMP_PIN, LOW);
        Blynk.virtualWrite(PUMP_SWITCH_VPIN, 0);
        Serial.println("Pump manually turned OFF");
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(PUMP_PIN, OUTPUT);

    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    Serial.print("Started");

  //  dht.begin();

    timer.setInterval(3000L, sendSensorData);

    Blynk.syncVirtual(PUMP_SWITCH_VPIN);
    Blynk.syncVirtual(THRESHOLD_VPIN);
}

void loop()
{
    Blynk.run();
    timer.run();
//
//    float t = dht.readTemperature();                // Read temperature as Fahrenheit (isFahrenheit = true)
//    float h = dht.readHumidity();                   // Read temperature as Celsius (the default)
//
//  if (isnan(h) || isnan(t) ) {
//    Serial.println(F("Failed to read from DHT sensor!"));
//    return;
//  }
//
//  if (isnan(h) || isnan(t) ) {
//    Serial.println(F("Failed to read from DHT sensor!"));
//    return;
//  }
//
//  Blynk.virtualWrite(HUMID, h);
//  Blynk.virtualWrite(TEMP, t);
    
}
