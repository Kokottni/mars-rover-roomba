#include "movement.h"
#include "lcd.h"
#include <stdlib.h>

double move_forward(oi_t *sensor_data, float distance)
{
    double total = 0;
    oi_setWheels(250, 250);
    while(total < distance)
    {
        oi_update(sensor_data);
        total += fabs(sensor_data->distance);
    }
    oi_setWheels(0,0);
    return total;
}

double turn_left(oi_t *sensor_data, double degrees)
{
    float total = 0;
    oi_setWheels(50, -50);
    while(total < degrees)
    {
        oi_update(sensor_data);
        total += fabs(sensor_data->angle);
    }
    oi_setWheels(0, 0);
    return total;
}

double turn_right(oi_t *sensor_data, double degrees)
{
    float total = 0;
    oi_setWheels(-50, 50);
    while(total < degrees)
    {
        oi_update(sensor_data);
        total += fabs(sensor_data->angle);
    }
    oi_setWheels(0, 0);
    return total;
}

double bump_forward(oi_t *sensor_data, float distance) {
    double total_distance = 0;
    const double move_back = 150;   // Distance to move backward
    const double lateral_move = 200; // Lateral move distance
    char r, l = 0;

    oi_setWheels(250, 250); // Start moving forward

    while (total_distance < distance) {
        oi_update(sensor_data); // Update sensor data
        total_distance += fabs(sensor_data->distance);

        // Debugging: Print current distance and bump statuses
        printf("Distance traveled: %.2f, Bump Right: %d, Bump Left: %d\n", total_distance, sensor_data->bumpRight, sensor_data->bumpLeft);

        if (sensor_data->bumpRight || sensor_data->bumpLeft) {
            if(sensor_data->bumpRight) {r = 1;}
            if(sensor_data->bumpLeft) {l = 1;}
            // Stop and reverse the robot
            double back_dist = 0;
            oi_setWheels(-250, -250); // Reverse
            while (back_dist < move_back) {
                oi_update(sensor_data);
                back_dist += fabs(sensor_data->distance);
            }
            total_distance -= back_dist; // Adjust total distance
            oi_setWheels(0, 0); // Stop wheels after reversing

            // Turn based on bump sensor triggered
            if (r) {
                turn_left(sensor_data, 90); // Turn left if right bumper is triggered
            } else if (l) {
                turn_right(sensor_data, 90); // Turn right if left bumper is triggered
            }

            // Move forward to avoid the obstacle and return to the path
            move_forward(sensor_data, lateral_move);

            // Realign the robot forward
            if (r) {
                turn_right(sensor_data, 90); // Realign after left turn
            } else {
                turn_left(sensor_data, 90); // Realign after right turn
            }
            r = l = 0;
            oi_setWheels(250, 250); // Resume forward motion
        }
    }

    oi_setWheels(0, 0); // Stop once the target distance is reached
    return total_distance; // Return total traveled distance
}

