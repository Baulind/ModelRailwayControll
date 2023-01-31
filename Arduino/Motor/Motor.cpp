/*
 * Filename: ...
 * File description: ...
 *
 * Authors:
 * Benjamin Lindbæck
 * ...
 * ...
 * 
 * Revised by: Emil Fagerli Osvoll
 * Revision date: 31.01.2023
 * 
 * Project: Bachelor thesis
 * Project name: Model railway Control via Colaborating Processing Units
 * Project recipient: Fagskulen Vestland
 * University: Western Norway University of Applied Sciences
 * Course: Automation with Robotics
 */

#include "Arduino.h"
#include "Motor.h"

Motor::Motor() {
    
}

void Motor::setup(int _controlPin1, int _controlPin2, int _enablePin, double _Kp, double _Ki) {
    // Initialize the inputs and outputs
    // Motor Pins
    setIO(_controlPin1, _controlPin2, _enablePin);

    // Regulator
    Motor::setPI(_Kp, _Ki);
    Motor::setPoint(0);

    // Pull the enable pin LOW to start
    digitalWrite(enablePin, LOW);
}

void Motor::setPoint(double target) {
    Setpoint = target;
}

void Motor::setPI(double _Kp, double _Ki) {
    Kp = _Kp;
    Ki = _Ki;
}

void Motor::setIO(int _controlPin1, int _controlPin2, int _enablePin) {
    controlPin1 = _controlPin1;
    controlPin2 = _controlPin2;
    enablePin = _enablePin;

    pinMode(controlPin1, OUTPUT);
    pinMode(controlPin2, OUTPUT);
    pinMode(enablePin, OUTPUT);
}

void Motor::Run(){
    // If the motor doesnt have any assosiated pins then return
    if (controlPin1 == 0 || controlPin2 == 0 || enablePin == 0) {
        return;
    }
    
    // PI Controller
    Input = motorSpeed;
    Error = Setpoint - Input;
    cumError += Error;
    Output = Error * Kp + cumError * Ki;

    //------------------------------------
    // Motor
    //------------------------------------
    if (Output == 0.0) {
        motorEnabled = 0;
    }

    if (Output > 0) {
        motorEnabled = 1;
        motorDirection = 1;
    } else if (Output < 0) {
        motorEnabled = 1;
        motorDirection = 0;
    }

    motorSpeed = Output;

    /* Change the direction the motor spins by talking to the control pins
     * on the H-Bridge
     */
    if (motorDirection == 1) {
        digitalWrite(controlPin1, HIGH);
        digitalWrite(controlPin2, LOW);
    } else {
        digitalWrite(controlPin1, LOW);
        digitalWrite(controlPin2, HIGH);
    }

    // If the motor is supposed to be on
    if (motorEnabled == 1) {
        // PWM the enable pin to vary the speed
        analogWrite(enablePin, abs(motorSpeed));
    } else {  /* If the motor is not supposed to be on
               * turn the motor off
               */
        analogWrite(enablePin, 0);
    }

    /*
    Serial.print(Output);
    Serial.print('\t');
    // Plot the desired output
    //Serial.print("Setpoint");
    Serial.print(Setpoint);
    Serial.print('\t');
    // Plot the error
    //Serial.print("Error");
    Serial.print(Error);
    Serial.println('\t');
    */
}