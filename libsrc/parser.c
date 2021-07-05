/*
 * Luke Vassallo @ 2021/07/05 9:37
 * XLCn (XLC2 / XLC4) thermocouple data.
 */

#include "parser.h"

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
