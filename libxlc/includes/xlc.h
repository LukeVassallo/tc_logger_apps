/*
 * Luke Vassallo @ 2021/07/05 9:37
 * XLCn (XLC2 / XLC4) thermocouple data.
 */

#ifndef XLC_H_
#define XLC_H_

/* includes */
#include "xlc.h"
#include "data.h"

/* Definitions */
#define MAX_FIELDS          16      // TLOGB has 16 fields after, the protocol identifier.
#define MAX_FIELD_LENGTH    12      // Floating point numerals are printed using 10 characters, such as "?XXXX.yyyy" (worst case scenario). 

#define PROTOCOL_TLOGA  0
#define PROTOCOL_TLOGB  1

/* Function Prototypes */
int parse_string(char *s, struct tc_data *dat); // Parses a <CR><LF> terminated string as received from an XLCn device. This can be a command or data string.
int parse_tloga(char *s, struct tc_data *dat);  // Parses a <CR><LF> terminated string with protocol identifier removed. i.e. "$TLOGA," is not passed to the function, only the remainder.
int parse_tlogb(char *s, struct tc_data *dat);  // Parses a <CR><LF> terminated string with protocol identifier removed. i.e. "$TLOGB," is not passed to the function, only the remainder.
int print_tloga(struct tc_data *dat);           // Prints data to standard output, assuming structure contains data parsed from TLOGA
int print_tlogb(struct tc_data *dat);           // Prints data to standard output, assuming structure contains data parsed from TLOGB

#endif /* XLC_H_ */
