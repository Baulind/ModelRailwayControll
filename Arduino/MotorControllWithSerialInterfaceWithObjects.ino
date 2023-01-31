/*
 * Filename:			...
 * File description:	...
 *
 * Authors:				* Benjamin Lindbæck
 * 						* ...
 * 						* ...
 * Revised by:			Emil Fagerli Osvoll
 * Revision date:		31.01.2023
 * 
 * Project:				Bachelor thesis
 * Project name:		Model railway Control via Colaborating Processing Units
 * Project recipient:	Fagskulen Vestland
 * University:			Western Norway University of Applied Sciences
 * Course:				Automation with Robotics
 */

#include <stdio.h>
#include <string.h>
#include <Motor.h>

#define RAIL_LIMIT 12
#define SWITCH_LIMIT 12
#define MESSAGE_LIMIT 5

String data[MESSAGE_LIMIT];

Motor railSystem[RAIL_LIMIT];

void setup() {
	// Open serial communications
	Serial.begin(9600);
	railSystem[0].setup(2, 3, 9, 0.1, 0.05);

	// Initialize the inputs and outputs
}

void loop() {
	if (read()) {
		bool Get = data[0] == "GET";
		bool Set = data[0] == "SET";
		bool Motor = data[1] == "MOTOR";
		bool Switch = data[1] == "SWITCH";
		int Id = atoi(data[2].c_str());
		int Value = atoi(data[3].c_str());

		if (Get && Motor) {

		}
		else if (Get && Switch) {

		}
		else if (Set && Motor) {
			railSystem[Id].setPoint(Value);
		}
		else if (Set && Switch) {
			
		}
	}

	// Run all rails
	for (int i = 0; i < RAIL_LIMIT; i++) {
		railSystem[i].Run();
	}
}

// Bound the input value between x_min and x_max.
float bound(float x, float x_min, float x_max) {
	if (x < x_min) { x = x_min; }
	if (x > x_max) { x = x_max; }
	return x;
}

bool read() {
	//------------------------------------
	// Input
	//------------------------------------

	// Check to see if anything is available in the serial receive buffer
	while (Serial.available() > 0) {
		const unsigned int MAX_MESSAGE_LENGTH = 32;

		// Create a place to hold the incoming message
		static char message[MAX_MESSAGE_LENGTH];
		static unsigned int message_pos = 0;

		// Read the next available byte in the serial receive buffer
		char inByte = Serial.read();

		// Message coming in (check not terminating character) and guard for over message size
		if (inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1)) {
	  		// Add the incoming byte to our message
	  		message[message_pos] = inByte;
	  		message_pos++;
		}
		// Full message received...
		else {
	  		// Add null character to string
	  		message[message_pos] = '\0';

			// Reset for the next message
			message_pos = 0;

			char* ptr;
			unsigned int i = 0;
			ptr = strtok(message, ":");

			while (ptr != NULL || i >= MESSAGE_LIMIT) {
				//data[i] = atoi(ptr);
				data[i] = ptr;
				ptr = strtok (NULL, ":");
				Serial.println(data[i]);
				i++;
			}

	  	return true;
		}
  	}

  	return false;
}