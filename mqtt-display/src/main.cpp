#include <Arduino.h>

#include <DHT.h>


#include "MqttHome.h"
#include "Functions.h"
#include "NetTime.h"
#include <SoftwareSerial.h>

struct mqttObjectStruct
{
  String objectName; 
  float minValue = 100.0;
  float maxValue = 0.0;
  struct tm minDate; 
  struct tm maxDate;
};

#define OBJECTSIZE 10
mqttObjectStruct mqttObjects[OBJECTSIZE] ;



void sendMqtt();
void sendDisplay(String objectname, String valuename, String value);

MqttHome mqttHome("IoTGFDisplay") ; 

#define DHTPIN D6     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino





int lastSecond = 0; 

void sendMqtt()
{
  float temperature = 0.0;
  float humidity = 0.0;
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();        

  mqttHome.mqttPublish(mqttHome.getMqttPath() + "/sensor/temperature",String(temperature));
  mqttHome.mqttPublish(mqttHome.getMqttPath() + "/sensor/humidity",String(humidity));

}


void sendDisplay(String objectname, String valuename, String value)
{

  Serial.print(objectname+"."+valuename+".txt=\""+value+"\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  //Serial.println("");
}


void setup() {
    Serial.begin(9600);
    
    //Serial1.begin(9600);

  //pinMode(1,FUNCTION_0); //TX
  //pinMode(3,FUNCTION_0); //RX

    mqttHome.wifiConnect();
    mqttHome.mqttConnect();
    mqttHome.mqttSubscribe("home/#");

    dht.begin();

    for (int i = 0 ; i < OBJECTSIZE;i++)
    {
      mqttObjects[i].objectName = ""; 
    }
    
}

String oldTime = ""; 
void updateTime()
{
  String date = NetTime::getInstance()->getDayOfWeekStr()+" ";
  date += NetTime::getInstance()->getDayStr()+".";
  date += NetTime::getInstance()->getMonthStr()+".";
  date += NetTime::getInstance()->getYearStr()+".";

  String time = NetTime::getInstance()->getHoursStr()+":"+NetTime::getInstance()->getMinutesStr();//+":"+NetTime::getInstance()->getSecondsStr();
  if (oldTime != time )
  {
    oldTime = time;
    for (int i = 0 ; i< OBJECTSIZE;i++)
    {
      if (mqttObjects[i].objectName != "" )
      {
        sendDisplay(mqttObjects[i].objectName,"time",time);
        sendDisplay(mqttObjects[i].objectName,"date",date);
      }
    }
  }
}

void loop() {

  NetTime::getInstance()->loop();
  // put your main code here, to run repeatedly:
  mqttHome.mqttLoop();
  if (mqttHome.getHasUnreadMessage())
  {
    MqttObject obj = mqttHome.getLastMessage();
    int index = -1 ;
    for (int i = 0 ; i< OBJECTSIZE;i++)
    {
      if ((mqttObjects[i].objectName == "") && (index == -1))
      {
        index = i;
      } else {
        if (mqttObjects[i].objectName == obj.objectname)
        {
          index = i ;
        }
      }
    }
    if (index != -1 )
    {
      mqttObjects[index].objectName = obj.objectname ; 
      if (obj.valuename == "temperature")
      {
        if (mqttObjects[index].maxValue < obj.value.toFloat())
        {
          mqttObjects[index].maxValue =  obj.value.toFloat();
          mqttObjects[index].maxDate =  NetTime::getInstance()->getNow();

          String date = NetTime::getInstance()->getDayStr()+".";
          date += NetTime::getInstance()->getMonthStr()+".";
          date += NetTime::getInstance()->getYearStr();
          String time = NetTime::getInstance()->getHoursStr()+":"+NetTime::getInstance()->getMinutesStr();
          sendDisplay(obj.objectname,"maxVal",String(round(obj.value.toFloat()*100)/100));
          sendDisplay(obj.objectname,"maxDate",date);
          sendDisplay(obj.objectname,"maxTime",time);
        }
        if (mqttObjects[index].minValue > obj.value.toFloat())
        {
          mqttObjects[index].minValue =  obj.value.toFloat();
          mqttObjects[index].minDate =  NetTime::getInstance()->getNow();
          String date = NetTime::getInstance()->getDayStr()+".";
          date += NetTime::getInstance()->getMonthStr()+".";
          date += NetTime::getInstance()->getYearStr();
          String time = NetTime::getInstance()->getHoursStr()+":"+NetTime::getInstance()->getMinutesStr();
          
          sendDisplay(obj.objectname,"minVal",String(round(obj.value.toFloat()*100)/100));
          sendDisplay(obj.objectname,"minDate",date);
          sendDisplay(obj.objectname,"minTime",time);          
        }        
      }
      

    }
    sendDisplay(obj.objectname,obj.valuename,obj.value);
  }


  long sec = Functions::getInstance()->second();
  if (sec != lastSecond)
  {
    updateTime();
    lastSecond = sec;
    if (sec%60 == 0)
    {
      sendMqtt();
      //updateTime();
    }
  }


}