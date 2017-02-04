// MOtor Handler For Arduino (MOHFA)
#include "connection_handler.h"
#include "motor.h"

const int pin_busy_led =    10;
const int pin_stop_front =  13;
const int pin_stop_back =   11;

// The defined values are sent to the control programm 
// via this sync struct
struct ArduinoSyncPackage{
    struct strans{
      long min_um;
      long max_um;
      int steps;
    } trans;
    struct sazimu{
      long min_mdeg;
      long max_mdeg;
      int steps;
    } azimu;
  } * MyPackage;

enum State {
  NOT_OK,
  OK,
  BUSY
} MyState;

ConnectionHandler* MyConnection;
Motor* TransMotor;
Motor* AzimuMotor;

bool Kalib = false;
unsigned int Steps = 0;

// Motor pin reihenfolge
// gelb - braun - weiß - grün

void setup()
{
 
  unsigned int baudrate = 9600;
  MyConnection = new ConnectionHandler(baudrate, 13);
 
  // int DirPin,  int StepPin, int SleepPin, int ResetPin, int MS1Pin
  TransMotor = new Motor( TRANS_MAX_STEPS,
                          0, 
                          1, 
                          2,  
                          3,  
                          4, 
                          pin_stop_front,
                          pin_stop_back);
  AzimuMotor = new Motor( AZIMU_MAX_STEPS, 
                          5,  
                          6, 
                          7, 
                          8, 
                          9);

  MyPackage = new ArduinoSyncPackage();
  MyPackage->trans.min_um = TRANS_MIN_VALUE;
  MyPackage->trans.max_um = TRANS_MAX_VALUE;
  MyPackage->trans.steps = TRANS_MAX_STEPS;
  MyPackage->azimu.min_mdeg = AZIMU_MIN_VALUE;
  MyPackage->azimu.max_mdeg = AZIMU_MAX_VALUE;
  MyPackage->azimu.steps = AZIMU_MAX_STEPS;

  pinMode(pin_busy_led, OUTPUT);

  MyState = OK;
}

void loop() {

  // Check for incoming commands
  switch (MyConnection->CheckForCommand()) {
    case TPOS:
      {
        MyState = BUSY;
        digitalWrite(pin_busy_led, HIGH);
        TransMotor->NewPosition( MyConnection->GetLastTransmittedMotorPosition() );
        break;
      }

    case APOS:
      {
        MyState = BUSY;
        digitalWrite(pin_busy_led, HIGH);
        AzimuMotor->NewPosition( MyConnection->GetLastTransmittedMotorPosition() );
        break;
      }

    case LED_ON:
      //digitalWrite(ledPin, HIGH);
      break;

    case LED_OFF:
      //digitalWrite(ledPin, LOW);
      break;

    case STATUS : {
        int s = MyState;
        switch (s) {
          case NOT_OK :
            MyConnection->SendResponse("NOT_OK");
            break;
          case OK :
            MyConnection->SendResponse("OK");
            break;
          case BUSY :
            MyConnection->SendResponse("BUSY");
            break;
          default:
            break;
        }
        break;
      }

    case NEW_ZERO:
      TransMotor->TakeCurrentPositionAsNewZero();
      AzimuMotor->TakeCurrentPositionAsNewZero();
      break;

    case RESET:
      // ...
      break;
      
    case KALIB:
    { 
      Kalib = true;   
      TransMotor->NewPosition( TRANS_MAX_STEPS );
      break;
    }
        
    case GET_TPOS:
      MyConnection->SendResponse(String(TransMotor->GetPosition()));
      break;

    case GET_APOS:
      MyConnection->SendResponse(String(AzimuMotor->GetPosition()));
      break;

  case GET_TPOS_SET:
      MyConnection->SendResponse(String(TransMotor->GetSetPointPosition()));
      break;

    case GET_APOS_SET:
      MyConnection->SendResponse(String(AzimuMotor->GetSetPointPosition()));
      break;

    case GET_VERSION:
      // ...
      break;

    case SYNC:
      static int counter = 0;
      switch(counter % 6) {
      case 0:
        MyConnection->SendResponse(String(MyPackage->trans.min_um));
        counter++;
        break;
      case 1:
        MyConnection->SendResponse(String(MyPackage->trans.max_um));
        counter++;
        break;
      case 2:
        MyConnection->SendResponse(String(MyPackage->trans.steps));
        counter++;
        break;
      case 3:
        MyConnection->SendResponse(String(MyPackage->azimu.min_mdeg));
        counter++;
        break;
      case 4:
        MyConnection->SendResponse(String(MyPackage->azimu.max_mdeg));
        counter++;
        break;
      case 5:
        MyConnection->SendResponse(String(MyPackage->azimu.steps));
        counter++;
        break;
      default :
        break;
      }
      break;

    default:
      break;
  }

  if( Kalib ){
    if( digitalRead( pin_stop_front) == HIGH) {
      //MyConnection->SendResponse(String( Steps ));
      Steps= 0;
      Kalib = false;
    }
    Steps += 10;
  }

  // Move motors
  if (TransMotor->Busy()) {
    TransMotor->UpdateMotorPosition();
  }
  else if (AzimuMotor->Busy()) {
    AzimuMotor->UpdateMotorPosition();
  }

  if (!TransMotor->Busy() && !AzimuMotor->Busy() ) {
    MyState = OK;
    digitalWrite(pin_busy_led, LOW);
  }
}
