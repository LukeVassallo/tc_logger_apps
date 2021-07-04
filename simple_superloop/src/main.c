/*
 * Luke Vassallo @ 2021/06/29 9:37
 * Simple program to read thermocouple data.
 * 
 * Make sure to configure the serial port prior to launching the application.
 * stty -F /dev/ttyS0 9600 cs8 -cstopb -parenb -ixon -echo
 *  9600 baud, 8 data bits, 1 stop bit, no parity, no hardware flow control, local echo disabled.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 128


#define PROTOCOL_TLOGA  0
#define PROTOCOL_TLOGB  1

struct tc_data {
    uint8_t protocol;
    float   tc[4];
    float   th[4];
    char    unit[4];
    uint8_t status[4];
    bool    valid[4];       // true if data is valid, false otherwise. Used to be able to parse data from both 2/4 channel devices in a unified manner.
};
    

// removes CRLF, if present in the string.
// string must be NULL ('\0') terminated.
// returns 0 on success, -1 if \r\n is not encountered.
int strip_crlf( char *s );
int parse_tloga(char *s, struct tc_data *dat);
int parse_tlogb(char *s, struct tc_data *dat);


int strip_crlf( char *s )
{
    int len;
    len = strlen(s);
    
    if(s[len-2] == '\r' && s[len-1] == '\n')    // equality operator '==' has a higher precedence than logical and '&&' operator.
    {
        s[len-2] = '\0';
        return 0;
    }
    
    return -1;
}

#define MAX_FIELDS          16
#define MAX_FIELD_LENGTH    8

int parse_string(char *s, struct tc_data *dat)
{
    char tmp[64];
    int i;
    i = 0;
    
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

int print_tloga(struct tc_data *dat)
{
    int i;
    
    printf("TLOGA\t");
    for(i=0; i<4; i++)
    {
        if (dat->valid[i] == true) { printf("%d->%0.4f%c\t",i, dat->th[i], dat->unit[i]); }
    }
    printf("\n");
    
    return 0;
}

int print_tlogb(struct tc_data *dat)
{
    int i;
    
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
    FILE *dev;

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

    char *c = NULL; 
    char line[128];
    struct tc_data dat;
    while(1)
    {
        c = fgets(line, MAX_LINE_LENGTH, dev);
        if ( c != NULL )
        {
            //strip_crlf(line);
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
        
