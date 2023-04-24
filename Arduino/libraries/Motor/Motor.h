#ifndef Motor_h
#define Motor_h
#include "Arduino.h"
#include <analogWrite.h>

class Motor {
public:
	Motor();
	void setSpeed(int _target);
    void setDriverPin(int _enablePin);
    void setDirectionPin(int _controlPin);
    void setIO(int _controlPin,  int _enablePin);
    void setup(int _controlPin, int _enablePin);
    int getDriverPin();
    int getDirectionPin();
    int* getPin();
    int getSpeed();
    void Run();
    char* handleCMD(const char* Function, int* args);
private:
    //Motor Pins
    int controlPin;
    int enablePin;
    //Motor data
    int Setpoint;
    int Input;
    int Output;
};
#endif