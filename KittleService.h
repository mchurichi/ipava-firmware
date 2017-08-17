#include <WString.h>
#include "RelayControl.h"
#include "ThermistorSensor.h"
#include "WaterSensor.h"

#ifndef KittleService_h
#define KittleService_h

class KittleService
{
  public:
    static KittleService* getInstance();
    void InitializeKittle();
    bool StartClientRequest(short desiredTemp);
    String UpdateClientRequest();
  private:
    //singleton instance
    static KittleService* instance; 
    //hardware interfacing stuff
    RelayControl* waterValve;
    RelayControl* waterHeater;
    ThermistorSensor* tempSensor;
    WaterSensor* waterLevelSensor;
    //status stuff
    bool _processingARequest;
    short _currentStep;
    short _desiredTemp;
    //methods
    KittleService(); //private constructor because of singleton
    void EnsureEnoughWater();
    void HeatWater();
    void EndClientRequest();
    void LogKittleStatus();
};

#endif
