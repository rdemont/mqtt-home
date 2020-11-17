
#include <MqttHome.h>
#include <Functions.h>
#include <Arduino.h>



#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
 
#define SEALEVELPRESSURE_HPA (1013.25)


Adafruit_BME280 bme;

unsigned long delayTime;

void printValues() ;

void sendMqtt();
void sendDisplay(String objectname, String valuename, String value);

MqttHome mqttHome("IoTGFGarden") ; 

int lastSecond = 0; 

void sendMqtt()
{
  float temperature = 0.0;
  float humidity = 0.0;
  float pressure = 0.0;
  humidity = bme.readHumidity();
  temperature = bme.readTemperature(); 
  pressure = bme.readPressure() / 100.0F;       

  Serial.println("Send MQTT");

  mqttHome.mqttPublish(mqttHome.getMqttPath() + "/sensor/temperature",String(temperature));
  mqttHome.mqttPublish(mqttHome.getMqttPath() + "/sensor/humidity",String(humidity));
  mqttHome.mqttPublish(mqttHome.getMqttPath() + "/sensor/pressure",String(pressure));

  Serial.println("end send MQTT");

}

void setup() {
  Serial.begin(9600);
  delayTime = 1000;
  //delay(100);

  bme.begin(0x76); 

  mqttHome.wifiConnect();
  mqttHome.mqttConnect();  

  Serial.println("Start");
}


void loop() { 
  mqttHome.mqttLoop();

  long sec = Functions::getInstance()->second();
  if (sec != lastSecond)
  {
    
    lastSecond = sec;
    if (sec%60 == 0)
    {
      sendMqtt();
      printValues();
    }
  }
}

void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}