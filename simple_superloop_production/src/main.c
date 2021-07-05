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

/* Definitions */
#define MAX_LINE_LENGTH 128

#define MAX_FIELDS          16      // TLOGB has 16 fields after, the protocol identifier.
#define MAX_FIELD_LENGTH    12      // Floating point numerals are printed using 10 characters, such as "?XXXX.yyyy" (worst case scenario). 

#define PROTOCOL_TLOGA  0
#define PROTOCOL_TLOGB  1

/* Declarations */
struct tc_data {
    uint8_t protocol;       // Identifies the protocol. 0 = TLOGA, 1 = TLOGB, anything else is not valid.
    float   tc[4];          // Cold Junction temperature
    float   th[4];          // Hot Junction temperature
    char    unit[4];        // Temperature unit (C / F / K)
    uint8_t status[4];      // Channel status
    bool    valid[4];       // True if data at index is valid, false otherwise. Used to be able to parse data from both 2/4 channel devices in a unified manner.
};

/* Function Prototypes */
int parse_string(char *s, struct tc_data *dat); // Parses a <CR><LF> terminated string as received from an XLCn device. This can be a command or data string.
int parse_tloga(char *s, struct tc_data *dat);  // Parses a <CR><LF> terminated string with protocol identifier removed. i.e. "$TLOGA," is not passed to the function, only the remainder.
int parse_tlogb(char *s, struct tc_data *dat);  // Parses a <CR><LF> terminated string with protocol identifier removed. i.e. "$TLOGB," is not passed to the function, only the remainder.
int print_tloga(struct tc_data *dat);           // Prints data to standard output, assuming structure contains data parsed from TLOGA
int print_tlogb(struct tc_data *dat);           // Prints data to standard output, assuming structure contains data parsed from TLOGB

// Parses a <CR><LF> terminated string as received from an XLCn device and stores data in floating point format. 
// Accepts a string as received from an XLCn device and a pointer to a thermocouple data structure.
// The string can be a data string (ex: $TLOGA or $TLOGB) or a command response string. Parsing is not supported for the latter.
// Returns 0 on success, -2 / -3 / -4 on error.
int parse_string(char *s, struct tc_data *dat)
{   
    char tmp[64];
    int i=0;
    
    // get field[0] 
    // if tloga/tlogb parse that 
    // else return error

    while( (s[i] != '\r') && (s[i] != '\n') )
    {
        if (s[i] == ',')
        {
           tmp[i] = '\0';
    
            if (strcmp(tmp, "$TLOGA") == 0)
            {
                if (parse_tloga(&s[++i], dat) < 0 ) { return -2; }
                else { return 0; }
            }
            else if (strcmp(tmp, "$TLOGB") == 0)
            {
                if(parse_tlogb(&s[++i], dat)) { return -3; }
                else { return 0; }
            }
            else
            {
                // parse command response
                return -4;
            }
        }
        else
        {
            tmp[i] = s[i];
        }
        i++;
    }

    return -1;
}

// Accepts TLOGA string payload and a pointer to thermocouple data structure (tc_data).
// Assumes a TLOGA string with protocol header removed. i.e. "$TLOGA," has already been stripped.
// Returns 0 on success, -1 or -2 in the event of a parsing error.
int parse_tloga(char *s, struct tc_data *dat)
{
    dat->protocol = PROTOCOL_TLOGA;
    char fields[8][8];
    
    int i,j,k;
    i = 0;  // character index in input string.
    j = 0;  // field index.
    k = 0;  // character index in field.
    
    for(i=0; i < 4; i++) { dat->valid[i] = false; }
    i=0;    // reset input string character index to 0.

    while( (s[i] != '\r') && (s[i] != '\n') )
    {
        if (k > MAX_FIELD_LENGTH)
        {
            // string field length exceeds field array storage size
            return -1;
        }

        if (j > MAX_FIELDS - 1)
        {
            // string items exceed no of field storage elements.
            return -2;
        }

        if (s[i] == ',')
        {
            fields[j][k] = '\0';
            j++;
            k = 0;
        }
        else
        {
            fields[j][k++] = s[i];
        }
        i++;
    }
    fields[j][k] = '\0';
    j++;                    // due to having n commas and n+1 values.
    
    for(i=0; i < j/2; i++)
    {
        dat->th[i] = atof(fields[2*i]);
        dat->unit[i] = fields[2*i+1][0];
        dat->valid[i] = true;
    }
    
    return 0;
}

// Accepts TLOGB string payload and a pointer to thermocouple data structure (tc_data).
// Assumes a TLOGB string with protocol header removed. i.e. "$TLOGB," has already been stripped.
// Returns 0 on success, -1 or -2 in the event of a parsing error.
int parse_tlogb(char *s, struct tc_data *dat)
{
    dat->protocol = PROTOCOL_TLOGB;
    char fields[16][8];
    
    int i,j,k;
    i = 0;  // character index in input string.
    j = 0;  // field index.
    k = 0;  // character index in field.
    
    for(i=0; i < 4; i++) { dat->valid[i] = false; }
    i=0;    // reset input string character index to 0.

    while( (s[i] != '\r') && (s[i] != '\n') )
    {
        if (k > MAX_FIELD_LENGTH)
        {
            // string field length exceeds field array storage size
            return -1;
        }

        if (j > MAX_FIELDS - 1)
        {
            // string items exceed no of field storage elements.
            return -2;
        }

        if (s[i] == ',')
        {
            fields[j][k] = '\0';
            j++;
            k = 0;
        }
        else
        {
            fields[j][k++] = s[i];
        }
        i++;
    }
    fields[j][k] = '\0';
    j++;                    // due to having n commas and n+1 values.

    for(i=0; i < j/4; i++)
    {
        dat->tc[i] = atof(fields[4*i]);
        dat->th[i] = atof(fields[4*i+1]);
        dat->status[i] = atoi(fields[4*i+2]);
        dat->unit[i] = fields[4*i+3][0];
        dat->valid[i] = true;
    }
    
    return 0;
}

// Assumes data derived from protocol TLOGA is available in the thermocouple data structure.
// Prints protocol identifier, followed by hot junction temperature and unit for all available channels.
// Returns 0 on success, -1 if thermocouple data structure does not contain data derived from TLOGA. 
int print_tloga(struct tc_data *dat)
{
    int i;
    
    if( dat->protocol != PROTOCOL_TLOGA ) { return -1; }

    printf("TLOGA\t");
    for(i=0; i<4; i++)
    {
        if (dat->valid[i] == true) { printf("%d->%0.4f%c\t",i, dat->th[i], dat->unit[i]); }
    }
    printf("\n");
    
    return 0;
}

// Assumes data derived from protocol TLOGB is available in the thermocouple data structure.
// Prints protocol identifier, followed by hot junction temperature and unit for all available channels.
// Returns 0 on success, -1 if thermocouple data structure does not contain data derived from TLOGB. 
int print_tlogb(struct tc_data *dat)
{
    int i;

    if( dat->protocol != PROTOCOL_TLOGB ) { return -1; }
    
    printf("TLOGB\t");
    for(i=0; i<4; i++)
    {
        if (dat->valid[i] == true) { printf("%d->%0.4f%c,%0.4f%c\t",i, dat->tc[i], dat->unit[i], dat->th[i], dat->unit[i]); }
    }
    printf("\n");
    
    return 0;
}

int main(int argc, char **argv)
{
    FILE *dev;              // file pointer to serial device.
    struct tc_data dat;     // thermocouple data structure
    // variables required for reading file data.
    char *c = NULL; 
    char line[128];

    if (argc < 2) 
    { 
        printf("Error: Incorrect usage. ./logger_simple <serial-device>\r\n"); 
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