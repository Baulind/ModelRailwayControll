#include <stdio.h>
#include <string.h>

const unsigned int MAX_MESSAGE_LENGTH = 32;
const int controlPin1 = 2;                // connected to pin 7 on the H-bridge
const int controlPin2 = 3;                // connected to pin 2 on the H-bridge
const int enablePin = 9;                  // connected to pin 1 on the H-bridge

//Motor
int state = 0;

int motorEnabled = 0;    // Turns the motor on/off
int motorDirection = 0;  // current direction of the motor
int motorSpeed = 0;      // speed of the motor

//Regulator
double Setpoint, Input, Output, Error, cumError;
double Kp = 0.1;
double Ki = 0.05;

void setup() {
  //Opeen serial communications
  Serial.begin(9600);
  // initialize the inputs and outputs
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // pull the enable pin LOW to start
  digitalWrite(enablePin, LOW);

  //Initialize
  Setpoint = 0;
  Error = 0;
  Input = 0;
  Output = 0;
}

void loop() {
  //------------------------------------
  //Input
  //------------------------------------
  //Check to see if anything is available in the serial receive buffer
  while (Serial.available() > 0)
  {
    //Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();
      //Message coming in (check not terminating character) and guard for over message size
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      //Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    }
    //Full message received...
    else
    {
      //Add null character to string
      message[message_pos] = '\0';

      //Reset for the next message
      message_pos = 0;

      char* pch;
      unsigned int i = 0;
      pch = strtok(message, "#");
      while (pch != NULL)
      {
        state = atoi(pch);
        pch = strtok (NULL, "#");
        i++;
        Setpoint = bound(state, -255, 255);
      }
    }
  }
  //------------------------------------
  //PI Regulator
  //------------------------------------
  Input = motorSpeed;
  Error = Setpoint - Input;
  cumError += Error;
  Output = Error * Kp + cumError * Ki;
  //------------------------------------
  //Motor
  //------------------------------------
  if(Output == 0){
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
  //------------------------------------
  //Serial
  //------------------------------------
  //Serial.print("Input");
  Serial.print(Input);
  Serial.print('\t');
  // plot the desired output
  //Serial.print("Setpoint");
  Serial.print(Setpoint);
  Serial.print('\t');
  // plot the error
  //Serial.print("Error");
  Serial.print(Error);
  Serial.println('\t');
}

// Bound the input value between x_min and x_max.
float bound(float x, float x_min, float x_max) {
    if (x < x_min) { x = x_min; }
    if (x > x_max) { x = x_max; }
    return x;
}