/*
 * Luke Vassallo @ 2021/07/05 9:37
 * XLCn (XLC2 / XLC4) thermocouple data.
 */

#ifndef DATA_H_
#define DATA_H_

struct tc_data {
    uint8_t protocol;       // Identifies the protocol. 0 = TLOGA, 1 = TLOGB, anything else is not valid.
    float   tc[4];          // Cold Junction temperature
    float   th[4];          // Hot Junction temperature
    char    unit[4];        // Temperature unit (C / F / K)
    uint8_t status[4];      // Channel status
    bool    valid[4];       // True if data at index is valid, false otherwise. Used to be able to parse data from both 2/4 channel devices in a unified manner.
};

#endif /* DATA_H_ */
