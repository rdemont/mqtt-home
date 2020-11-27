
#include "NetTime.h"
#include <Arduino.h>


#include <NTPClient.h>
#include <WiFiUdp.h>

#include <TimeLib.h>
#include <time.h>
#include <Timezone.h> 


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
time_t getNtpTime()
{
    timeClient.update();      
    return timeClient.getEpochTime();
}


// Australia Eastern Time Zone (Sydney, Melbourne)
TimeChangeRule aEDT = {"AEDT", First, Sun, Oct, 2, 660};    // UTC + 11 hours
TimeChangeRule aEST = {"AEST", First, Sun, Apr, 3, 600};    // UTC + 10 hours
Timezone ausET(aEDT, aEST);
 
// Moscow Standard Time (MSK, does not observe DST)
TimeChangeRule msk = {"MSK", Last, Sun, Mar, 1, 180};
Timezone tzMSK(msk);
 
// Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     // Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       // Central European Standard Time
Timezone CE(CEST, CET);
 
// United Kingdom (London, Belfast)
TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time
Timezone UK(BST, GMT);
 
// UTC
TimeChangeRule utcRule = {"UTC", Last, Sun, Mar, 1, 0};     // UTC
Timezone UTC(utcRule);
 
// US Eastern Time Zone (New York, Detroit)
TimeChangeRule usEDT = {"EDT", Second, Sun, Mar, 2, -240};  // Eastern Daylight Time = UTC - 4 hours
TimeChangeRule usEST = {"EST", First, Sun, Nov, 2, -300};   // Eastern Standard Time = UTC - 5 hours
Timezone usET(usEDT, usEST);
 
// US Central Time Zone (Chicago, Houston)
TimeChangeRule usCDT = {"CDT", Second, Sun, Mar, 2, -300};
TimeChangeRule usCST = {"CST", First, Sun, Nov, 2, -360};
Timezone usCT(usCDT, usCST);
 
// US Mountain Time Zone (Denver, Salt Lake City)
TimeChangeRule usMDT = {"MDT", Second, Sun, Mar, 2, -360};
TimeChangeRule usMST = {"MST", First, Sun, Nov, 2, -420};
Timezone usMT(usMDT, usMST);
 
// Arizona is US Mountain Time Zone but does not use DST
Timezone usAZ(usMST);
 
// US Pacific Time Zone (Las Vegas, Los Angeles)
TimeChangeRule usPDT = {"PDT", Second, Sun, Mar, 2, -420};
TimeChangeRule usPST = {"PST", First, Sun, Nov, 2, -480};
Timezone usPT(usPDT, usPST);


//char* dayOfWeekNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char* dayOfWeekNames[] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};

NetTime* NetTime::instance = 0;

NetTime* NetTime::getInstance()
{
  if (instance == 0)
  {
    instance = new NetTime();
  }
  return instance; 
}

NetTime::NetTime()
{
    timeClient.begin();
    // Set offset time in seconds to adjust for your timezone, for example:
    // GMT +1 = 3600
    // GMT +8 = 28800
    // GMT -1 = -3600
    // GMT 0 = 0
    int timeOffset = 0 ; //3600; // winter // summer = 7200
    timeClient.setTimeOffset(timeOffset);    
    setSyncProvider(getNtpTime);
    _timezone = &CE;
}


void NetTime::loop()
{
    if (timeStatus() != timeNotSet) 
    {
        if (now() != _prevDisplay) 
        { //update the display only if time has changed
            _prevDisplay = now();
            time_t rawtime = _timezone->toLocal(_prevDisplay);
            _localtime = localtime (&rawtime);
        }
    }        
}

struct tm NetTime::getNow()
{
    return *_localtime;
}

int NetTime::getSeconds()
{
    return _localtime->tm_sec;
}
int NetTime::getMinutes()
{
    return _localtime->tm_min;
}
int NetTime::getHours()
{
    return _localtime->tm_hour;
}
int NetTime::getDay()
{
    return _localtime->tm_mday;
}
int NetTime::getMonth()
{
    return _localtime->tm_mon+1 ;
}
int NetTime::getYear()
{
    return _localtime->tm_year + 1900 ;
}
int NetTime::getDayOfWeek()
{
    return _localtime->tm_wday;
}
int NetTime::getDayOfYear()
{
    return _localtime->tm_yday;
}

String NetTime::getSecondsStr()
{
    return getSeconds() < 10 ? "0" + String(getSeconds()) : String(getSeconds());
}
String NetTime::getMinutesStr()
{
    return getMinutes() < 10 ? "0" + String(getMinutes()) : String(getMinutes());
}
String NetTime::getHoursStr()
{
    return getHours() < 10 ? "0" + String(getHours()) : String(getHours());
}
String NetTime::getDayStr()
{
    return getDay() < 10 ? "0" + String(getDay()) : String(getDay());
}
String NetTime::getMonthStr()
{
    return getMonth() < 10 ? "0" + String(getMonth()) : String(getMonth());
}
String NetTime::getYearStr()
{
    return String(getYear());
}

String NetTime::getDayOfWeekStr()
{
    return dayOfWeekNames[getDayOfWeek()];
}

String NetTime::getDayOfYearStr()
{
    return getDayOfYear() < 10 ? "00" + String(getDayOfYear()) : getDayOfYear() < 100 ? "0" + String(getDayOfYear()) : String(getDayOfYear()) ;
}