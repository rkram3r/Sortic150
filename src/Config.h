#ifndef config_h
#define config_h

#define DISTANCE_SENSOR A1
#define RFIDDETECTOR_SDA 7
#define RFIDDETECTOR_RST_PIN 6
#define CHASSIS_DIGITAL_TRIG_PIN 4
#define CHASSIS_DIGITAL_ECHO_PIN 5
#define MOTOR_NR 4
#define CHASIS_POS_MAX 45
#define CHASIS_POS_MIN 15
#define NofActions 3
#define NofSensors 2
#define ImportantSensorsForAction boolean[NofActions][NofSensors]
#define RFID_LENGTH 7

#endif
