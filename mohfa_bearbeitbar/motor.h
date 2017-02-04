#ifndef MOTOR_H
#define MOTOR_H


#define TRANS_MIN_VALUE   -25000  // micrometer
#define TRANS_MAX_VALUE   25000   // micrometer
#define TRANS_MAX_STEPS   5060
#define AZIMU_MIN_VALUE   0       // millidregree
#define AZIMU_MAX_VALUE   180000  // millidregree
#define AZIMU_MAX_STEPS   1000

#include "Arduino.h"


class Motor {
  public:
    Motor( int MAX_STEPS, int DirPin,  int StepPin, int SleepPin, int ResetPin, int MS1Pin  );
    Motor( int MAX_STEPS, int DirPin,  int StepPin, int SleepPin, int ResetPin, int MS1Pin, int StopPinFront, int StopPinBack  );

    // Returns immediately if _set_point == _actual_value
    // Otherwise returns after motor has been driven one step towards _set_point
    void NewPosition( int New_Position );
    void UpdateMotorPosition();
    void TakeCurrentPositionAsNewZero();

    bool Busy();
    int GetPosition();
    int GetSetPointPosition();

  protected:
    const int _MAX_STEPS;

    int _set_point;
    int _actual_value;

    // pins
    const int _pin_dir;
    const int _pin_step;
    const int _pin_sleep;
    const int _pin_reset;
    const int _pin_ms1;
    const int _pin_stop_front;
    const int _pin_stop_back;

    int _delay_microseconds;
    int _microsteps;
    
};



#endif /* MOTOR_H */
