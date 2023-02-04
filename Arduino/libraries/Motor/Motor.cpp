/*
 * Filename: ...
 * File description: ...
 *
 * Authors:
 * Benjamin Lindbæck
 * ...
 * ...
 * 
 * Revised by:
 * 
 * Project: Bachelor thesis
 * Project name: Model railway Control via Colaborating Processing Units
 * Project recipient: Fagskulen Vestland
 * University: Western Norway University of Applied Sciences
 * Course: Automation with Robotics
 */
#include "Arduino.h"
#include <analogWrite.h>
#include "Motor.h"
#include <PubSubClient.h>

Motor::Motor() {
    
}

void Motor::setSpeed(double target){
    Setpoint = target;
}
void Motor::setP(double _Kp){
    Kp = _Kp;
}
void Motor::setI(double _Ki){
    Ki = _Ki;
}
void Motor::setPI(double _Kp, double _Ki){
    Motor::setP(_Kp);
    Motor::setI(_Ki);
}
void Motor::setDriverPin(int _enablePin){
    enablePin = _enablePin;
    pinMode(enablePin, OUTPUT);
}
void Motor::setDirectionPin(int _controlpin){
    controlPin1 = _controlpin;
    pinMode(controlPin1, OUTPUT);
}
void Motor::setIO(int _controlPin1, int _controlPin2, int _enablePin){
    controlPin1 = _controlPin1;
    controlPin2 = _controlPin2;
    enablePin = _enablePin;

    pinMode(controlPin1, OUTPUT);
    pinMode(controlPin2, OUTPUT);
    pinMode(enablePin, OUTPUT);
}
void Motor::setup(int _controlPin1, int _controlPin2, int _enablePin, double _Kp, double _Ki){
    Motor::setIO(_controlPin1, _controlPin2, _enablePin);  
    //Regulator
    Motor::setPI(_Kp, _Ki);
    Motor::setSpeed(0);

    // pull the enable pin LOW to start
    digitalWrite(enablePin, LOW);
}
double Motor::getP(){
    return Kp;
}
double Motor::getI(){
    return Ki;
}
double* Motor::getPI(){
    double pi[] = {getP(), getI()};
    return pi;
}
int Motor::getDriverPin(){
    return enablePin;
}
int Motor::getDirectionPin(){
    return controlPin1;
}
int Motor::getSpeed(){
    return motorSpeed;
}
int* Motor::getPin(){
    int pin[] = {getDriverPin(), getDirectionPin()};
    return pin;
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
    //Serial.print(Setpoint);
    //Serial.print('\t');
    //Serial.print(Error);
    //Serial.println('\t');
}

char* Motor::handleCMD(const char* Function, int* args){
  if (strcmp(Function, "setSpeed") == 0)
  {
    setSpeed(args[0]);
  }
  else if (strcmp(Function, "setP") == 0)
  {
    setP(args[0]);
  }
  else if (strcmp(Function, "setI") == 0)
  {
    setI(args[0]);
  }
  else if (strcmp(Function, "setPI") == 0)
  {
    setPI(args[0], args[1]);
  }
  else if (strcmp(Function, "setDriverPin") == 0)
  {
    setDriverPin(args[0]);
  }
  else if (strcmp(Function, "setDirectionPin") == 0)
  {
    setDirectionPin(args[0]);
  }
  else if (strcmp(Function, "setIO") == 0)
  {
    setIO(args[0], args[1], args[1]);
  }
  else if (strcmp(Function, "setup") == 0)
  {
    setup(args[0], args[1], args[2], args[3], args[4]);
  }
  else if (strcmp(Function, "getP") == 0)
  {
    //return std::format("{\"value\"=\"{%s}\"}", getP()).c_str();
  }
  else if (strcmp(Function, "getI") == 0)
  {
    //getI();
  }
  else if (strcmp(Function, "getPI") == 0)
  {
    //getPI();
  }
  else if (strcmp(Function, "getDriverPin") == 0)
  {
    //getDriverPin();
  }
  else if (strcmp(Function, "getDirectionPin") == 0)
  {
    //getDirectionPin();
  }
  else if (strcmp(Function, "getPin") == 0)
  {
    //getPin();
  }
  else if (strcmp(Function, "getSpeed") == 0)
  {
    //getSpeed();
  }
  return "-1";
}