#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

MAX30105 particleSensor;

#define MAX_BRIGHTNESS 255

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
uint16_t irBuffer[100]; 
uint16_t redBuffer[100];
#else
uint32_t irBuffer[100]; 
uint32_t redBuffer[100];
#endif
int oxsatthreshold=85; //Notify if SpO2<85%
int hrthreshold=60; //Notify if Heart Rate<60 BPM
int ecglead1=3;
int ecglead2=4;
int ecgin=1;
int blankrun=0;

int32_t bufferLength; 
int32_t spo2; 
int8_t validSPO2;
int32_t heartRate;
int8_t validHeartRate;

byte pulseLED = 11;
byte readLED = 13; 

void setup()
{
  Serial.begin(9600); 
  pinMode(ecglead1,INPUT);
  pinMode(ecglead2,INPUT);
  pinMode(pulseLED, OUTPUT);
  pinMode(readLED, OUTPUT);

  
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port
  {
      //ERROR
  }

    //Callibration
  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
}

void loop()
{
  //Make 1000 blank runs to remove inconsistent data and wait for sensors to settle
  if(blankrun<1000)
  {
    chk(0);
    blankrun++;
  }
  else
  {
    chk(1);
  }
}

void chk(int alert)
{
  ecg(alert);
  oxsat(alert);
}


void oxsat(int alert)
{
  bufferLength = 100; 

  
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false)
      particleSensor.check(); 

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample();
  }

 
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

  


  
    for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

  
    for (byte i = 75; i < 100; i++)
    {
      while (particleSensor.available() == false) 
        particleSensor.check(); 

      digitalWrite(readLED, !digitalRead(readLED));

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); 
      int hr=heartRate;
      int oxsaturation=spo2;
      if(alert&&validSPO2)
      {
        if(oxsaturation<oxsatthreshold)
        {
          notify(1);//Low SpO2
        }
      }
      if(alert&&validHeartRate)
      {
        if(hr<hrthreshold)
        {
          notify(2); //Low Heart Rate
        }
      }
    }
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  
}

void ecg(int alert)
{
  int dump[30];
  for(int i=0;i<30;i++)
  {
    if(digitalRead(ecglead1)||digitalRead(ecglead2))
    {
      i--;
      continue;
    }
    dump[i]=analogRead(ecgin);
  }
  int count=0;
  for(int i=1;i<29;i++)
  {
    if(dump[i]<dump[i-1]&&dump[i]<dump[i+1])
    {
      count++;
    }
  }
  if(count>2)
  {
    if(alert)
    {
      notify(4); //Fragmented QRS segment
    }
  }
  else
  {
    int l1=0;
    int l2=0;
    int chk=0;
    for(int i=1;i<29;i++)
    {
      if(dump[i]<dump[i-1]&&dump[i]<dump[i+1])
      {
        if(chk==0)
        {
          l1=dump[i]; 
          chk++;
        }
        else
        {
          l2=dump[i];
          break;
        }
      }
    }
    if(l1<l2)
    {
      if(alert)
      {
        notify(3); //Anterior Wall ST Segment Elevation
      }
    }
  }
}

void notify(int stat)
{
  /*
   * 1 for Low SpO2
   * 2 for low heart rate
   * 3 for MI
   * 4 for blockage
   */
   Serial.println(stat);
}
