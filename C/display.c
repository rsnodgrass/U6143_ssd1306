/******
Demo for ssd1306 i2c driver for Raspberry Pi
******/
#include <stdio.h>
#include "ssd1306_i2c.h"
#include "time.h"
#include <unistd.h>

#define DEBUG 1

#define DISTINCT_DISPLAYS 3

#define SECONDS_TO_DISPLAY 3.0
#define NANOSECONDS_PER_SECOND 1000000000

/**
  Sleep until the next second that the display should change. This should allow
  all displays to change at roughly the same instance in a rack without any
  coordination.
*/
float sleep_until_scheduled_display_change() {
     struct timespec current;
     clock_gettime(CLOCK_MONOTONIC_RAW, &current);

     long duration = SECONDS_TO_DISPLAY * NANOSECONDS_PER_SECOND; // FIXME: calculate
     if (DEBUG) printf("Sleeping %0.3f seconds", duration / NANOSECONDS_PER_SECOND);


     struct timespec remaining, sleep_ts = { 
        (time_t)( duration / NANOSECONDS_PER_SECOND ),
        duration % NANOSECONDS_PER_SECOND
     };

     nanosleep(&sleep_ts, &remaining);
}

/**
  This selects which display to be shown based on the current time. For RPi
  which have system time synchronized closely, the displays should all show
  the same information without coordination.
*/
unsigned char select_display() {
     struct timespec current;
     clock_gettime(CLOCK_MONOTONIC_RAW, &current);
     return current.tv_sec % DISTINCT_DISPLAYS;
}

int main(void) 
{
    ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);      //LCD Screen initialization
    if(i2cd<0)
    {
        printf("I2C device failed to open\r\n");
        return 0;
    }

    usleep(150*1000); // Short delay Ensure the normal response of the lower function
    FirstGetIpAddress();

    while(1)
    {
        unsigned char symbol = select_display();
        if (DEBUG) printf("Display = %d", symbol);
        LCD_Display(symbol);

        sleep_until_scheduled_display_change();
    }
    return 0;
}
