#include <Arduino.h>
#include <MqttHome.h>
#include <Functions.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#include <Mp3voices.h>
#include <Button.h>

#include <DHT.h>

#define DHTPIN D7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


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
int lastSecond = 0; 


Mp3voices mp3voices ;

struct ALARM {
  int hours;
  int minutes;
  int song;
  bool active ;
  bool acknowledge ; // alarme validé 
};



ALARM alarm = {0,0,0,false,true}; 
//ALARM alarm = {21,7,0,true,true}; // test

#define ALARME_SONG_FOLDER 4 

#define ALARME_SETUP_NONE 0 
#define ALARME_SETUP_HOUR 1 
#define ALARME_SETUP_MINUTE 2 
#define ALARME_SETUP_SONG 3 
#define ALARME_SETUP_ACTIVE 4 
#define ALARME_SETUP_MAX 4 

int alarmSetUp = ALARME_SETUP_NONE;


void sendMqtt();


void playTime()
{
      mp3voices.play(mp3voices.TXT_IL_EST);
      mp3voices.playNumber(timeClient.getHours());
      mp3voices.play(mp3voices.TXT_HEURE);
      mp3voices.playNumber(timeClient.getMinutes());
      mp3voices.play(mp3voices.TXT_MINUTES);
      mp3voices.playNumber(timeClient.getSeconds());
      mp3voices.play(mp3voices.TXT_SECONDS);
}

void setup() {

  Serial.begin(9600);

  mqttHome.wifiConnect();
  mqttHome.mqttConnect();
  mqttHome.mqttSubscribe(Functions::getInstance()->StringToChar(mqttHome.getMqttPath()+"/alarm"));


  dht.begin();

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

  if (mqttHome.getHasUnreadMessage())
  {
    MqttObject obj = mqttHome.getLastMessage();

  }

  long sec = Functions::getInstance()->second();
  if (sec != lastSecond)
  {
    
    lastSecond = sec;
    if (sec%60 == 0)
    {
      sendMqtt();
    }
  }

  if ((alarm.active) && (millis() %1000 == 0))
  {
    if ((timeClient.getHours() == alarm.hours) && (timeClient.getMinutes() == alarm.minutes) && (timeClient.getSeconds() == 0))
    {
      //Alarm 
      alarm.acknowledge = false; 
      playTime();
      mp3voices.playFolder(ALARME_SONG_FOLDER,alarm.song);
    } else {
      if ((!alarm.acknowledge) && (timeClient.getSeconds() == 0)) // alarm pas arrêté 
      {
        int volum = mp3voices.getVolume();
        mp3voices.setVolume(VOLUM_MAX);
        playTime();
        mp3voices.playFolder(ALARME_SONG_FOLDER,alarm.song);
        mp3voices.setVolume(volum);
      }
    }
    
  }
  
  if(btnTime.isReleasedLongPress())
  {
    alarm.acknowledge = true ;
    if (alarmSetUp == 0)
    {
      alarmSetUp++ ; 
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
      mp3voices.play(mp3voices.TXT_AVEC_LA); 
      mp3voices.play(mp3voices.TXT_MUSIQUES); 
      mp3voices.playFolder(ALARME_SONG_FOLDER,alarm.song);
    }else {
      alarmSetUp = 0 ; 
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
      mp3voices.play(mp3voices.TXT_AVEC_LA); 
      mp3voices.play(mp3voices.TXT_MUSIQUES); 
      mp3voices.playFolder(ALARME_SONG_FOLDER,alarm.song);  
    }
  }

  if(btnTime.isReleasedShortPress())
  {
    alarm.acknowledge = true ;
    if (alarmSetUp > 0 ) 
    {
      alarmSetUp++;
      if (alarmSetUp>ALARME_SETUP_MAX) alarmSetUp=1;
      switch(alarmSetUp){
        case ALARME_SETUP_HOUR :
          mp3voices.play(mp3voices.TXT_REGLAGE_DES);  
          mp3voices.play(mp3voices.TXT_HEURE);
          break ; 
        case ALARME_SETUP_MINUTE :
          mp3voices.play(mp3voices.TXT_REGLAGE_DES);  
          mp3voices.play(mp3voices.TXT_MINUTES);
          break ; 
        case ALARME_SETUP_SONG :
          mp3voices.play(mp3voices.TXT_REGLAGE_DES);  
          mp3voices.play(mp3voices.TXT_MUSIQUES);   
          break ;                     
        case ALARME_SETUP_ACTIVE :
          mp3voices.play(mp3voices.TXT_L_ALARME_EST);  
          if (alarm.active)
          {
            mp3voices.play(mp3voices.TXT_ACTIVE);   
          }else {
            mp3voices.play(mp3voices.TXT_DESACTIVE);
          }
            
          break ;                     

      }
      
    }else {
      Serial.println("Time short relaseed");    
      playTime();
    }
  }

  if(btnVolUp.isReleasedShortPress())
  {
    alarm.acknowledge = true ;
  }

  if(btnVolUp.isReleasedShortPress())
  {
    alarm.acknowledge = true ;
    if (alarmSetUp > 0 ) 
    {
      switch(alarmSetUp){
        case ALARME_SETUP_HOUR :
          alarm.hours++; 
          if (alarm.hours > 23 )alarm.hours = 0 ; 
          mp3voices.playNumber(alarm.hours);
          break ; 
        case ALARME_SETUP_MINUTE :
          alarm.minutes++; 
          if (alarm.minutes > 59 )alarm.minutes = 0 ; 
          mp3voices.playNumber(alarm.minutes);
          break ; 
        case ALARME_SETUP_SONG :
          alarm.song++; 
          if (alarm.song > mp3voices.fileCountsInForder(ALARME_SONG_FOLDER)) alarm.song = 0 ; 
          mp3voices.playFolder(ALARME_SONG_FOLDER,alarm.song);
          
          break ;    
        case ALARME_SETUP_ACTIVE :
          alarm.active = !alarm.active ;
          mp3voices.play(mp3voices.TXT_L_ALARME_EST);  
          if (alarm.active)
          {
            mp3voices.play(mp3voices.TXT_ACTIVE);   
          }else {
            mp3voices.play(mp3voices.TXT_DESACTIVE);
          }        
        break;                 
      }
    }else {
      Serial.println("Volum UP"); 
      mp3voices.volumeUp();
      mp3voices.play(mp3voices.TXT_LE_VOLUME_EST_A);
      mp3voices.playNumber(timeClient.getHours());
    }
  }

  if(btnVolDown.isReleasedLongPress())
  {
    alarm.acknowledge = true ;
  }

  if(btnVolDown.isReleasedShortPress())
  {
    alarm.acknowledge = true ;
    if (alarmSetUp > 0 ) 
    {
      switch(alarmSetUp){
        case ALARME_SETUP_HOUR :
          alarm.hours--; 
          if (alarm.hours < 0 )alarm.hours = 23 ; 
          mp3voices.playNumber(alarm.hours);
          break ; 
        case ALARME_SETUP_MINUTE :
          alarm.minutes++; 
          if (alarm.minutes < 0 )alarm.minutes = 59 ; 
          mp3voices.playNumber(alarm.minutes);
          break ; 
        case ALARME_SETUP_SONG :
          alarm.song--; 
          if (alarm.song < 0) alarm.song = mp3voices.fileCountsInForder(ALARME_SONG_FOLDER) ; 
          mp3voices.playFolder(ALARME_SONG_FOLDER,alarm.song);
          
          break ;    
        case ALARME_SETUP_ACTIVE :
          alarm.active = !alarm.active ;
          mp3voices.play(mp3voices.TXT_L_ALARME_EST);  
          if (alarm.active)
          {
            mp3voices.play(mp3voices.TXT_ACTIVE);   
          }else {
            mp3voices.play(mp3voices.TXT_DESACTIVE);
          }        
        break;   
      }              
    }else {
      Serial.println("Volum Down"); 
      mp3voices.volumeDown();
      mp3voices.play(mp3voices.TXT_LE_VOLUME_EST_A);
      mp3voices.playNumber(timeClient.getHours()); 
    }
  }
}

void sendMqtt()
{
  float temperature = 0.0;
  float humidity = 0.0;
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();        

  mqttHome.mqttPublish(mqttHome.getMqttPath() + "/sensor/temperature",String(temperature));
  mqttHome.mqttPublish(mqttHome.getMqttPath() + "/sensor/humidity",String(humidity));

  String str = String(alarm.hours)+";"+String(alarm.minutes)+";"+String(alarm.song)+";"+Functions::getInstance()->boolToString(alarm.active);
  mqttHome.mqttPublish(mqttHome.getMqttPath() +"/alarm/", str);  

}