#include <Arduino.h>
#include <NewPing.h>
#include <MFRC522.h>

#include <Print.h>
#include <Action.h>
#include <Sensor.h>
#include <Config.h>
#include <Distance.h>
#include <Motor.h>
#include <Sleep.h>
#include <RfidDetector.h>

static Adafruit_MotorShield currentMotorShield{};
static Adafruit_DCMotor *driverMotor = currentMotorShield.getMotor(MOTOR_NR);
static MFRC522 partDetector{RFIDDETECTOR_SDA, RFIDDETECTOR_RST_PIN};
static NewPing *newPing = new NewPing{CHASSIS_DIGITAL_TRIG_PIN, CHASSIS_DIGITAL_ECHO_PIN, CHASIS_POS_MAX};

static Sensor* sensors [NofSensors]{ 
    new RfidDetector{&partDetector},  
    new Distance{newPing}
    };
static Action* actions [NofActions]{ 
    new Motor{driverMotor, FORWARD},  
    new Motor{driverMotor, BACKWARD}, 
    new Sleep{}
    };

void setup(){
    Serial.begin(9600);
    currentMotorShield.begin();
    SPI.begin();
    partDetector.PCD_Init();
}

int parseAction(String actionName, String action){
    return action.substring(actionName.length()+1, action.length()).toInt();
}

void loop(){
    if(Serial.available()){
        String actionMarker = Serial.readStringUntil(')');
        String actionName = actionMarker.substring(0, actionMarker.indexOf('('));
        Action* action = actions[actionName.toInt()];
        action->start(parseAction(actionName, actionMarker));
    }
    Serial << '[';
    for(auto index = 0; index < NofSensors; index ++){
        sensors[index]->get(Serial) << (index == NofSensors-1 ? ']': ',');
    }
}
