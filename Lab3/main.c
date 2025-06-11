#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "scan.h"

int main (void) {
//    calibrate();

    timer_init();
    lcd_init();
    cyBOT_init_Scan(0b0011);
    cyBot_uart_init();

    cyBOT_Scan_t *scan = (cyBOT_Scan_t*)calloc(1, sizeof(cyBOT_Scan_t));

    right_calibration_value = 337750;
    left_calibration_value = 1335250;

    detectSmallest(scan);

    free(scan);
    return 0;
}
