#include "Arduino.h"
#include "ThermistorSensor.h"

ThermistorSensor::ThermistorSensor(unsigned short pin, int seriesResistor, int nominalResistance, short nominalTemperature, int betaCoefficient)
{
  _pin = pin;
  _seriesResistor = seriesResistor;
  _nominalResistance = nominalResistance;
  _nominalTemperature = nominalTemperature;
  _betaCoefficient = betaCoefficient;
}

short ThermistorSensor::getTemperatureInCelsius()
{
  float ADCvalue;
  float Resistance;
  ADCvalue = analogRead(_pin);

  //convert value to resistance
  Resistance = (1023 / ADCvalue) - 1;
  Resistance = _seriesResistor / Resistance;

  //start steinhart equation
  float steinhart;
  steinhart = Resistance / _nominalResistance; // (R/Ro)
  steinhart = log(steinhart); // ln(R/Ro)
  steinhart /= _betaCoefficient; // 1/B * ln(R/Ro)
  steinhart += 1.0 / (_nominalTemperature + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart; // Invert
  steinhart -= 273.15; // convert to C

  return steinhart;
}
