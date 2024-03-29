#ifndef BINCLOCK_H
#define BINCLOCK_H

//Some reading (if you want)
//https://stackoverflow.com/questions/1674032/static-const-vs-define-vs-enum

// Function definitions
int hFormat(int hours);
void lightHours(int units);
void lightMins(int units);
int hexCompensation(int units);
int decCompensation(int units);
void initGPIO(void);
void toggleTime(void);
void signalHandler(int signum);
void cleanUp();

// define constants
const char RTCAddr = 0x6f;
const char SEC = 0x00; // see register table in datasheet
const char MIN = 0x01;
const char HOUR = 0x02;

// define pins
const int LEDS[] = {2,3,25,22,21,27,4,6}; //H0-H4, M0-M5
const int SECS = 1;
const int BTNS[] = {7,0}; // B0, B1


#endif
