#ifndef MqttHome_h
#define MqttHome_h

#include <Arduino.h>



class MqttObject
{
  public :
    
    MqttObject();
    String environment;
    String objectname; 
    String objecttype; 
    String valuename;
    String setter;    
    String value;
    bool isReaded ;  
    void setMessage(String chanel, String value);  
    
    
    
  private :
    String _objectName ;
  
};

    
class MqttHome
{
  public:
    MqttHome(String hostname);
    String getHostname();
    String getMqttBase();
    String getMqttPath();
    bool wifiConnect();
    bool mqttConnect();
    bool mqttLoop();
    void mqttPublish(char* chanel, char* value);
    void mqttPublish(String chanel, String value);
    void mqttSubscribe(char* chanel);
    void mqttUnsubscribe(char* chanel);
    
    long second();
    String strTime(long val);
    String boolToString(bool val);
    bool getHasUnreadMessage();
    MqttObject getLastMessage();

    
  private:
    String _hostname;
    String _mqttBase;
    String _mqttPath; 
    void mqttSendHeader();
    int _mqttLastHeaderSend = 0; 
    int _mqttLastReconnect = 0; 
    void callback(char *topic, byte *payload, unsigned int length);
    MqttObject _mqTTMessage ; 
    char* _subscribe[10];
    int _subscribeIndex;
    
    
};



    
#endif
