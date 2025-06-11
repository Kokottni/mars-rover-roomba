/**
 * Driver for ping sensor
 * @file ping.c
 * @author Nick Kokott
 */

#include "ping.h"
#include "Timer.h"

// Global shared variables
// Use extern declarations in the header file

volatile uint32_t g_start_time = 0;
volatile uint32_t g_end_time = 0;
volatile enum{LOW, HIGH, DONE} g_state = LOW; // State of ping echo pulse

void ping_init (void){

    SYSCTL_RCGCTIMER_R |= 0x08;
    SYSCTL_RCGCGPIO_R |= 0x02;
    while((SYSCTL_PRGPIO_R & 0x02) == 0) {};
    while((SYSCTL_PRTIMER_R & 0x08) == 0) {};

    GPIO_PORTB_DEN_R |= 0x08;
    GPIO_PORTB_PCTL_R |= 0x00007000;

    // Configure and enable the timer
    TIMER3_CTL_R &= 0xFEFF;
    TIMER3_CFG_R |= 0x4;
    TIMER3_TBMR_R |= 0x0007;
    TIMER3_TBMR_R &= 0xFFEF;
    TIMER3_CTL_R |= 0x0C00;
    TIMER3_TBPR_R |= 0xFF;
    TIMER3_TBILR_R |= 0xFFFE;
    TIMER3_ICR_R |= 0x0400;
    TIMER3_IMR_R |= 0x0400;

    //Enable Interrupts
    NVIC_EN1_R |= 0x10;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();
    TIMER3_CTL_R |= 0x0100;
}

void ping_trigger (void){
    g_state = LOW;
    // Disable timer and disable timer interrupt
    //TIMER3_CTL_R &= 0xFEFF;
    TIMER3_IMR_R &= 0xFBFF;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= 0xF7;
    GPIO_PORTB_PCTL_R &= 0xFFFF8FFF;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    GPIO_PORTB_DIR_R |= 0x08;
    GPIO_PORTB_DATA_R |= 0x08;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= 0xF7;
    GPIO_PORTB_DIR_R &= 0xF7;
    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x0400;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x08;
    GPIO_PORTB_PCTL_R |= 0x00007000;
    TIMER3_IMR_R |= 0x0400;
    //TIMER3_CTL_R |= 0x0100;
}

void TIMER3B_Handler(void){

  // YOUR CODE HERE
  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens.

    if(TIMER3_MIS_R & 0x0400){
        TIMER3_ICR_R |= 0x0400;
        if(g_state == DONE){
            g_state = LOW;
        }
        else if(g_state == LOW){
            g_start_time = TIMER3_TBR_R;
            g_state = HIGH;
        }
        else if(g_state == HIGH){
            g_end_time = TIMER3_TBR_R;
            g_state = DONE;
        }

    }
}

uint32_t ping_getDistance (void){

    ping_trigger();
    timer_waitMillis(18.5);
    uint32_t dist = g_start_time - g_end_time;

    while(dist < 0){
        char overflow[20];
        sprintf(overflow, "OVERFLOW OCCURRED!");
        uart_sendStr(overflow);
        ping_trigger();
        timer_waitMillis(18.5);
        dist = g_start_time - g_end_time;
    }

    return dist;
}
