/*
 * Luke Vassallo @ 2021/06/29 9:37
 * Simple program to read XLCn (XLC2 / XLC4) thermocouple data.
 * 
 * Make sure to configure the serial port prior to launching the application.
 * at the command line type : stty -F /dev/ttyS0 9600 cs8 -cstopb -parenb -ixon -echo
 * This configures /dev/ttyS0 accordingly : 9600 baud, 8 data bits, 1 stop bit, no parity, no hardware flow control, local echo disabled.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <xlc.h>

/* Definitions */
#define MAX_LINE_LENGTH 128


int main(int argc, char **argv)
{
    FILE *dev;              // file pointer to serial device.
    struct tc_data dat;     // thermocouple data structure
    // variables required for reading file data.
    char *c = NULL; 
    char line[128];

    if (argc < 2) 
    { 
        printf("Error: Incorrect usage. ./parser <serial-device>\r\n"); 
        exit(-1);
    }

    dev = fopen(argv[1], "r");  // open device in read mode

    if (dev == NULL)
    {
        printf("Error: Failed to open device.\r\n");
        exit(-1);
    }

    while(1)
    {
        c = fgets(line, MAX_LINE_LENGTH, dev);
        if ( c != NULL )
        {
            if (parse_string(line, &dat) == 0)
            {
                if (dat.protocol == PROTOCOL_TLOGA) { print_tloga(&dat); }
                else if (dat.protocol == PROTOCOL_TLOGB) { print_tlogb(&dat); }
                else { printf("err\n"); }
            }            
        }
        else { continue; }
    }
    
    fclose(dev);
    return 0;
}
