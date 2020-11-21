#include <Arduino.h>
#include <MqttHome.h>
#include <Functions.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#include <Mp3voices.h>
#include <Button.h>

#define BTN_TIME D3
#define BTN_VOLUM_UP D4
#define BTN_VOLUM_DOWN D5
#define BTN_ALARM D6

Button btnTime(BTN_TIME);
Button btnVolUp(BTN_VOLUM_UP);
Button btnVolDown(BTN_VOLUM_DOWN);
Button btnAlarm(BTN_ALARM);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

MqttHome mqttHome("IoTFFNachaAlarmClock") ; 

Mp3voices mp3voices ;

struct ALARM {
  int hours;
  int minutes;
  bool active ;
};

ALARM alarm = {0,0,false}; 

void setup() {

  Serial.begin(9600);
  mqttHome.wifiConnect();
  mqttHome.mqttConnect();

  timeClient.begin();
    // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  int timeOffset = 3600; // winter // summer = 7200
  timeClient.setTimeOffset(3600);
}

void loop() {
  mqttHome.mqttLoop();

  btnTime.loop();
  btnVolUp.loop();
  btnVolDown.loop();
  btnAlarm.loop();

  timeClient.update();

  if(btnTime.isRelaseedShortPress())
  {
    mp3voices.play(mp3voices.TXT_IL_EST);
    mp3voices.playNumber(timeClient.getHours());
    mp3voices.play(mp3voices.TXT_HEURE);
    mp3voices.playNumber(timeClient.getMinutes());
    mp3voices.play(mp3voices.TXT_MINUTES);
  }

  if(btnVolUp.isRelaseedShortPress())
  {
    mp3voices.volumeUp();
    mp3voices.play(mp3voices.TXT_LE_VOLUME_EST_A);
    mp3voices.playNumber(timeClient.getHours());
  }

  if(btnVolDown.isRelaseedShortPress())
  {
    mp3voices.volumeDown();
    mp3voices.play(mp3voices.TXT_LE_VOLUME_EST_A);
    mp3voices.playNumber(timeClient.getHours()); 
  }

  if(btnAlarm.isRelaseedShortPress())
  {

    mp3voices.play(mp3voices.TXT_L_ALARME_EST);
    if (alarm.active)
    {
      mp3voices.play(mp3voices.TXT_ACTIVE);  
    }else {
      mp3voices.play(mp3voices.TXT_DESACTIVE);  
    }
    mp3voices.play(mp3voices.TXT_REGLE_A);
    mp3voices.playNumber(alarm.hours);    
    mp3voices.play(mp3voices.TXT_HEURE);
    mp3voices.playNumber(alarm.minutes);    
    mp3voices.play(mp3voices.TXT_MINUTES);
  }

  //Serial.println(timeClient.getFormattedTime());
  //delay(1000);
}


