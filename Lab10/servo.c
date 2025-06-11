#include "servo.h"

volatile uint32_t curr_pos = 0;
uint16_t max = 41856;

void serv_init(void){

    SYSCTL_RCGCTIMER_R |= 0x02;
    SYSCTL_RCGCGPIO_R |= 0x02;
    while((SYSCTL_RCGCGPIO_R & 0x02) == 0) {};
    while((SYSCTL_PRTIMER_R & 0x02) == 0) {};

    //GPIO PORT SETUP
    GPIO_PORTB_DIR_R |= 0x20;
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R |= 0x00700000;
    GPIO_PORTB_DEN_R |= 0x20;

    //TIMER SETUP
    TIMER1_CTL_R &= 0xFEFF;
    TIMER1_CFG_R |= 0x4;
    TIMER1_TBMR_R &= ~0x4;
    TIMER1_TBMR_R |= 0x000A;
    TIMER1_TBMR_R &= ~0x10;
    TIMER1_TBPR_R |= 0x04;
    TIMER1_TBILR_R |= 0xE200;
    TIMER1_TBMATCHR_R |= 0xA380;
    TIMER1_TBPMR_R |= 0x04;
    TIMER1_CTL_R |= 0x0100;
}


void serv_move(uint16_t degrees, uint16_t right, uint16_t left){
    TIMER1_CTL_R &= ~0x100;
    TIMER1_TBPR_R |= 0x4;
    TIMER1_TBILR_R |= 0xE200;
    uint16_t diff = right - left;
    uint16_t match = right - (diff * ((float)degrees/180));
    TIMER1_TBMATCHR_R = (match & 0xFFFF);
    TIMER1_TBPMR_R |= 0x04;
    TIMER1_CTL_R |= 0x100;
    timer_waitMillis(1000);
}


void serv_move_configure(uint16_t degrees){
    TIMER1_CTL_R &= ~0x100;
    TIMER1_TBPR_R |= 0x4;
    TIMER1_TBILR_R |= 0xE200;
    uint16_t match = max - (16000 * ((float)degrees/180));
    TIMER1_TBMATCHR_R = (match & 0xFFFF);
    TIMER1_TBPMR_R |= 0x04;
    TIMER1_CTL_R |= 0x100;
    timer_waitMillis(10);
}

void configure(void){
    timer_init();
    lcd_init();
    button_init();
    uint8_t finish = 0;
    int degree_curr = 0;
    uint16_t timer_right_val = 0;
    uint16_t timer_left_val = 0;
    serv_move_configure(0);
    while(finish != 2){
        uint8_t button = 0;
        while(button == 0){
            button = button_getButton();
        }
        if(button == 4){
            ++finish;
            if(finish == 1){
                timer_right_val = TIMER1_TBMATCHR_R;
            }
            else{
                timer_left_val = TIMER1_TBMATCHR_R;
            }
        }
        if(button == 1){
            --degree_curr;
            serv_move_configure(degree_curr);
        }
        if(button == 2){
            ++degree_curr;
            serv_move_configure(degree_curr);
        }
    }
    char vals[40];
    sprintf(vals, "Right: %d\tLeft: %d", timer_right_val, timer_left_val);
    uart_sendStr(vals);
}
