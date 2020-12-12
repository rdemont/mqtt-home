
#include "Functions.h"

#include "AlarmObject.h"

#include "NetTime.h"


AlarmObject::AlarmObject()
{
    hours = 0;
    minutes = 0;
    days = 0;
    song = 0;
    active = false;
    acknowledge = true; // alarme validé  
}


bool AlarmObject::hasDay(int day)
{
    switch (day)
    {
        case 0 :  
            return (SUNDAY & days) == SUNDAY;  
        case 1 :  
            return (MONDAY & days) == SUNDAY;
        case 2 :  
            return (TUESDAY & days) == TUESDAY;
        case 3 :  
            return (WEDNESDAY & days) == WEDNESDAY;
        case 4 :  
            return (THURSDAY & days) == THURSDAY;
        case 5 :  
            return (FRIDAY & days) == FRIDAY;
        case 6 : 
            return (SATURDAY & days) == SATURDAY;  
            
    }
    return false ;         
} 

int AlarmObject::getBitwiseDay(int day) 
{
    switch (day)
    {
        case 0 :  
            return SUNDAY;  
        case 1 :  
            return MONDAY;
        case 2 :  
            return TUESDAY;
        case 3 :  
            return WEDNESDAY;
        case 4 :  
            return THURSDAY;
        case 5 :  
            return FRIDAY;
        case 6 : 
            return SATURDAY;  
    }
    return 0 ;
}

void AlarmObject::addDay(int day)
{
    days = days | getBitwiseDay(day) ; 
} 

void AlarmObject::removeDay(int day)
{
    days = days ^ getBitwiseDay(day) ;
}

bool AlarmObject::isRinging() 
{
    if ((hasDay(NetTime::getInstance()->getDayOfWeek())) 
        && (NetTime::getInstance()->getHours() == hours) 
        && (NetTime::getInstance()->getMinutes() == minutes)
        && (!hasPopup))
    {
        hasPopup = true ;
        remindCount = 0; 
        setNextRemind(); 
        return true ; 
    }

    if ((hasPopup) 
        && (NetTime::getInstance()->getHours() == remindHours) 
        && (NetTime::getInstance()->getMinutes() == remindMinutes))
    {
        remindCount ++ ;
        setNextRemind(); 
        return true ; 
    }

    if (hasPopup && (remindCount > 10))
    {
        setAcknowledge(); 
    }

    if ((hasPopup) 
        && (NetTime::getInstance()->getHours() == remindHours) 
        && (NetTime::getInstance()->getMinutes() == remindMinutes)
        && acknowledge)
    {
        hasPopup = false ; // ready for next time  
    }
    return false ; 
} 

void AlarmObject::setNextRemind()
{
    remindMinutes += minutesForReminde ; 
    if (remindMinutes >= 60)
    {
        remindHours ++ ; 
        remindMinutes -= 60 ; 
    }
    if (remindHours >= 24)
    {
        remindHours -= 24 ; 
    }
}

int AlarmObject::getRemindCount()
{
    return remindCount ; 
}
void AlarmObject::setAcknowledge()
{
    remindCount = 0; 
    acknowledge = true ; 
}

void AlarmObject::setAlarm(String setter, String value) 
{

    if ((setter == ALARM_HOURS) && (Functions::getInstance()->isValidNumber(value)))
    {
        hours = value.toInt();
    }
    if ((setter == ALARM_MINUTES) && (Functions::getInstance()->isValidNumber(value)))
    {
        minutes = value.toInt();
    }
    if ((setter == ALARM_SONG) && (Functions::getInstance()->isValidNumber(value)))
    {
        song = value.toInt();
    }      
    if ((setter == ALARM_ACTIVE) && (value != NULL) && (value != ""))
    {
        active = Functions::getInstance()->stringToBool(value);
    }      
    if ((setter == ALARM_DAYS) && (Functions::getInstance()->isValidNumber(value)))
    {
        days = value.toInt();
    }   
}