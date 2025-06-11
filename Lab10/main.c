#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "scan.h"
#include "button.h"
#include "uart.h"
#include "adc.h"
#include "ping.h"
#include "servo.h"

int main (void) {
    uart_init();

    serv_init();
    //configure();
    uint16_t right = 57316;
    uint16_t left = 30656;
    while(1){
        serv_move(0, right, left);
        serv_move(30, right, left);
        serv_move(60, right, left);
        serv_move(90, right, left);
        serv_move(120, right, left);
        serv_move(150, right, left);
        serv_move(180, right, left);
    }

    return 0;
}
