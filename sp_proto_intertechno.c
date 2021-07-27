#include "sp_proto_intertechno.h"

#include <stdlib.h>

const uint16_t PULSE_SHORT = 200;           // will effectively be ~250us
const uint16_t PULSE_LONG = 1300;           // 
const uint16_t PULSE_LONG_START = 2800;     // 
const uint16_t PULSE_LONG_END = 11000;      // 


/**
 * Encode a bit as sequence of pulses. In this case, first pulse is high.
 * 1: |-|    |-| 
 *    | |____| |_ => short, long, short, short
 * 
 * 0: |-| |-| 
 *    | |_| |____ => short, short, short, long
 */
uint16_t* intertechno_pulses_from_bits(uint16_t* dst, uint32_t bits, int numBits)
{
    for(int i = numBits - 1; i >= 0; --i)
    {
        int state = bits & (1 << i);
        *dst++ = PULSE_SHORT;
        *dst++ = state ? PULSE_LONG : PULSE_SHORT;
        *dst++ = PULSE_SHORT;
        *dst++ = state ? PULSE_SHORT : PULSE_LONG;
    }

    return dst;
}


Command intertechno_command(const Arguments* args)
{
    const int SIZE = 132;

    Command cmd = { 0 };
    cmd.start = 1;
    cmd.numPulses = SIZE;
    cmd.pulses = malloc(SIZE * sizeof(uint16_t));

    uint16_t* dst = cmd.pulses;
    *dst++ = PULSE_SHORT;
    *dst++ = PULSE_LONG_START;
    dst = intertechno_pulses_from_bits(dst, args->id, 26);
    dst = intertechno_pulses_from_bits(dst, args->mode, 1);
    dst = intertechno_pulses_from_bits(dst, args->state, 1);
    dst = intertechno_pulses_from_bits(dst, args->unit, 4);
    *dst++ = PULSE_SHORT;
    *dst++ = PULSE_LONG_END;

    return cmd;
}


int intertechno_validate_args(const Arguments* args)
{
    int err = ERR_SUCCESS;
    if(0 > args->id || args->id > 67108863)
        err |= ERR_BAD_ID;
    if(0 > args->unit || args->unit > 15)
        err |= ERR_BAD_UNIT;
    if(0 > args->state )
        err |= ERR_BAD_STATE;

    return err;
}