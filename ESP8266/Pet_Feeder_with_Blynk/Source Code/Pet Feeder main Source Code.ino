#define BLYNK_TEMPLATE_ID       "TMPL3K5USBAAf"
#define BLYNK_TEMPLATE_NAME     "Pet Feeder"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
#define APP_DEBUG
#define USE_NODE_MCU_BOARD

#include "BlynkEdgent.h"
#include <WidgetRTC.h>

#define Food                D0
#define Water               D1 
#define FoodButton          D2
#define WaterButton         D3
#define FoodLevel_Trig      D5
#define FoodLevel_Echo      D6  
#define WaterLevel_Trig     D7
#define WaterLevel_Echo     D8


#define Bfoodandwater       V0
#define Bfood               V1
#define Bwater              V2
#define Alert               V3
#define Bfeeding_interval   V4


void Dispense_food();
void Dispense_water();
void checkFeedingTime();
void Level_check();

WidgetRTC rtc;
//BlynkTimer timer;

int lastFeedhour=0, lastFeedmin=0;
int interval=1, lastwatermin=0;
bool nightMode = false, Start = true;
int fb=1, wb=1;

void setup()
{
  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();
  rtc.begin();
  setSyncInterval(10 * 60); // Sync interval in seconds (every 30 minutes)
  //  timer.setInterval(60000L, checkFeedingStatus);

  pinMode(Food, OUTPUT);
  pinMode(Water, OUTPUT);
  pinMode(FoodLevel_Trig, OUTPUT);
  pinMode(WaterLevel_Trig, OUTPUT);

  pinMode(FoodButton, INPUT_PULLUP);
  pinMode(WaterButton, INPUT_PULLUP);
  pinMode(WaterLevel_Echo, INPUT);
  pinMode(FoodLevel_Echo, INPUT);

  digitalWrite(Food, HIGH);
  digitalWrite(Water, HIGH);
}

BLYNK_WRITE(Bfoodandwater)       // Dispense Food and water
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  if (value == 1) {
    Dispense_food();
    Dispense_water();
    Blynk.virtualWrite(Bfoodandwater, 0);
    Level_check();
  }
}

BLYNK_WRITE(Bfood)       // Serve Food
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  if (value) {
    Dispense_food();
    Blynk.virtualWrite(Bfood, 0);
    Level_check();
  }
}

BLYNK_WRITE(Bwater)       // Serve Water
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  if (value == 1) {
    Dispense_water();
    Level_check();
  }
}

BLYNK_WRITE(Bfeeding_interval) {
  interval = param.asInt();
}

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(Bfoodandwater);
  Blynk.syncVirtual(Bfood);
  Blynk.syncVirtual(Bwater);
  rtc.begin();
}

void loop() {

  BlynkEdgent.run();

  fb = digitalRead(FoodButton);
  wb = digitalRead(WaterButton);

  if (fb == 0) {
    delay(500);
    Dispense_food();
    Level_check();
  }

  if (wb == 0) {
    delay(500);
    Dispense_water();
    Level_check();
  }
}

void Dispense_food() {
  Blynk.virtualWrite(Bfood, 1);
  digitalWrite(Food, LOW);
  delay(3 * 1000);
  digitalWrite(Food, HIGH);
  Blynk.virtualWrite(Bfood, 0);
  lastFeedhour = hour();
  lastFeedmin = minute();
}

void Dispense_water() {
  Blynk.virtualWrite(Bwater, 1);
  digitalWrite(Water, LOW);
  delay(3 * 1000);
  digitalWrite(Water, HIGH);
  Blynk.virtualWrite(Bwater, 0);
  lastwatermin = minute();
}

void checkFeedingTime() {

  if ((lastFeedhour + interval == hour()) && (lastFeedmin == minute()) && (!nightMode) && (Start))
  {
    Dispense_food();
    Dispense_water();
  }

  if (hour() >= 22 || hour() < 6) {  // Check if it's night time (after 10 PM and before 6 AM)
    nightMode = true;
    Start = false;
  } else {
    nightMode = false;
  }
}

void Level_check()
{
  float foodlevel, waterlevel, durationF, durationW;
  
  digitalWrite(FoodLevel_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(FoodLevel_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(FoodLevel_Trig, LOW);

  durationF = pulseIn(FoodLevel_Echo, HIGH);
  foodlevel = durationF * 0.034/2;               //dist. in cm

  digitalWrite(WaterLevel_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(WaterLevel_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(WaterLevel_Trig, LOW);

  durationW = pulseIn(WaterLevel_Echo, HIGH);
  waterlevel = durationW * 0.034/2;              //dist. in cm

  if(foodlevel > 20 || waterlevel > 20)
  {
    Blynk.virtualWrite(Alert, 1);
    delay(5 * 1000);
    Blynk.virtualWrite(Alert, 0);
  }
}
