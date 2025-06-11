#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "scan.h"
#include "button.h"
#include "uart.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"
#define IMPERIAL_MARCH 1

int main (void) {


    uart_init();
    adc_init();
    ping_init();
    serv_init();
    //configure();
    uint16_t right = 57227;
    uint16_t left = 30122;
    serv_setVals(right, left);

    //THESE VALUES ARE NEEDED FOR CALIBRATION DO NOT REMOVE


    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    oi_setWheels(0, 0);

//    uint16_t val1 = 0;
//    uint16_t val2 = 0;
//    uint16_t val3 = 0;
//    uint16_t val4 = 0;
//    char next = 0;
//    while(next != 'e'){
//        next = uart_receive();
//        oi_update(sensor_data);
//        val1 = sensor_data->cliffFrontLeftSignal;
//        val2 = sensor_data->cliffFrontRightSignal;
//        val3 = sensor_data->cliffLeftSignal;
//        val4 = sensor_data->cliffRightSignal;
//        char arr[50];
//        sprintf(arr, "FrontL: %d | FrontR: %d | L: %d | R: %d", val1, val2, val3, val4);
//        uart_sendStr(arr);
//    }
    getToDest(sensor_data);

    char arr[50];
    sprintf(arr, "Hurray! You've arrived at your destination!");
    uart_sendStr(arr);
    unsigned char ImperialMarchNumNotes = 19;
    unsigned char ImperialMarchNotes[19]  = {55, 55, 55, 51, 58, 55, 51, 58, 55, 0,  62, 62, 62, 63, 58, 54, 51, 58, 55};
    unsigned char ImperialMarchDurations[19] = {32, 32, 32, 20, 12, 32, 20, 12, 32, 32, 32, 32, 32, 20, 12, 32, 20, 12, 32};
    oi_loadSong(IMPERIAL_MARCH, ImperialMarchNumNotes, ImperialMarchNotes, ImperialMarchDurations);
    oi_play_song(IMPERIAL_MARCH);
    free(sensor_data);

    return 0;
}
