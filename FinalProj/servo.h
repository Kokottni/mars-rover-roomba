#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "uart.h"
#include "Timer.h"
#include "lcd.h"
#include "button.h"

//Init the servo
void serv_init(void);

//move that jawn normally
void serv_move(uint16_t degree);

//Move that jawn for configuration
void serv_move_configure(uint16_t degrees);

//configure turn
void configure(void);

void serv_setVals(uint16_t r, uint16_t l);

#endif
