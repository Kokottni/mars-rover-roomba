/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"
#include "movement.h"

int main (void) {
	timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
	              // and enables time functions (e.g. timer_waitMillis)
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
	lcd_init();   // Initialize the LCD screen.  This also clears the screen. 

	// Print "Hello, world" on the LCD
	//lcd_printf("Hello, world");
    double distance_moved = 0;

    //basic 1m forward
//    distance_moved = move_forward(sensor_data, 1000);
//    timer_waitMillis(2000);
    oi_setWheels(0,0);
    //square
    double amount_turned = 0;
//    distance_moved = move_forward(sensor_data, 500);
//    amount_turned = turn_right(sensor_data, 86);
//    distance_moved = move_forward(sensor_data, 500);
//    amount_turned = turn_right(sensor_data, 86);
//    distance_moved = move_forward(sensor_data, 500);
//    amount_turned = turn_right(sensor_data, 86);
//    distance_moved = move_forward(sensor_data, 500);
//    amount_turned = turn_right(sensor_data, 86);
//
//    //move x distance, with bump detection
    distance_moved = bump_forward(sensor_data, 2000);
	//lcd_puts("Hello, world"); // Replace lcd_printf with lcd_puts
        // step through in debug mode and explain to TA how it works
	//char msg[] = "Microcontrollers are lots of fun!";

    //lcd_rotatingBanner(msg);
	// NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
       // NOTE: For time functions, see Timer.h
	oi_free(sensor_data);
	return 0;
}
