
#include "Functions.h"
#include <Arduino.h>

Functions* Functions::instance = 0;

Functions* Functions::getInstance()
{
  if (instance == 0)
  {
    instance = new Functions();
  }
  return instance; 
}

Functions::Functions()
{}


boolean Functions::isValidNumber(String str)
{
  boolean isNum=false;
  for(byte i=0;i<str.length();i++)
  {
    isNum = isDigit(str.charAt(i)) || str.charAt(i) == '+' || str.charAt(i) == '.' || str.charAt(i) == '-';
    if(!isNum) return false;
  }
  return isNum;
} 

char* Functions::StringToChar(String str)
{

  if(str.length()!=0)
  {
      char *p = const_cast<char*>(str.c_str());
      return p;
  }
}

long Functions::second()
{
  return millis()/1000;
}


String Functions::boolToString(bool val)
{
  if (val)
  {
    return "true";
  }else {
    return "false";
  }
}


bool Functions::stringToBool(String val)
{
  return val.equalsIgnoreCase("TRUE");
}

String Functions::strTime(long val)
{  
  
  int j = val / (24 * 60 * 60); // Nombre de jours. ça,donne 0 si on n'a pas encore passé un jour
  int h = (val - (j*24 * 60 * 60)) / (60 * 60); // on soustrait à t le nb de jours en ms (ou 0 si J valait 0), donc reste moins d'un jour qu'on divise par Le nombre de ms dans 1h (3600 secondes * 1000ms par secondes)
  int m = (val - ((j*24 * 60 * 60) + (h*60 * 60))) / 60; // nb de minutes (1 min = 60s = 60000ms)
  int s = val - ((j*24 * 60 * 60) + (h*60 * 60) + (m* 60)); //

  String result = String(j)+"d " ;
  if (h<10)
  {
    result += "0";
  }
  result += String(h)+":";
  if (m<10)
  {
    result += "0";
  }
  result += String(m)+":";
  if(s<10)
  {
    result += "0";
  }
  result += String(s);

  //Serial.println(result);
  return result; 

}
