
#ifndef Mp3voices_h
#define Mp3voices_h

//#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

struct VOICE_FILE {
  int folder;
  int file;
  int time;
};

#define VOLUM_MAX 30


class Mp3voices
{
    private : 
        DFRobotDFPlayerMini *player;
        SoftwareSerial *softwareSerial;

    public :
        Mp3voices();
        Mp3voices(int rxPin, int txPin);
        void setup();
        void loop();
        void play(VOICE_FILE voice);
        void playNumber(int number);
        void playUnitNumber(int number);
        void playFolder(int folder, int file);
        void volumeUp();
        void volumeDown();
        int getVolume();
        void setVolume(int volume);
        int fileCountsInForder(int folder);


        VOICE_FILE NUM_0 = {1,10,0};
        VOICE_FILE NUM_1 = {1,1,0};
        VOICE_FILE NUM_2 = {1,2,0};
        VOICE_FILE NUM_3 = {1,3,0};
        VOICE_FILE NUM_4 = {1,4,0};
        VOICE_FILE NUM_5 = {1,5,0};
        VOICE_FILE NUM_6 = {1,6,0};
        VOICE_FILE NUM_7 = {1,7,0};
        VOICE_FILE NUM_8 = {1,8,0};
        VOICE_FILE NUM_9 = {1,9,0};
        VOICE_FILE NUM_10 = {2,1,0};
        VOICE_FILE NUM_11 = {2,2,0};
        VOICE_FILE NUM_12 = {2,3,0};
        VOICE_FILE NUM_13 = {2,4,0};
        VOICE_FILE NUM_14 = {2,5,0};
        VOICE_FILE NUM_15 = {2,6,0};
        VOICE_FILE NUM_16 = {2,7,0};
        VOICE_FILE NUM_17 = {2,8,0};
        VOICE_FILE NUM_18 = {2,9,0};
        VOICE_FILE NUM_19 = {2,10,0};
        VOICE_FILE NUM_20 = {2,11,0};
        VOICE_FILE NUM_30 = {2,12,0};
        VOICE_FILE NUM_40 = {2,13,0};
        VOICE_FILE NUM_50 = {2,14,0};
        VOICE_FILE NUM_60 = {2,15,0};
        VOICE_FILE NUM_70 = {2,16,0};
        VOICE_FILE NUM_80 = {2,17,0};
        VOICE_FILE NUM_90 = {2,18,0};
        VOICE_FILE NUM_100 = {2,19,0};
        VOICE_FILE NUM_1000 = {2,20,0};
        VOICE_FILE TXT_EST_DE = {3,1,0};
        VOICE_FILE TXT_ET = {3,2,0};
        VOICE_FILE TXT_EXTERIEUR = {3,3,0};
        VOICE_FILE TXT_HEURE = {3,4,0};
        VOICE_FILE TXT_IL_EST = {3,5,0};
        VOICE_FILE TXT_INTERIEUR = {3,6,0};
        VOICE_FILE TXT_LA_TEMPERATURE = {3,7,0};
        VOICE_FILE TXT_MINUTES = {3,8,0};
        VOICE_FILE TXT_DEGRE = {3,9,0};
        VOICE_FILE TXT_HECTOPASCAL = {3,10,0};
        VOICE_FILE TXT_LE_VOLUME_EST_A = {3,11,0};
        VOICE_FILE TXT_L_ALARME_EST = {3,12,0};
        VOICE_FILE TXT_REGLE_A = {3,13,0};
        VOICE_FILE TXT_DESACTIVE = {3,14,0};
        VOICE_FILE TXT_ACTIVE = {3,15,0};
        VOICE_FILE TXT_REGLAGE_DES = {3,16,0};
        VOICE_FILE TXT_MUSIQUES = {3,17,0};
        VOICE_FILE TXT_AVEC_LA = {3,18,0};
        VOICE_FILE TXT_SECONDS = {3,19,0};
};



#endif