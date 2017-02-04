#include "motor.h"
Motor::Motor( int MAX_STEPS, int DirPin,  int StepPin, int SleepPin, int ResetPin, int MS1Pin ) :
  _MAX_STEPS(MAX_STEPS),
  _set_point(_MAX_STEPS / 2),
  _actual_value(_MAX_STEPS / 2),
  _pin_dir(DirPin),
  _pin_step(StepPin),
  _pin_sleep(SleepPin),
  _pin_reset(ResetPin),
  _pin_ms1(MS1Pin),
  _pin_stop_front( -1 ),
  _pin_stop_back( -1)
{

  // Azimu Motor
  _delay_microseconds = 3000;
  _microsteps = 1;
  
  pinMode(_pin_dir, OUTPUT);
  pinMode(_pin_step, OUTPUT);
  pinMode(_pin_sleep, OUTPUT);
  pinMode(_pin_reset, OUTPUT);
  pinMode(_pin_ms1, OUTPUT);
  
  digitalWrite(_pin_ms1, LOW);
  digitalWrite(_pin_sleep, HIGH);
  digitalWrite(_pin_reset, HIGH);
}

Motor::Motor( int MAX_STEPS, int DirPin,  int StepPin, int SleepPin, int ResetPin, int MS1Pin, int StopPinFront, int StopPinBack ) :
  _MAX_STEPS(MAX_STEPS),
  _set_point(_MAX_STEPS / 2),
  _actual_value(_MAX_STEPS / 2),
  _pin_dir(DirPin),
  _pin_step(StepPin),
  _pin_sleep(SleepPin),
  _pin_reset(ResetPin),
  _pin_ms1(MS1Pin),
  _pin_stop_front( StopPinFront ),
  _pin_stop_back( StopPinBack )
{

  // Trans Motor
  _delay_microseconds = 2000;
  _microsteps = 10;
  
  pinMode(_pin_dir, OUTPUT);
  pinMode(_pin_step, OUTPUT);
  pinMode(_pin_sleep, OUTPUT);
  pinMode(_pin_reset, OUTPUT);
  pinMode(_pin_ms1, OUTPUT);

  pinMode(_pin_stop_front, INPUT);
  pinMode(_pin_stop_back, INPUT);

  digitalWrite(_pin_ms1, LOW);
  digitalWrite(_pin_sleep, HIGH);
  digitalWrite(_pin_reset, HIGH);

}

void Motor::NewPosition( int New_Position ) {
  if ( New_Position < 0 || New_Position > _MAX_STEPS)
    return;
  else
    _set_point = New_Position;
}

void Motor::UpdateMotorPosition(  ) {
  if ( !Busy()  ) return;
  else {
    // Do some motor magic ( drive only one step in the right direction! ) e.g.
    if ( _actual_value < _set_point ) {
      digitalWrite(_pin_dir, LOW);
      for (int i = 0; i < _microsteps; i++){
        if( _pin_stop_front != -1 && digitalRead(_pin_stop_front) == HIGH){
          _actual_value = TRANS_MAX_STEPS;
          break;
        }
        digitalWrite(_pin_step, HIGH);
        delayMicroseconds(_delay_microseconds);
        digitalWrite(_pin_step, LOW);
        if( i == _microsteps -1 )  _actual_value++;
      }
    }
    else {
      digitalWrite(_pin_dir, HIGH);
      for (int i = 0; i < _microsteps; i++){
        if( _pin_stop_back != -1 && digitalRead(_pin_stop_back) == HIGH){
          _actual_value = 0;
          break;
        }
        digitalWrite(_pin_step, HIGH);
        delayMicroseconds(_delay_microseconds);
        digitalWrite(_pin_step, LOW);
        if( i == _microsteps - 1)  _actual_value--;
      }    
    }
  }
}

// Warning: Calling this method will abort all previous position commands
void Motor::TakeCurrentPositionAsNewZero() {
  this->_actual_value = _MAX_STEPS / 2;
  this->_set_point = _MAX_STEPS / 2;
}

bool Motor::Busy() {
  return !( _set_point == _actual_value);
}

int Motor::GetPosition() {
  return _actual_value;
}

int Motor::GetSetPointPosition(){
  return _set_point;
}

