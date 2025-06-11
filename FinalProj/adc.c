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
    return adc_translate(result);
}

uint16_t adc_translate(uint16_t translate){
    //bot 2041-0
    return ((2.51978 * pow(10, -12) * pow(translate, 4)) - (2.5335 * pow(10, -8) * pow(translate, 3)) + (0.0000929871 * pow(translate, 2)) - (0.154988 * translate) + 115.9421);
    //bot 2041-13
    return ((3.85626 * pow(10, -11) * pow(translate, 4)) - (2.53605 * pow(10, -7) * pow(translate, 3)) + (0.000598603 * pow(translate, 2)) - (0.630033 * translate) + 281.31968);
}
