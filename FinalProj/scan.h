#ifndef SCAN_H
#define SCAN_H

#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"
#include "uart.h"
#include "math.h"
#include "movement.h"
#include "ping.h"
#include "adc.h"
#include "servo.h"
#include "cyBot_Scan.h"
#include "cyBot_uart.h"

//send strings
void sendString(const char str[]);

//initial calibrate
void calibrate();

//uses the 180 degree scan
void scanAround();

//finds smallest object in 180 degree scan
void detectSmallest(cyBOT_Scan_t *data);

//drives to smallest object in 180 degree scan
void drivetoSmallest(cyBOT_Scan_t *data, oi_t *sensor_data);

//FINAL PROJECT FUNCTION
void getToDest(oi_t *sensor_data);

#endif
