#include "Arduino.h"
#include "WaterSensor.h"

WaterSensor::WaterSensor(unsigned short pin)
{
  pinMode(pin, INPUT);
  _pin = pin;
}

bool WaterSensor::WaterReachedLevel()
{
  return (digitalRead(_pin) == HIGH);
}

