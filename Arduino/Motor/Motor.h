#ifndef Motor_h
#define Motor_h
#include "Arduino.h"

class Motor {
public:
	Motor();
	void setPoint(double target);
    void setPI(double Kp, double Ki);
    void setIO(int _controlPin1, int _controlpin2, int _enablePin);
    void setup(int _controlPin1, int _controlPin2, int _enablePin, double _Kp, double _Ki);
    void Run();
private:
    //Motor Pins
    int controlPin1;
    int controlPin2;
    int enablePin;
    //Motor data
    int motorEnabled;    
    int motorDirection;  
    int motorSpeed;      
    //Regulator
    double Kp;
    double Ki;

    double Setpoint;
    double Input;
    double Output;
    double Error;
    double cumError;
};
#endif