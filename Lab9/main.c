#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "scan.h"
#include "button.h"
#include "uart.h"
#include "adc.h"
#include "ping.h"

int main (void) {
//    calibrate();

//    timer_init();
//    oi_t *sensor_data = oi_alloc();
//    oi_init(sensor_data);
//    oi_setWheels(0, 0);
    uart_init();
//
//    cyBOT_init_Scan(0b0111);
//    cyBOT_Scan_t *scan = (cyBOT_Scan_t*)calloc(1, sizeof(cyBOT_Scan_t));
//    right_calibration_value = 280000;
//    left_calibration_value = 1198750;
//
//    drivetoSmallest(scan, sensor_data);

    ping_init();
    uint32_t dist;
    while(1){
        dist = ping_getDistance();
        double cm = dist * 0.0343 / (2 * 16);
        char output[70];
        sprintf(output, "There were %d clock cycles for this trigger, which is %f.2 cm away.", dist, cm);
        uart_sendStr(output);
    }

    return 0;
}
