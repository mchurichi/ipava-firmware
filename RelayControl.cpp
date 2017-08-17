#include "Arduino.h"
#include "RelayControl.h"

RelayControl::RelayControl(unsigned short pin, bool normallyClosed)
{
  pinMode(pin, OUTPUT);
  digitalWrite(_pin, LOW);
  
  _pin = pin;
  _flow = normallyClosed;
  _normallyClosed = normallyClosed;
}

void RelayControl::off()
/* 
  Opens the relay. No matter what type it is, the function always stops the 
  current flow.
*/
{ 
  if (_normallyClosed)
  {
    digitalWrite(_pin, HIGH);
    _flow = false; //ON
  }
  else
  {
    digitalWrite(_pin, LOW);
    _flow = false; //ON 
  }
}

void RelayControl::on()
/* 
  Closes the relay. No matter what type it is, the function always starts the 
  current flow.
*/
{
  if (_normallyClosed)
  {
    digitalWrite(_pin, LOW);
    _flow = true; //ON
  }
  else
  {
    digitalWrite(_pin, HIGH);
    _flow = true; //ON  
  }
}

void RelayControl::flick()
/* 
  Turns ON if it is OFF and viceversa
*/
{
  if (_flow)
  {
    off(); 
  }
  else
  {
    on();
  }
}

bool RelayControl::isCircuitClosed()
/* 
  Returns True if current is flowing.
*/
{
  return _flow;
}
