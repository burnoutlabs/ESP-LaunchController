#include "rlht.h"

#include <Wire.h>
#include <Arduino.h>
#include <stdint.h>

// Global variables
RLHT_t RLHT[NUM_RLHT_SLICES];

int relay_heaters[2][4] = {
    {10, 1, 1, 0}, // C0H
    {10, 2, 1, 1}, // C0F
};

float relay_heater_pid[2][4]; // relay heater PID {setpoint, Kp, Ki, Kd}

float relay_heater_val[2] = {0, 0};

// Function definitions
void initSlices() {
    // RLHT Slice address assignment
    for (int i = 0; i < NUM_RLHT_SLICES; i++) {
        RLHT[i].address = 10 + i; // addresses 10-15
    }
}

void handleSerialCommand() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim(); // Remove any leading/trailing whitespace

        int index = 0; // Assuming a single RLHT slice

        switch (command.charAt(0)) {
            case 's': // Setpoint
                relay_heater_pid[index][0] = command.substring(1).toFloat();
                RLHTCommandSetpoint(relay_heaters[index][0], relay_heaters[index][1], relay_heater_pid[index][0], relay_heaters[index][2], relay_heaters[index][3]);
                Serial.println("Setpoint updated");
                break;

            case 'p': // Kp
                relay_heater_pid[index][1] = command.substring(1).toFloat();
                RLHTCommandPID(relay_heaters[index][0], relay_heaters[index][1], relay_heater_pid[index][1], relay_heater_pid[index][2], relay_heater_pid[index][3]);
                Serial.println("Kp updated");
                break;

            case 'i': // Ki
                relay_heater_pid[index][2] = command.substring(1).toFloat();
                RLHTCommandPID(relay_heaters[index][0], relay_heaters[index][1], relay_heater_pid[index][1], relay_heater_pid[index][2], relay_heater_pid[index][3]);
                Serial.println("Ki updated");
                break;

            case 'd': // Kd
                relay_heater_pid[index][3] = command.substring(1).toFloat();
                RLHTCommandPID(relay_heaters[index][0], relay_heaters[index][1], relay_heater_pid[index][1], relay_heater_pid[index][2], relay_heater_pid[index][3]);
                Serial.println("Kd updated");
                break;

            default:
                Serial.println("Invalid command");
                break;
        }
    }
}


void RLHTRequestThermo(int address, float *t1, float *t2) {
    FLOATUNION_t thermo1;
    FLOATUNION_t thermo2;

    thermo1.number = 0;
    thermo2.number = 0;

    // Request 8 bytes from the given address
    int bytesRead = Wire.requestFrom(address, 8);
    if (bytesRead != 8) {
        Serial.print("Error: Expected 8 bytes, received ");
        Serial.println(bytesRead);
        *t1 = thermo1.number; // Assigning the default zero value
        *t2 = thermo2.number; // Assigning the default zero value
        return;
    }

    // Read data if 8 bytes are available
    byte in_data[8];
    for (int i = 0; i < 8; i++) {
        if (Wire.available()) {
            in_data[i] = Wire.read(); // Receive a byte
        } else {
            Serial.println("Error: Data not available");
            *t1 = thermo1.number; // Assigning the default zero value
            *t2 = thermo2.number; // Assigning the default zero value
            return;
        }
    }

    // Parse received data into floats
    for (int x = 0; x < 4; x++) {
        thermo1.bytes[x] = in_data[x];
        thermo2.bytes[x] = in_data[x + 4];
    }

    *t1 = thermo1.number;
    *t2 = thermo2.number;
}


void RLHTCommandSetpoint(int address, byte heater, float heatSetpoint, byte thermocouple, bool enableReverse) {
    FLOATUNION_t setpoint;
    setpoint.number = heatSetpoint;

    Wire.beginTransmission(address);
    Wire.write('H');
    Wire.write(heater);
    for (int i = 0; i < 4; i++) {
        Wire.write(setpoint.bytes[i]); // sends one byte
    }
    Wire.write(thermocouple);
    Wire.write(enableReverse);
    Wire.endTransmission(); // stop transmitting

    Serial.print(address);
    Serial.print('H');
    Serial.print(heater);
    Serial.print(setpoint.number);
    Serial.print(thermocouple);
    Serial.println(enableReverse);
}

void RLHTCommandPID(int address, byte heater, float Kp_set, float Ki_set, float Kd_set) {
    FLOATUNION_t Kp;
    FLOATUNION_t Ki;
    FLOATUNION_t Kd;

    Kp.number = Kp_set;
    Ki.number = Ki_set;
    Kd.number = Kd_set;

    Wire.beginTransmission(address);
    Wire.write('P');
    Wire.write(heater);
    for (int i = 0; i < 4; i++) {
        Wire.write(Kp.bytes[i]); // sends one byte
    }
    for (int i = 0; i < 4; i++) {
        Wire.write(Ki.bytes[i]); // sends one byte
    }
    for (int i = 0; i < 4; i++) {
        Wire.write(Kd.bytes[i]); // sends one byte
    }
    Wire.endTransmission(); // stop transmitting

    Serial.print(address);
    Serial.print('P');
    Serial.print(heater);
    Serial.print(Kp.number);
    Serial.print(Ki.number);
    Serial.println(Kd.number);
}
