#include <UniversalTelegramBot.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ThingSpeak.h>
#include <HX711.h>
#include "HX711.h"

#define BOTID "6281076425:AAH5MaT6ScNacBIPF7IJfgMjhIcihqKgEas"
#define CH 2077783
#define WRITEAPI "6KHR9VVHME4P7C0R"
#define READAPI "GTE308YD1INF90YS"
#define CLK D5
#define DOUT D6 
#define SET D7
#define RESET D8
#define FULLRESET D0

HTTPClient web;
WiFiManager net;
HX711 scale(DOUT, CLK);
WiFiClientSecure nodemcu;
UniversalTelegramBot bot(BOTID, nodemcu);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

long long chat_id[10] = {5163176125};
int maxCount = 2;
float calibration_factor = -216235.00;
int TEMP,NEW = 0;
int stock = 0;

void internet();             //Connect to internet
void calib_factor();         //Change the calliberation Factor of scale
void telegram(int stock);    //telegram interactions with the bot 
void show(int stock);         //Display on lcd

void setup() 
{
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  Serial.begin(115200);
  ThingSpeak.begin(nodemcu);
  scale.set_scale(calibration_factor);
  scale.tare();
  nodemcu.setInsecure();
  internet();

  pinMode(SET, INPUT);
  pinMode(RESET, INPUT);
  pinMode(FULLRESET, INPUT);
}

void loop() 
{
  float obj = scale.get_units();
  TEMP = scale.get_units();
  float item_wt = 0, down, up;

  show(stock);
  if(WiFi.status() != WL_CONNECTED)
  {
    internet();
  }
  if(FULLRESET == HIGH)
  {
    ESP.restart();
  }
  if(RESET == HIGH)
  {
   scale.tare();
   item_wt = 0;
  }
  if(SET == HIGH)
  {
    item_wt = scale.get_units();
  }
    if(item_wt)
  {
    up = item_wt + (0.25*item_wt);
    down = item_wt - (0.25*item_wt);
    stock = scale.get_units()/item_wt ;
    show(stock);
    if(stock <= up && stock >= down && NEW != TEMP)
    {
      ThingSpeak.writeField(CH, 2, stock, WRITEAPI);
      for(int i = 0; i <= maxCount; i++)
      {
        bot.sendMessage(String(chat_id[i]), String(stock));
      }
      NEW = TEMP;
    } 
    else
    {
      lcd.setCursor(0,0);
      lcd.print("Error ");
    }
  }  

  telegram(stock);
  calib_factor();
}

void internet()
{
  lcd.setCursor(0,0);
  lcd.print("Connect to WiFi");
  lcd.setCursor(0,1);
  lcd.print("to Continue");
  delay(1000);
  net.autoConnect("NodeMcu");

  while(WiFi.status() != WL_CONNECTED)
  {
    net.autoConnect("NodeMcu"); 
    Serial.print("..");
  }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Connected");
    lcd.setCursor(0,1);
    lcd.print("to WiFi");
    delay(2000);
    lcd.clear();
    Serial.println("Connected to WiFi");
}

void calib_factor()
{
   if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 10;
    else if(temp == 's')
      calibration_factor += 100;  
    else if(temp == 'x')
      calibration_factor -= 100;  
    else if(temp == 'd')
      calibration_factor += 1000;  
    else if(temp == 'c')
      calibration_factor -= 1000;
    else if(temp == 'f')
      calibration_factor += 10000;  
    else if(temp == 'v')
      calibration_factor -= 10000;  
    else if(temp == 't')
      scale.tare();  //Reset the scale to zero
  }
}

void show(int stock)
{
  Serial.print("Weight = ");
  Serial.print(scale.get_units(),3);
  Serial.println(" Kg");
  lcd.setCursor(0,0);
  lcd.print("Weight(Kg)");
  lcd.setCursor(12,0);
  lcd.print("Unit");  
  lcd.setCursor(0,1);
  lcd.print(scale.get_units(),3);  
  lcd.setCursor(12,1);
  lcd.print(stock);  
}

void telegram(int stock)
{
  int newMessages = bot.getUpdates(bot.last_message_received + 1);    //Check for new messages
   bool authorised = false;
    
   for(int i=0;i < newMessages; i++)
   {
     for(int j=0; j < maxCount; j++)
     {
      Serial.println(i);
      Serial.println(j);
      Serial.print("chat id :");
      Serial.println(bot.messages[i].chat_id);
      for(int k =0; k < maxCount; k++)
      {
        if(String(bot.messages[i].chat_id) == String(chat_id[k])) //check for authorised users 
        {
          authorised = true;
          Serial.print("authorised user");
          break;
        }
      }     
     }
     if(!authorised)
     {
      Serial.print("Unauthorised user");
      continue;    //Ignore the below statements for unauthorised users      
     }
     
     String CHATID = String(bot.messages[i].chat_id);   // get the chat id of the bot 
     String TEXT = String(bot.messages[i].text);        // get the text of the message sent on the bot by the user 

     Serial.println(TEXT);
     newMessages = bot.getUpdates(bot.last_message_received + 1);  //update the no. of the message 
     
     if(TEXT == "/start")   //welcome text to be send at the start 
     {
        String welcome = "Welcome to the IoT Inventory Management Bot\n\n";
        welcome += "This bot gives you the stock update of your inventory\n";
        welcome += "You can control the action by the following commands\n";
        welcome += "/getstatus : To get the current stock info\n";
        welcome += "/getweight : To get the current weight on the shelf\n";
        
        bot.sendMessage(CHATID, welcome);
     } 
     else if(TEXT == "/getstatus")
     {
        String Status = String(stock);
        bot.sendMessage(CHATID, Status);
     }
     else if(TEXT == "/getweight")
     {
        String weight = String(scale.get_units(),3);
        weight += " Kg";
        bot.sendMessage(CHATID, weight);
     }
   }
}
