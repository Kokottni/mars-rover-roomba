#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "open_interface.h"

//moves the robot forward a specfied distance in mm
double move_forward(oi_t *sensor_data, float distance_mm);

//moves robot backward specified distance in mm
double move_backward(oi_t *sensor_data, float distance);

//turns the robot x degrees to the left
double turn_left(oi_t *sensor_data, double degrees);

//turns the robot x degrees to the right
double turn_right(oi_t *sensor_data, double degrees);

//allows the robot to bump into an object and reroute accordingly based on the bump sensor that is triggered
double bump_forward(oi_t *sensor_data, float distance);

//Same thing as bump but uses cliff
double cliff_bump_forward(oi_t *sensor_data, float distance);


#endif
