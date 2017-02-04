#include "connection_handler.h"

ConnectionHandler::ConnectionHandler( unsigned int Baudrate, char StopChar ) :
  _StopChar(StopChar),
  _OptionalMotorPosition( 0 ),
  _command_led_on("LED=ON"),
  _command_led_off("LED=OFF"),
  _command_status("STATUS?"),
  _command_new_zero("NEW_ZERO"),
  _command_trans_pos("TPOS="),
  _command_azimu_pos("APOS="),
  _command_get_trans_pos("TPOS?"),
  _command_get_azimu_pos("APOS?"),
  _command_get_trans_pos_setpoint("TPOS_SET?"),
  _command_get_azimu_pos_setpoint("APOS_SET?"),
  _command_reset("RESET"),
  _command_get_version("VERSION?"),
  _command_sync_package("SYNC"),
  _command_kalib("CALIB")
{
  // Wait for serial port to open
  while (! Serial) {}
  // Set baudrate
  Serial.begin(Baudrate);
}

int ConnectionHandler::CheckForCommand() {
  // Purpose: Check if command sequence has been send
  // Return:  Returns value of type enumCommand or 0 if no command has been detected
  if (Serial.available() > 0) {
    String        InputString;

    InputString = Serial.readStringUntil(_StopChar);
    
    if (InputString.substring(0, 5) == _command_trans_pos) {
      // Command for trans motor position has been sent
      // bytes after the '=' indicate new position
      String NewPos_s = InputString.substring(5);
      int NewPos   = NewPos_s.toInt();
      _OptionalMotorPosition = NewPos;
      return TPOS;
    }
    else if (InputString.substring(0, 5) == _command_azimu_pos) {
      // Command for azimu motor position has been sent
      // bytes after the '=' indicate new position
      String NewPos_s = InputString.substring(5);
      int NewPos   = NewPos_s.toInt();
      _OptionalMotorPosition = NewPos;
      return APOS;
    }
    else if (InputString == _command_led_on) {
      return LED_ON;
    }
    else if (InputString == _command_led_off) {
      return LED_OFF;
    }
    else if (InputString == _command_status) {
      return STATUS;
    }
    else if (InputString == _command_new_zero) {
      return NEW_ZERO;
    }
    else if (InputString == _command_reset) {
      return RESET;
    }
    else if (InputString == _command_get_trans_pos) {
      return GET_TPOS;
    }
    else if (InputString == _command_get_azimu_pos) {
      return GET_APOS;
    }
    else if (InputString == _command_get_trans_pos_setpoint) {
      return GET_TPOS_SET;
    }
    else if (InputString == _command_get_azimu_pos_setpoint) {
      return GET_APOS_SET;
    }
    else if (InputString == _command_get_version){
      return GET_VERSION; 
    }
    else if (InputString == _command_sync_package){
      return SYNC;
    }
    else if (InputString == _command_kalib){
      return KALIB;
    }
    else return 0;
  }
  return 0;
}

int ConnectionHandler::GetLastTransmittedMotorPosition() {
  return _OptionalMotorPosition;
}

void ConnectionHandler::SendResponse( String Response ) {
  String s = Response + String(_StopChar);
  Serial.write(s.c_str());
}

