#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "scan.h"
#include "button.h"
#include "uart-interrupt.h"

int main (void) {
//    calibrate();

    timer_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    oi_setWheels(0, 0);
    uart_interrupt_init();

    cyBOT_init_Scan(0b0111);
    cyBOT_Scan_t *scan = (cyBOT_Scan_t*)calloc(1, sizeof(cyBOT_Scan_t));
    right_calibration_value = 280000;
    left_calibration_value = 1198750;

    drivetoSmallest(scan, sensor_data);

    return 0;
}
