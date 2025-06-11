#include "scan.h"
#include "Timer.h"
#include "lcd.h"
#include "uart.h"

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
    char input;
    for(i = 0; i <= 180; ++i)
    {
        cyBOT_Scan(i, data);
        char arr[40];
        sprintf(arr, "%d\t\t%f", i, data->sound_dist);
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
