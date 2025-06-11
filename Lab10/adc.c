#include"adc.h"

void adc_init(void){
    //Clock Setup
    SYSCTL_RCGCADC_R |= 0x01;
    SYSCTL_RCGCGPIO_R |= 0x02;
    while((SYSCTL_PRGPIO_R & 0x02) == 0) {};

    //PB4
    GPIO_PORTB_DIR_R &= ~0x10;
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_DEN_R &= ~0x10;
    GPIO_PORTB_AMSEL_R |= 0x10;

    //ADC setup
    while((SYSCTL_PRADC_R & 0x1) == 0) {};
    ADC0_PC_R &= ~0xF;
    ADC0_PC_R |= 0x1;
    ADC0_ACTSS_R &= ~0x0008;
    ADC0_SSPRI_R |= 0x0123;
    ADC0_EMUX_R |= 0x000F;
    ADC0_SSMUX3_R &= ~0xF;
    ADC0_SSMUX3_R += 10;
    ADC0_SSCTL3_R = 0x0006;
    ADC0_IM_R &= ~0x0008;
    ADC0_ACTSS_R |= 0x0008;
}

uint16_t adc_read(void){
    uint16_t result;
    ADC0_PSSI_R = 0x0008;
    while((ADC0_RIS_R & 0x08) == 0) {};
    result = ADC0_SSFIFO3_R & 0xFFF;
    return result;
}

uint16_t adc_translate(uint16_t translate){
    return ((1.16572 * pow(10, -10) * pow(translate, 4)) - (4.00231 * pow(10, -7) * pow(translate, 3)) + (0.000513748 * pow(translate, 2)) - (0.315049 * translate) + 98.09759);
}
