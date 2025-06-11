#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "scan.h"
#include "button.h"
#include "uart.h"

int main (void) {
    timer_init();
    uart_init();
    char data;
    while(1)
    {
        data = uart_receive();
        if(data == '\r')
        {
            uart_sendChar('\n');
            uart_sendChar('\r');
        }
        else
        {
            uart_sendChar(data);
        }
    }
    return 0;
}
