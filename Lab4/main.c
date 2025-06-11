#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "scan.h"
#include "button.h"

int main (void) {
    button_init();
    timer_init();
    lcd_init();
    cyBot_uart_init();

    uint8_t furthestRight;
    char resp[30];

    while(1)
    {
        furthestRight = button_getButton();
        if(furthestRight == 1)
        {
            sprintf(resp, "Man button %d sucks", furthestRight);
            //lcd_printf(resp);
            sendString(resp);
        }
        if(furthestRight == 2)
        {
            sprintf(resp, "Sheesh you found %d", furthestRight);
            //lcd_printf(resp);
            sendString(resp);
        }
        if(furthestRight == 3)
        {
            sprintf(resp, "%drd base is cool", furthestRight);
            //lcd_printf(resp);
            sendString(resp);
        }
        if(furthestRight == 4)
        {
            sprintf(resp, "%d? HOME RUN!!!", furthestRight);
            //lcd_printf(resp);
            sendString(resp);
        }
    }



    return 0;
}
