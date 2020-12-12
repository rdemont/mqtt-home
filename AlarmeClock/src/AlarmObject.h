#ifndef AlarmObject_h
#define AlarmObject_h

#include <Arduino.h>




class AlarmObject
{
  public :
    AlarmObject();
    void loop();
    int hours;
    int minutes;
    int days;
    int song;
    bool active ;
    
    bool hasDay(int day);  
    void addDay(int day);  
    void removeDay(int day);  
    void setAlarm(String setter, String value)  ;
    bool isRinging() ; 
    int getRemindCount();
    void setAcknowledge();
    const String ALARM_HOURS = "hours";
    const String ALARM_MINUTES = "minutes";
    const String ALARM_DAYS =  "days";
    const String ALARM_SONG = "song";
    const String ALARM_ACTIVE = "active";

  private :
    int remindCount = 0; // nombre de remind
    bool acknowledge ; // alarme validé  
    bool hasPopup = false ; 
    int minutesForReminde = 5 ; 
    int remindHours;
    int remindMinutes;
    const int MONDAY = 1;
    const int TUESDAY = 2 ;
    const int WEDNESDAY = 4 ; 
    const int THURSDAY = 8;
    const int FRIDAY = 16;
    const int SATURDAY = 32;
    const int SUNDAY = 64;
    int getBitwiseDay(int day) ;
    void setNextRemind();
  
};



#endif