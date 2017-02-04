#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include<Arduino.h>

enum  {
  LED_ON = 1,
  LED_OFF,
  STATUS,
  NEW_ZERO,
  TPOS,
  APOS,
  GET_TPOS,
  GET_APOS,
  GET_TPOS_SET,
  GET_APOS_SET,
  RESET,
  GET_VERSION,
  SYNC,
  KALIB
} enumCommands;

class ConnectionHandler {
  public:
    ConnectionHandler( unsigned int Baudrate, char StopChar );
    int CheckForCommand();
    int GetLastTransmittedMotorPosition();

    void SendResponse( String Response );

  protected:
    char _StopChar;
    int _OptionalMotorPosition;

    String _command_led_on;
    String _command_led_off;
    String _command_status;
    String _command_new_zero;
    String _command_trans_pos;
    String _command_azimu_pos;
    String _command_get_trans_pos;
    String _command_get_azimu_pos;
    String _command_get_trans_pos_setpoint;
    String _command_get_azimu_pos_setpoint;
    String _command_reset;
    String _command_get_version;
    String _command_sync_package;
    String _command_kalib;
};

#endif /* CONNECTION_HANDLER_H */
