#include <Arduino.h>
#include <MqttHome.h>
#include <Functions.h>
#include <AlarmObject.h>


#include <NetTime.h>

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


MqttHome mqttHome("IoTFFNachaAlarmClock") ; 
int lastSecond = 0; 


Mp3voices mp3voices ;



#define MAXALARM 10
AlarmObject alarm[MAXALARM] ;


#define ALARME_SONG_FOLDER 4 

#define ALARME_SETUP_NONE 0 
#define ALARME_SETUP_HOUR 1 
#define ALARME_SETUP_MINUTE 2 
#define ALARME_SETUP_DAYS 3
#define ALARME_SETUP_SONG 4 
#define ALARME_SETUP_ACTIVE 5 
#define ALARME_SETUP_MAX 6 

int alarmSetUp = ALARME_SETUP_NONE;


void sendMqtt();
void sendMqttAlarms();
void setAlarmsAcknowledge();

void playTime()
{
  mp3voices.play(mp3voices.TXT_IL_EST);
  mp3voices.playNumber(NetTime::getInstance()->getHours());
  mp3voices.play(mp3voices.TXT_HEURE);
  mp3voices.playNumber(NetTime::getInstance()->getMinutes());
  mp3voices.play(mp3voices.TXT_MINUTES);
  mp3voices.playNumber(NetTime::getInstance()->getSeconds());
  mp3voices.play(mp3voices.TXT_SECONDS);
}

void setup() {

  Serial.begin(9600);

  mqttHome.wifiConnect();
  mqttHome.mqttConnect();
  mqttHome.mqttSubscribe(Functions::getInstance()->StringToChar(mqttHome.getMqttPath()+"/alarm/#"));

  mqttHome.mqttPublish(mqttHome.getMqttPath() +"/alarm/GETS", "true");  


  dht.begin();
  mp3voices.setup();
}

void loop() {
  mqttHome.mqttLoop();
  
  mp3voices.loop();

  btnTime.loop();
  btnVolUp.loop();
  btnVolDown.loop();
  btnAlarm.loop();

  NetTime::getInstance()->loop();

  

  if (mqttHome.getHasUnreadMessage())
  {
    MqttObject obj = mqttHome.getLastMessage();
    Serial.println(obj.objectname+"-"+obj.objecttype+":"+obj.valuename+"@"+obj.setter+"="+obj.value);
    int alarmIndex = -1 ;

    //Get alarm from mqtt 
    if ((obj.setter = "GETALARMS") 
      && (obj.value != NULL) 
      && (obj.value != "")
      && (Functions::getInstance()->stringToBool(obj.value)))
    {
      sendMqttAlarms();
    }else {
      if (Functions::getInstance()->isValidNumber(obj.valuename)) 
      {
        alarmIndex = obj.valuename.toInt();
      }
      if ((alarmIndex >= 0) && (alarmIndex < MAXALARM))
      {
        alarm[alarmIndex].setAlarm(obj.setter,obj.value);
      }
    }
  }

  long sec = Functions::getInstance()->second();
  if (sec != lastSecond)
  {
    lastSecond = sec;
    if(sec%60 == 0)
    {
      sendMqtt();
    }
  }

  //Ringing process
  for (int i = 0 ; i<MAXALARM;i++)
  {
    if (alarm[i].isRinging())
    {
      int volum = mp3voices.getVolume();
      if (alarm[i].getRemindCount()>1)
      {
        mp3voices.setVolume(VOLUM_MAX);
      }
      playTime(); 
      mp3voices.playFolder(ALARME_SONG_FOLDER,alarm[i].song);
      mp3voices.setVolume(volum);
    }
  }

  //Active Alarm info 
  if(btnTime.isReleasedLongPress())
  {
    setAlarmsAcknowledge();
    for (int i = 0 ; i<MAXALARM;i++)
    {
      //Active Alarm info 
      if (alarm[i].active)
      {
        mp3voices.play(mp3voices.TXT_L_ALARME_EST);
        mp3voices.play(mp3voices.TXT_ACTIVE);  
        mp3voices.play(mp3voices.TXT_REGLE_A);
        mp3voices.playNumber(alarm[i].hours);    
        mp3voices.play(mp3voices.TXT_HEURE);
        mp3voices.playNumber(alarm[i].minutes);    
        mp3voices.play(mp3voices.TXT_MINUTES); 
        mp3voices.play(mp3voices.TXT_AVEC_LA); 
        mp3voices.play(mp3voices.TXT_MUSIQUES); 
        mp3voices.playFolder(ALARME_SONG_FOLDER,alarm[i].song);
      }
    }
  }

  if(btnTime.isReleasedShortPress())
  {
    setAlarmsAcknowledge();
    Serial.println("Time short relaseed");    
    playTime();
  }

  if(btnVolUp.isReleasedLongPress())
  {
    setAlarmsAcknowledge();
  }  

  if(btnVolUp.isReleasedShortPress())
  {
    if (btnTime.isPressed())
    {
      //Internal Temperature 
    }else {
      setAlarmsAcknowledge();
      Serial.println("Volum UP"); 
      mp3voices.volumeUp();
      mp3voices.play(mp3voices.TXT_LE_VOLUME_EST_A);
      mp3voices.playNumber(mp3voices.getVolume());
    }
  }


  if(btnVolDown.isReleasedLongPress())
  {
    setAlarmsAcknowledge();
  }

  if(btnVolDown.isReleasedShortPress())
  {
    if (btnTime.isPressed())
    {
      //External Temperature 
    }else {
      Serial.println("Volum Down"); 
      mp3voices.volumeDown();
      mp3voices.play(mp3voices.TXT_LE_VOLUME_EST_A);
      mp3voices.playNumber(mp3voices.getVolume()); 
    }
  }
}
void setAlarmsAcknowledge()
{
  for (int i = 0 ; i<MAXALARM;i++)
  {
    alarm[i].setAcknowledge() ;
  }
}

void sendMqtt()
{
  Serial.println("Send MQTT");
  float temperature = 0.0;
  float humidity = 0.0;
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();        

  mqttHome.mqttPublish(mqttHome.getMqttPath() + "/sensor/temperature",String(temperature));
  mqttHome.mqttPublish(mqttHome.getMqttPath() + "/sensor/humidity",String(humidity));

}

void sendMqttAlarms()
{
  for (int i = 0 ; i< MAXALARM;i++)
  {
    mqttHome.mqttPublish(mqttHome.getMqttPath() +"/alarm/"+String(i)+"/hours", String(alarm[i].hours));  
    mqttHome.mqttPublish(mqttHome.getMqttPath() +"/alarm/"+String(i)+"/minutes", String(alarm[i].minutes));  
    mqttHome.mqttPublish(mqttHome.getMqttPath() +"/alarm/"+String(i)+"/song", String(alarm[i].song));  
    mqttHome.mqttPublish(mqttHome.getMqttPath() +"/alarm/"+String(i)+"/days", String(alarm[i].days));  
    mqttHome.mqttPublish(mqttHome.getMqttPath() +"/alarm/"+String(i)+"/active", Functions::getInstance()->boolToString(alarm[i].active));  
  }

}