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
#ifndef Motor_h
#define Motor_h
#include "Arduino.h"
#include <analogWrite.h>

class Motor {
public:
	Motor();
	void setSpeed(double _target);
    void setP(double _Kp);
    void setI(double _Ki);
    void setPI(double _Kp, double _Ki);
    void setDriverPin(int _enablePin);
    void setDirectionPin(int _controlPin);
    void setIO(int _controlPin1, int _controlpin2, int _enablePin);
    void setup(int _controlPin1, int _controlPin2, int _enablePin, double _Kp, double _Ki);
    double getP();
    double getI();
    double* getPI();
    int getDriverPin();
    int getDirectionPin();
    int* getPin();
    int getSpeed();
    void Run();
    char* handleCMD(const char* Function, int* args);
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