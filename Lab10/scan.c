#include "scan.h"
#include "Timer.h"
#include "lcd.h"
#include "uart.h"
#include "math.h"
#include "movement.h"

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
