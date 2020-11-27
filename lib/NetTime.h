#ifndef NetTime_h
#define NetTime_h

#include <Arduino.h>
#include <Timezone.h> 

class NetTime
{
  private :
    NetTime();
    static NetTime* instance;
    Timezone* _timezone;
    time_t _prevDisplay;
    struct tm *  _localtime;
  public : 
    static NetTime* getInstance();
    void loop();
    struct tm getNow();
    int getSeconds();
    int getMinutes();
    int getHours();
    int getDay();
    int getMonth();
    int getYear();
    int getDayOfWeek();
    int getDayOfYear();
    String getSecondsStr();
    String getMinutesStr();
    String getHoursStr();
    String getDayStr();
    String getMonthStr();
    String getYearStr();    
    String getDayOfWeekStr();
    String getDayOfYearStr();
};

#endif