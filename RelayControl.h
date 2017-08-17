#ifndef RelayControl_h
#define RelayControl_h

class RelayControl
{
  public:
    RelayControl(unsigned short pin, bool normallyClosed);
    void off();
    void on();
    void flick();
    bool isCircuitClosed();
  private:
    unsigned short _pin;
    bool _flow;
    bool _normallyClosed;
};

#endif
