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
void serv_move(uint16_t degrees, uint16_t right, uint16_t left);

//Move that jawn for configuration
void serv_move_configure(uint16_t degrees);

//configure turn
void configure(void);

#endif
