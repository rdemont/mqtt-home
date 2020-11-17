#ifndef Functions_h
#define Functions_h

#include <Arduino.h>

class Functions
{
  private :
    Functions();
    static Functions* instance;
    
  public : 
    static Functions* getInstance();
  
    char* StringToChar(String str);

    //Return time in String 
    String strTime(long val);

    //return "true" or "false"
    String boolToString(bool val);
    bool stringToBool(String val);

    //return current time in second 
    long second();
};

#endif
