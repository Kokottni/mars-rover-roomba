#ifndef SCAN_H
#define SCAN_H

#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "open_interface.h"

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

#endif
