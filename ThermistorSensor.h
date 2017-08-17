#ifndef ThermistorSensor_h
#define ThermistorSensor_h

class ThermistorSensor
{
  public:
    ThermistorSensor(unsigned short pin, int seriesResistor, int nominalResistance, short nominalTemperature, int betaCoefficient);
    short getTemperatureInCelsius();
  private:
    unsigned short _pin;
    int _seriesResistor;
    int _nominalResistance;
    short _nominalTemperature;
    int _betaCoefficient;
};

#endif
