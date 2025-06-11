#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "scan.h"
#include "button.h"
#include "uart.h"
#include "adc.h"

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

    adc_init();
    while(1){
        int i;
        uint16_t avg_val = 0;
        for(i = 0; i < 16; ++i){
            avg_val += adc_read();
        }
        avg_val /= 16;
        uint16_t converted = adc_translate(avg_val);
        char output[50];
        sprintf(output, "Raw ADC value: %d  Converted to cm: %d", avg_val, converted);
        uart_sendStr(output);
        char receive = 'a';
        while(receive != 'n'){
            receive = uart_receive();
        }
    }

    return 0;
}
