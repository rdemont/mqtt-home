#include <ESP8266WiFi.h>

#include <PubSubClient.h>

#include "MqttHome.h"
#include "Functions.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

MqttHome::MqttHome(String hostname)
{
  _hostname = hostname;
  _mqttBase = "home/";
  _mqttPath = _mqttBase+String(_hostname);
  _subscribeIndex = -1; 
}


bool MqttHome::wifiConnect()
{
  WiFi.begin("HomeSweetHome", "qwert12345");
  WiFi.hostname(_hostname);
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  
  return true ; 
}



void MqttHome::callback(char *topic, byte *payload, unsigned int length) 
{
    //Serial.print("Message arrived in topic: ");
    //Serial.println(topic);
    //Serial.print("Message:");
    String value = "";
    for (int i = 0; i < length; i++) {
        value += (char) payload[i];
    }
    
    //Serial.println(value);
    //Serial.println("-----------------------");
    
    _mqTTMessage.setMessage(topic,value);
}

bool MqttHome::mqttLoop()
{
  mqttClient.loop();

  long sec = Functions::getInstance()->second();
  
  //Serial.print("Time-sec: ");
  //Serial.println(sec);
  
  if (sec != _mqttLastReconnect)
  {
    if ((sec-_mqttLastReconnect)%600 == 0)
    {
      
      //Serial.println("Send header");
      //wifiConnect();
      _mqttLastReconnect = sec;
      mqttClient.disconnect();

      //mqttConnect();
      //mqttSendHeader();
    }
  }


  if (sec != _mqttLastHeaderSend)
  {
    if ((sec-_mqttLastHeaderSend)%60 == 0)
    {
      
      //Serial.println("Send header");
      //wifiConnect();
      _mqttLastHeaderSend = sec;
      mqttConnect();
      mqttSendHeader();
    }
  }
  

  
}

bool MqttHome::mqttConnect()
{
  if (! mqttClient.connected())
  {

    char _mqttServer[] = "192.168.1.10";
    int _mqttPort = 1883;

    mqttClient.setServer(_mqttServer, _mqttPort);
    mqttClient.setCallback([this](char* topic, byte* payload, unsigned int length) {this->callback(topic, payload, length); });
    //mqttClient.setCallback(this->callback);
    while (!mqttClient.connected()) 
    {
      Serial.println("Connecting to mqtt broker.....");
      Serial.println(Functions::getInstance()->StringToChar(_hostname+"_client"));
      Serial.println("End Name");
      if (mqttClient.connect(Functions::getInstance()->StringToChar(_hostname+"_client"))) {
        Serial.println("Mqtt broker connected");
      } else {
        Serial.print("failed with state ");
        Serial.print(mqttClient.state());
        delay(2000);
      }
    }

    for (int i = 0 ; i <= _subscribeIndex;i++)
    {
      mqttClient.subscribe(_subscribe[i]);
    }

  }

}

void MqttHome::mqttPublish(String chanel, String value)
{
  mqttPublish(Functions::getInstance()->StringToChar(chanel),Functions::getInstance()->StringToChar(value));
}

void MqttHome::mqttPublish(char* chanel, char* value)
{
  
  mqttClient.publish(chanel, value);
}


void MqttHome::mqttSubscribe(char* chanel)
{
  int index = _subscribeIndex +1 ; 
  if (index >= 10)
  {
    index = 0;
  }
  for (int i =0 ; i< _subscribeIndex+1;i++)
  {
    if (_subscribe[i]==chanel)
    {
      index = i ; 
    }
  }
  if (_subscribeIndex < index)
  {
    _subscribeIndex = index ;
  }
  _subscribe [index] = chanel ;

  mqttClient.subscribe(chanel);
}
void MqttHome::mqttUnsubscribe(char* chanel)
{
  int index = _subscribeIndex ; 
  bool find = false ; 
  for (int i=0;i<_subscribeIndex;i++)
  {
    if (find)
    {
      _subscribe[i-i] = _subscribe[i];
    }
    if (_subscribe[i]==chanel)
    {
      find = true ; 
    }
  }
  if (find)
  {
    _subscribeIndex-- ;
  }

  mqttClient.unsubscribe(chanel);
}


void MqttHome::mqttSendHeader()
{
  IPAddress ip = WiFi.localIP();
  mqttPublish(_mqttPath + "/iot/ip",String(ip[0])+"."+String(ip[1])+"."+String(ip[2])+"."+String(ip[3]));
  mqttPublish(_mqttPath + "/iot/uptime",Functions::getInstance()->strTime(Functions::getInstance()->second()));
}



String MqttHome::getMqttBase()
{
  return _mqttBase;
}

String MqttHome::getMqttPath()
{
  return _mqttPath;
}

String MqttHome::getHostname() 
{
  return _hostname ;
}










bool MqttHome::getHasUnreadMessage()
{
  return !_mqTTMessage.isReaded;
}

MqttObject MqttHome::getLastMessage()
{
  _mqTTMessage.isReaded = true ; 
  return _mqTTMessage;
}


MqttObject::MqttObject()
{
  environment = "";
  objectname = "";
  objecttype = "";
  valuename = "";
  setter = "";
  value = "";
  isReaded = true ; 
}

void MqttObject::setMessage(String chanel, String v)
{
  environment = "";
  objectname = "";
  objecttype = "";
  valuename = "";
  setter = "";
  value = v;
  isReaded = false ; 
  
  int step = 0 ; 
  String tt = "" ; 
  for (int i = 0 ; i< chanel.length();i++)
  {
  
    if(chanel.charAt(i) == '/')
    {

      switch (step)
      {
        case 0: 
          environment = tt ;
          break ; 
        case 1:
          objectname = tt ; 
          break ;
        case 2: 
          objecttype = tt ;
          break ;
        case 3:
          valuename = tt ; 
          break;
      }

      step += 1;
      tt = "";
    
    }else {
      tt += chanel.charAt(i);
    }
  }

  switch (step)
  {
    case 0: 
      environment = tt ;
      break ; 
    case 1:
      objectname = tt ; 
      break ;
    case 2: 
      objecttype = tt ;
      break ;      
    case 3:
      valuename = tt ; 
      break;
    case 4:
      setter = tt ; 
      break;      
  }  
 
  //Serial.println(objecttype+"**"+valuename+"  "+setter+"--"+value);
}
