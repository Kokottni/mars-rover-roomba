#include "scan.h"

typedef struct{
    double angle;
    uint16_t dist;
    double linearWidth;
    int radialWidth;
} object_t;

void sendString(const char str[])
{
    int i;
    cyBot_sendByte('\r');
    cyBot_sendByte('\n');
    for(i = 0; str[i] != '\0'; ++i)
    {
        cyBot_sendByte(str[i]);
    }
}

void calibrate()
{
    timer_init();
    lcd_init();
    cyBOT_init_Scan(0b0111);
    cyBOT_SERVO_cal();
}

void scanAround(cyBOT_Scan_t *data)
{
    timer_init();
    lcd_init();
    cyBOT_init_Scan(0b0111);

    right_calibration_value = 248500;
    left_calibration_value = 1183000;

    int i;
    for(i = 0; i <= 180; ++i)
    {
        cyBOT_Scan(i, data);
        char arr[40];
        sprintf(arr, "%d\t\t%f\t\t%d", i, data->sound_dist, data->IR_raw_val);
        sendString(arr);
    }
}

void detectSmallest(cyBOT_Scan_t *data) {
    int objCount = 0;
    int scanning = 0;
    int currRadial = 0;
    int found = 0;
    int i;
    float minDistance = 250.0;
    int minRadialWidth = 181;
    int minAngle = 0;
    float prevDistance1 = 250.0;
    float prevDistance2 = 250.0;

    for (i = 0; i < 181; ++i) {
        cyBOT_Scan(i, data);
        if (data->sound_dist < 250 && scanning == 0 && fabs(data->sound_dist - prevDistance1) <= 3.0 && fabs(data->sound_dist - prevDistance2) <= 4) {
            scanning = 1;
            objCount++;
            found = i;
            currRadial = 1;
        } else if (data->sound_dist < 250 && scanning == 1 && fabs(data->sound_dist - prevDistance1) <= 3.0 && fabs(data->sound_dist - prevDistance2) <= 4) {
            currRadial++;
        } else if ((data->sound_dist > 250 || fabs(data->sound_dist - prevDistance1) > 3.0 || fabs(data->sound_dist - prevDistance2) > 4) && scanning == 1) {
            if (currRadial > 7) {
                char stuff[100];
                sprintf(stuff, "Object %d: Angle Found = %d, Distance = %.2f, Radial Width = %d", objCount, found, prevDistance1, currRadial + 1);
                sendString(stuff);

                // Check if this object has the smallest radial width
                if (currRadial < minRadialWidth) {
                    minRadialWidth = currRadial;
                    minDistance = data->sound_dist;
                    minAngle = found + currRadial / 2; // Middle of the object
                }
            }else{
                objCount--;
            }

            scanning = 0;
            currRadial = 0;
        }
        prevDistance2 = prevDistance1;
        prevDistance1 = data->sound_dist;
        char test[20];
        sprintf(test, "%d %.2f", i, data->sound_dist);
        sendString(test);
    }

    // Point CyBot to the object with the smallest radial width
    if (minRadialWidth < 181) {
        char result[100];
        sprintf(result, "Smallest Object: Angle = %d, Distance = %.2f, Radial Width = %d", minAngle, minDistance, minRadialWidth + 1);
        sendString(result);
        cyBOT_Scan(minAngle, &data[minAngle]);
    }
}

void drivetoSmallest(cyBOT_Scan_t *data, oi_t *sensor_data) {
    int objCount = 0;
    char scanning = 0;
    int currRadial = 0;
    int found = 0;
    char i;
    int minRadialWidth = 181;
    float minDistance = 250.0;
    float minLinearWidth = 1000;
    float closestDist = 1000;
    int minAngle = 0;
    float prevDistance1 = 250.0;
    int prevIR = 250.0;
    int minFound = 0;
    while(minDistance > 20){
        for (i = 0; i < 181; ++i) {
                cyBOT_Scan(i, data);
                if (data->sound_dist < 100 && scanning == 0 && fabs(data->sound_dist - prevDistance1) <= 5.0 && abs(data->IR_raw_val - prevIR) <= 200 && abs(data->IR_raw_val) > 650) {
                    scanning = 1;
                    objCount++;
                    found = i;
                    currRadial = 1;
                } else if (data->sound_dist < 100 && scanning == 1 && fabs(data->sound_dist - prevDistance1) <= 5.0 && abs(data->IR_raw_val - prevIR) <= 200) {
                    currRadial++;
                } else if ((data->sound_dist > 100 || abs(data->IR_raw_val) < 650 || fabs(data->sound_dist - prevDistance1) > 5.0 || abs(data->IR_raw_val - prevIR) > 200) && scanning == 1) {
                    if (currRadial > 3) {
                        char stuff[100];
                        float currLinear = 2 * M_PI * data->sound_dist * (currRadial / 360.0);
                        sprintf(stuff, "Object %d: Angle Found = %d, Distance = %.2f, Radial Width = %d, Linear Width = %.2f", objCount, found, prevDistance1, currRadial + 1, currLinear);
                        sendString(stuff);

                        // Check if this object has the smallest linear width
                        if (currLinear < minLinearWidth) {
                            minLinearWidth = currLinear;
                            minDistance = data->sound_dist;
                            minAngle = found + currRadial / 2;
                            minFound = found;
                            minRadialWidth = currRadial;
                        }
                        if(data->sound_dist < closestDist) {closestDist = data->sound_dist;}
                    } else {
                        objCount--;
                    }

                    scanning = 0;
                    currRadial = 0;
                }
                prevDistance1 = data->sound_dist;
                prevIR = data->IR_raw_val;

                char test[20];
                float IRdist = 0;
                char j;
                for (j = 0; j < 5; ++j) {
                    IRdist += data->IR_raw_val;
                }
                IRdist /= 5;
                sprintf(test, "%d\t%.2f\t%.2f", i, data->sound_dist, IRdist);
                sendString(test);
            }
            // Point CyBot to the object with the smallest linear width
            if (minRadialWidth < 181) {
                char result[100];
                sprintf(result, "Smallest Object: Starting Angle = %d, Ending Angle = %d, Distance = %.2f, Linear Width = %.2f", minFound, minFound + minRadialWidth, minDistance, minLinearWidth);
                sendString(result);
                if (minAngle < 90) {
                    int turn = abs(minAngle - 90);
                    turn_right(sensor_data, turn);
                } else if (minAngle > 90) {
                    int turn = minAngle - 90;
                    turn_left(sensor_data, turn);
                }
                double dist = bump_forward(sensor_data, ((minDistance - 15) * 10));
            }
            scanning = 0;
    }
}


//THIS IS THE JAWN WE USING
void getToDest(oi_t *sensor_data){
    object_t *objs[5];
    char found = 0;
    char dir = 0;
    while(1){
        int i, j, k;
        char object_started = 0;
        int radial = 0;
        double angle = 0;
        uint16_t prev_ir = 0;
        double prev_ping = 0;
        k = 0;
        for(i = 0; i < 5; ++i){
            objs[i] = NULL;
        }
        //initiate scan
        for(i = 0; i < 181; ++i){
            serv_move(i);
            uint16_t ir_data=0;
            double ping_data=0;
            //average readings
            for(j=0; j<3; ++j){
                ir_data += adc_read();
                ping_data += ping_getDistance();
            }
            ir_data /=3;
            ping_data /=3;
            //if we havent seen an object but indicate one starting
            if(!object_started && ping_data <= 46 && ir_data <= 46){
                object_started = 1;
                ++radial;
                angle = i;
            //if object looks to still be going
            }else if(object_started && ping_data <= 46 && ir_data <= 46 && fabs(prev_ir - ir_data) <= 3 && fabs(prev_ping - ping_data) <= 3){
                ++radial;
            //if it looks like object ends
            }else if(object_started && (ping_data >= 46 || ir_data >= 46 || fabs(prev_ir - ir_data) >= 3 || fabs(prev_ping - ping_data) >= 3)){
                //check if object is large enough to even be considered
                if(radial < 6){
                    object_started = 0;
                    radial = 0;
                    continue;
                }
                //if it is create object
                angle += (double)radial / 2;
                double linearWidth = 2 * M_PI * prev_ir * ((double)radial / 360.0);
                if(linearWidth < 5){
                    object_started = 0;
                    radial = 0;
                    continue;
                }
                object_t *obj = malloc(sizeof(object_t));
                obj->angle = angle;
                obj->dist = prev_ir;
                obj->linearWidth = linearWidth;
                obj->radialWidth = radial;
                objs[k] = obj;
                char stuff[150];
                sprintf(stuff, "Object %d: Angle Middle = %.2f, Distance = %hu, Radial Width = %d, Linear Width = %.2f", k, obj->angle, obj->dist, obj->radialWidth, obj->linearWidth);
                uart_sendStr(stuff);
                ++k;
                object_started = 0;
                radial = 0;
            }
            //use previous data to ensure real distance
            prev_ir = ir_data;
            prev_ping = ping_data;
        }

        //Logic for if we find the end zone
        for(i = 0; i < 5; ++i){
            if(!objs[i]){
                break;
            }
        }
        object_t *first = NULL;
        object_t *second = NULL;
        for(j = 0; j < i; ++j){
            if(objs[j]->linearWidth <= 8 && first == NULL){
                first = objs[j];
            }else if(objs[j]->linearWidth <= 8 && first != NULL && second == NULL){
                second = objs[j];
            }else if(objs[j]->linearWidth <= 8){
                uint16_t dist = objs[j]->dist;
                if(dist < first->dist && second->dist > first->dist){
                    first = objs[j];
                }
                if(dist < second->dist && first->dist > second->dist){
                    second = objs[j];
                }
            }
        }
        //if 2 small poles found go to middle and finish out
        if(first != NULL && second != NULL && first->angle > 15 && first-> angle < 165 && second->angle > 15 && second->angle < 165 && (first->dist < 29 || second->dist < 29)){
            //find mid point between pillars
            if(found){
                break;
            }
            double mid_angle = first->angle + ((second->angle - first->angle) / 2);
            uint16_t smallerDist = 0;
            if(first->dist < second->dist){
                smallerDist = first->dist;
            }else{
                smallerDist = second->dist;
            }
            uint16_t dist = smallerDist + fabs(first->dist - second->dist) / 2;
            //move robot there
            if(mid_angle < 90){
                double turn = fabs(mid_angle - 90);
                turn_right(sensor_data, (int)turn);
                move_forward(sensor_data, dist);
            }else{
                double turn = mid_angle - 90;
                turn_left(sensor_data, (int)turn);
                move_forward(sensor_data, dist);
            }
            //turn robot to face the middle
            if(first->dist > second->dist + 15){
                turn_left(sensor_data, 45);
            }
            if(first->dist + 15 < second->dist){
                turn_right(sensor_data, 45);
            }
            //go forward halfway into the pillar square
            move_forward(sensor_data, 550);
            //break while loop
            break;
        }
        //if not try to find a small pole and go to it
        else if(first != NULL && first->dist < 29 && first->angle > 15 && first->angle < 165){
            double turn = first->angle - 90;
            if(found){
                if(dir == 1 && first->angle < 60 && first->angle > 120){
                    turn_left(sensor_data, 110);
                    cliff_bump_forward(sensor_data, 600);
                }else if(dir == 0 && first->angle < 60 && first->angle > 120){
                    turn_right(sensor_data, 110);
                    cliff_bump_forward(sensor_data, 600);
                }
                break;
            }
            if(turn < 0){
                turn_right(sensor_data, fabs(turn));
            }else{
                turn_left(sensor_data, fabs(turn));
            }
            cliff_bump_forward(sensor_data, (first->dist * 10) - 100);
            if(turn < 0){
                turn_left(sensor_data, fabs(turn) * 2);
                dir = 1;
            }else{
                turn_right(sensor_data, fabs(turn) * 2);
            }
            cliff_bump_forward(sensor_data, 200);
            ++found;
            break;

        }
        //if not move the jawn forward
        else{
            //if any of the objects are within 20 cm of it turn 90 in opposite direction
            double p_angle = 181;
            uint16_t p_dist = 100;
            uint8_t close_count = 0;
            for(i = 0; i < 5; ++i){
                if(objs[i]->dist <= 22){
                    p_dist = objs[i]->dist;
                    p_angle = objs[i]->angle;
                    ++close_count;
                }
            }
            //if there are more than one objects, backup and rescan
            if(close_count > 1){
                move_backward(sensor_data, 100);
            //if its only one, just route around a safe distance
            }else if(close_count == 1 && p_angle > 35 && p_angle <155){
                double determine = p_angle - 90;
                if(determine < 0){
                    turn_left(sensor_data, 90);
                    cliff_bump_forward(sensor_data, 180);
                    turn_right(sensor_data, 90);
                    cliff_bump_forward(sensor_data, 180);
                }else{
                    turn_right(sensor_data, 90);
                    cliff_bump_forward(sensor_data, 180);
                    turn_left(sensor_data, 90);
                    cliff_bump_forward(sensor_data, 180);
                }
            //if nothing seen within 22 cm move forward 20 cm
            }else{
                cliff_bump_forward(sensor_data, 200);
            }
        }
        for(i = 0; i < 5; ++i){
            if(objs[i] != NULL){
                free(objs[i]);
            }
            objs[i] = NULL;
        }
    }
}
