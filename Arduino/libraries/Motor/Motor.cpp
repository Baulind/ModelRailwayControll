#include "Arduino.h"
#include <analogWrite.h>
#include "Motor.h"
#include <PubSubClient.h>

Motor::Motor() {
    
}

void Motor::setSpeed(int target){
    Setpoint = target;
}
void Motor::setDriverPin(int _enablePin){
    enablePin = _enablePin;
    pinMode(enablePin, OUTPUT);
}
void Motor::setDirectionPin(int _controlpin){
    controlPin = _controlpin;
    pinMode(controlPin, OUTPUT);
}
void Motor::setIO(int _controlPin, int _enablePin){
    controlPin = _controlPin;
    enablePin = _enablePin;

    pinMode(controlPin, OUTPUT);
    pinMode(enablePin, OUTPUT);
}
void Motor::setup(int _controlPin, int _enablePin){
    Motor::setIO(_controlPin,  _enablePin);  
    //Regulator
    Motor::setSpeed(0);

    // pull the enable pin LOW to start
    digitalWrite(enablePin, LOW);
}
int Motor::getDriverPin(){
    return enablePin;
}
int Motor::getDirectionPin(){
    return controlPin;
}
int Motor::getSpeed(){
    return Output;
}
int* Motor::getPin(){
    int pin[] = {getDriverPin(), getDirectionPin()};
    return pin;
}
void Motor::Run(){
    //If the motor doesnt have any assosiated pins then return
    if (controlPin + enablePin == 0)
    {
        return;
    }
    
    Input = Output;
    if(Setpoint == Input){
      //do nothing    
    }
    else if(Setpoint - Input <= 0 )
      Output--;
    else
      Output++;
    //------------------------------------
    //Motor
    //------------------------------------
    //drive output
    digitalWrite(controlPin, Output > 0);
    analogWrite(enablePin, abs(Output));

    //Serial.print(Output);
    //Serial.print('\t');
    //Serial.print(Setpoint);
    //Serial.print('\t');
    //Serial.print(Output > 0);
    //Serial.println('\t');
}

char* Motor::handleCMD(const char* Function, int* args){
  if (strcmp(Function, "setSpeed") == 0)
  {
    setSpeed(args[0]);
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
    setIO(args[0], args[1]);
  }
  else if (strcmp(Function, "setup") == 0)
  {
    setup(args[0], args[1]);
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