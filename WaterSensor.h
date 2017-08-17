#ifndef WaterSensor_h
#define WaterSensor_h

class WaterSensor
{
  public:
    WaterSensor(unsigned short pin);
    bool WaterReachedLevel();
  private:
    unsigned short _pin;
};

#endif
