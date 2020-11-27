
#include <Arduino.h>

#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>


#include "Mp3voices.h"



Mp3voices::Mp3voices()
{
    SoftwareSerial s(10, 11); // RX, TX
    DFRobotDFPlayerMini p ;
    softwareSerial = &s;
    player = &p ;
    softwareSerial->begin(9600);
}


Mp3voices::Mp3voices(int rxPin, int txPin)
{
    SoftwareSerial s(rxPin, txPin); // RX, TX
    DFRobotDFPlayerMini p ;
    softwareSerial = &s;
    player = &p ;
    softwareSerial->begin(9600);
}



void Mp3voices::setup()
{
    if (!player->begin(*softwareSerial)) 
    {  //Use softwareSerial to communicate with mp3.
        Serial.println(F("Unable to begin:"));
        Serial.println(F("1.Please recheck the connection!"));
        Serial.println(F("2.Please insert the SD card!"));
        while(true);
    }
    Serial.println(F("DFPlayer Mini online."));
    player->setTimeOut(500); //Set serial communictaion time out 500ms
    player->volume(10);  //Set volume value (0~30).
  
    //----Set different EQ----
    player->EQ(DFPLAYER_EQ_NORMAL);
    //player->EQ(DFPLAYER_EQ_POP);
    //player->EQ(DFPLAYER_EQ_ROCK);
    //player->EQ(DFPLAYER_EQ_JAZZ);
    //player->EQ(DFPLAYER_EQ_CLASSIC);
    //player->EQ(DFPLAYER_EQ_BASS);
  
    //----Set device we use SD as default----
    //player->outputDevice(DFPLAYER_DEVICE_U_DISK);
    player->outputDevice(DFPLAYER_DEVICE_SD);
    //player->outputDevice(DFPLAYER_DEVICE_AUX);
    //player->outputDevice(DFPLAYER_DEVICE_SLEEP);
    //player->outputDevice(DFPLAYER_DEVICE_FLASH);
  
}
void Mp3voices::loop()
{
    ;
}

int Mp3voices::fileCountsInForder(int folder)
{
    return player->readFileCountsInFolder(folder);
}

int Mp3voices::getVolume()
{
    return player->readVolume();
}

void Mp3voices::setVolume(int volume)
{
    player->volume(volume);
}

void Mp3voices::volumeUp()
{
    player->volumeUp();
}
void Mp3voices::volumeDown()
{
    player->volumeDown();
}

void Mp3voices::playFolder(int folder, int file)
{
    player->playFolder(folder,file);
}


void Mp3voices::play(VOICE_FILE voice)
{
    Serial.print("folder :");
    Serial.print(voice.folder);
    Serial.print(" file : ");
    Serial.print(voice.file);
    Serial.println("");

    //player->playFolder(voice.folder,voice.file);
}


void Mp3voices::playUnitNumber(int number)
{
    if ((number <=20) || ((number <100) && (number%10 == 0)))
    {
        switch (number)
        {
            case 0 : 
                play(NUM_0);
                break ;
            case 1 : 
                play(NUM_1);
                break ;
            case 2 : 
                play(NUM_2);
                break ;                
            case 3 : 
                play(NUM_3);
                break ;                
            case 4 : 
                play(NUM_4);
                break ;                
            case 5 : 
                play(NUM_5);
                break ;                
            case 6 : 
                play(NUM_6);
                break ;                
            case 7 : 
                play(NUM_7);
                break ;                
            case 8 : 
                play(NUM_8);
                break ;                
            case 9 : 
                play(NUM_9);
                break ;                
            case 10 : 
                play(NUM_10);
                break ;                
            case 11 : 
                play(NUM_11);
                break ;                
            case 12 : 
                play(NUM_12);
                break ;                
            case 13 : 
                play(NUM_13);
                break ;                
            case 14 : 
                play(NUM_14);
                break ;                
            case 15 : 
                play(NUM_15);
                break ;                
            case 16 : 
                play(NUM_16);
                break ;                
            case 17 : 
                play(NUM_17);
                break ;                
            case 18 : 
                play(NUM_18);
                break ;                
            case 19 : 
                play(NUM_19);
                break ;                
            case 20 : 
                play(NUM_20);
                break ;                
            case 30 : 
                play(NUM_30);
                break ;                
            case 40 : 
                play(NUM_40);
                break ;                
            case 50 : 
                play(NUM_50);
                break ;                
            case 60 : 
                play(NUM_60);
                break ;                
            case 70 : 
                play(NUM_70);
                break ;                
            case 80 : 
                play(NUM_80);
                break ;                
            case 90 : 
                play(NUM_90);
                break ;                
        }
    }
}

void Mp3voices::playNumber(int number)
{
Serial.println("Number : "+String(number));    
    int rest = number;
    //millier
    int n = number / 1000 ;
    if (n >=1 )
    {
        if (n != 1)
        {
            playNumber(n);
        }
        play(NUM_1000);        
        rest = number - (n*1000);
    }

    //centaines 
    n = rest / 100 ;
    if (n >= 1)
    {
        if (n != 1)
        {
            playUnitNumber(n);
        }
        play(NUM_100);
        rest = rest - (n*100);
    }

    //dixaine 
    n = rest / 10 ;
    if (n >= 1)
    {
        playUnitNumber(n*10);
    }
    rest = rest - (n*10);
    playUnitNumber(rest);    
}