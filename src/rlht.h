#ifndef RLHT_H
#define RLHT_H

#include <Wire.h>
#include <Arduino.h>
#include <stdint.h>

// Definitions
#define SLICE_DATA_INTERVAL_MS 5000
#define SD_CS 5
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCK 18
#define ESTOP 32
#define NUM_RLHT_SLICES 1

// Data structures
union FLOATUNION_t {
    float number;
    uint8_t bytes[4];
};

union INTUNION_t {
    int number;
    uint8_t bytes[4];
};

struct RLHT_t {
    int address;
    float heatSetpoint_1;
    float Kp_1;
    float Ki_1;
    float Kd_1;
    float heatSetpoint_2;
    float Kp_2;
    float Ki_2;
    float Kd_2;
    float thermo1;
    float thermo2;
};

// Global variables
extern RLHT_t RLHT[NUM_RLHT_SLICES];
extern int relay_heaters[2][4];
extern float relay_heater_pid[2][4];
extern float relay_heater_val[2];

// Function declarations
void initSlices();
void handleSerialCommand();
void RLHTRequestThermo(int address, float *t1, float *t2);
void RLHTCommandSetpoint(int address, byte heater, float heatSetpoint, byte thermocouple, bool enableReverse);
void RLHTCommandPID(int address, byte heater, float Kp_set, float Ki_set, float Kd_set);

#endif // RLHT_H
