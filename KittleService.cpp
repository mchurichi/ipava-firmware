#include <WString.h>
#include "RelayControl.h"
#include "ThermistorSensor.h"
#include "WaterSensor.h"
#include "KittleService.h"
#include "Arduino.h"

//needed for singleton
KittleService* KittleService::instance = 0;

KittleService* KittleService::getInstance()
{
  if (!instance)
    instance = new KittleService();

  return instance;
}

KittleService::KittleService()
{
  waterValve = new RelayControl(4, false);
  waterHeater = new RelayControl(5, false);
  tempSensor = new ThermistorSensor(0, 10000, 10000, 25, 3950);
  waterLevelSensor = new WaterSensor(13);
  _processingARequest = false;
  _currentStep = 0;
  Serial.begin(9600);
}

bool KittleService::StartClientRequest(short desiredTemp)
{
  if (_processingARequest)
    return false; //Means kittle is already processing a request or Kittle hasn't been initialized through InitializeKittle
  else {
    waterValve->off();
    waterHeater->off();
    _processingARequest = true;
    _desiredTemp = desiredTemp;
    return true;
  }
}

String KittleService::UpdateClientRequest()
{ 
  //This method won't do anything unless a request has been started
  if(_processingARequest)
  {
    LogKittleStatus();
    switch(_currentStep)
    {
      case 0:
        EnsureEnoughWater();
        return "filling";
        break;
      case 1:
        HeatWater();
        return "heating";
        break;
      case 2:
        EndClientRequest();
        return "finished";
        break;
    }

  }
  return ""; //Shouldn't never reach this
}

void KittleService::EnsureEnoughWater()
{
  if(waterLevelSensor->WaterReachedLevel())
  {
    waterValve->off();
    _currentStep++; //go to next step    
  }
  else
    waterValve->on();
}

void KittleService::HeatWater()
{
  //not only we have to stop heating if the temp is major than desired, also if we don't have enough water (water leak, software bug, etc) beacuse it could be dangerous.
  if(!waterLevelSensor->WaterReachedLevel() || tempSensor->getTemperatureInCelsius() >= _desiredTemp)
  {
      waterHeater->off();
      _currentStep++;
  }
  else
    waterHeater->on();
}

void KittleService::EndClientRequest()
{
  _currentStep = 0;
  _processingARequest = false;  
}


void KittleService::LogKittleStatus()
{
  Serial.println("--------------------------");
  Serial.print("Temperatura del agua: ");
  Serial.print(tempSensor->getTemperatureInCelsius());
  Serial.print("\n");
  Serial.print("La pava tiene agua: ");
  Serial.print(waterLevelSensor->WaterReachedLevel());
  Serial.print("\n");
  Serial.print("Pava llenandose: ");
  Serial.print(waterValve->isCircuitClosed());
  Serial.print("\n");
  Serial.print("Agua Calentandose: ");
  Serial.print(waterHeater->isCircuitClosed());
  Serial.print("\n");
}


