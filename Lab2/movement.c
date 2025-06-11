#include "movement.h"
#include "lcd.h"
#include <stdlib.h>

double move_forward(oi_t *sensor_data, double distance)
{
    double total = 0;
    oi_setWheels(250, 250);
    while(total < distance)
    {
        oi_update(sensor_data);
        total += sensor_data->distance;
        lcd_printf("Distance: %lf", total);
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

double bump_forward(oi_t *sensor_data, double distance)
{
    double total_distance = 0;
    double move_back = 150;
    double lateral_move = 250;
    oi_setWheels(250, 239);
    while(total_distance < distance)
    {
        oi_update(sensor_data);
        total_distance += fabs(sensor_data->distance);
        if(sensor_data->bumpRight)
        {
            double back_dist = 0;
            oi_setWheels(-250, -250);
            while(back_dist < move_back)
            {
                oi_update(sensor_data);
                back_dist += fabs(sensor_data->distance);
            }
            total_distance -= move_back;
            turn_left(sensor_data, 90);
            move_forward(sensor_data, lateral_move);
            turn_right(sensor_data, 90);
            oi_setWheels(250, 239);
        }
        else if(sensor_data->bumpLeft)
        {
            double back_dist = 0;
            oi_setWheels(-250, -250);
            while(back_dist < move_back)
            {
                oi_update(sensor_data);
                back_dist += fabs(sensor_data->distance);
            }
            total_distance -= move_back;
            turn_right(sensor_data, 90);
            move_forward(sensor_data, lateral_move);
            turn_left(sensor_data, 90);
            oi_setWheels(250, 239);
        }
        else if(sensor_data->bumpRight && sensor_data->bumpLeft)
        {
            double back_dist = 0;
            oi_setWheels(-250, -250);
            while(back_dist < move_back)
            {
                oi_update(sensor_data);
                back_dist += fabs(sensor_data->distance);
            }
            total_distance -= move_back;
            turn_left(sensor_data, 90);
            move_forward(sensor_data, lateral_move);
            turn_right(sensor_data, 90);
            oi_setWheels(250, 239);

        }
    }
    return total_distance;
}
