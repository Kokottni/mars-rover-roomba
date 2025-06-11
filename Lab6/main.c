#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "scan.h"
#include "button.h"
#include "uart-interrupt.h"

int main (void) {
    timer_init();
    uart_interrupt_init();
    while(1){}
    cyBOT_init_Scan(0b0011);
    cyBOT_Scan_t *scan = (cyBOT_Scan_t*)calloc(1, sizeof(cyBOT_Scan_t));

    scanAround(scan);
    return 0;
}
