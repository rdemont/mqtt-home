#include <Arduino.h>

#include <DHT.h>

#include "MqttHome.h"
#include "Functions.h"


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
//  String pageName = "pDefault";
//  if (String("IoTGFGarage") == objectname) pageName = "pGarage";
//  else if (String("IoTBedFFEast") == objectname) pageName = "pEast";
//  else if (String("IoTBedFFWest") == objectname) pageName = "pWest";
//  else if (String("IoTGFGarden") == objectname) pageName = "pGarden";
//  else if (String("IoTGFFirePlace") == objectname) pageName = "pFirePlace";
//  else if (String("IoTGFDisplay") == objectname) pageName = "pDisplay";
  
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
    
}

void loop() {
  // put your main code here, to run repeatedly:
  mqttHome.mqttLoop();
  if (mqttHome.getHasUnreadMessage())
  {
    MqttObject obj = mqttHome.getLastMessage();
    sendDisplay(obj.objectname,obj.valuename,obj.value);
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


}