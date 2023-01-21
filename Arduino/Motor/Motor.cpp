#include "Arduino.h"
#include "Motor.h"
Motor::Motor() {
    
}
void Motor::setup(int _controlPin1, int _controlPin2, int _enablePin, double _Kp, double _Ki){
    // initialize the inputs and outputs
    //Motor Pins
    setIO(_controlPin1, _controlPin2, _enablePin);  
    //Regulator
    Motor::setPI(_Kp, _Ki);
    Motor::setPoint(0);


    // pull the enable pin LOW to start
    digitalWrite(enablePin, LOW);
}
void Motor::setPoint(double target){
    Setpoint = target;
}
void Motor::setPI(double _Kp, double _Ki){
    Kp = _Kp;
    Ki = _Ki;
}
void Motor::setIO(int _controlPin1, int _controlPin2, int _enablePin){
    controlPin1 = _controlPin1;
    controlPin2 = _controlPin2;
    enablePin = _enablePin;

    pinMode(controlPin1, OUTPUT);
    pinMode(controlPin2, OUTPUT);
    pinMode(enablePin, OUTPUT);
}
void Motor::Run(){
    //If the motor doesnt have any assosiated pins then return
    if (controlPin1 == 0 || controlPin2 == 0 || enablePin == 0)
    {
        return;
    }
    
    //PI Controller
    Input = motorSpeed;
    Error = Setpoint - Input;
    cumError += Error;
    Output = Error * Kp + cumError * Ki;
    //------------------------------------
    //Motor
    //------------------------------------
    if(Output == 0.0){
        motorEnabled = 0;
    }
    if(Output > 0){
        motorEnabled = 1;
        motorDirection = 1;
    }
    else if(Output < 0){
        motorEnabled = 1;
        motorDirection = 0;
    }
    motorSpeed = Output;

    // change the direction the motor spins by talking to the control pins
    // on the H-Bridge
    if (motorDirection == 1) {
        digitalWrite(controlPin1, HIGH);
        digitalWrite(controlPin2, LOW);
    } else {
        digitalWrite(controlPin1, LOW);
        digitalWrite(controlPin2, HIGH);
    }

    // if the motor is supposed to be on
    if (motorEnabled == 1) {
        // PWM the enable pin to vary the speed
        analogWrite(enablePin, abs(motorSpeed));
    } 
    else {  // if the motor is not supposed to be on
        //turn the motor off
        analogWrite(enablePin, 0);
    }
    //Serial.print(Output);
    //Serial.print('\t');
    //// plot the desired output
    ////Serial.print("Setpoint");
    //Serial.print(Setpoint);
    //Serial.print('\t');
    //// plot the error
    ////Serial.print("Error");
    //Serial.print(Error);
    //Serial.println('\t');
}