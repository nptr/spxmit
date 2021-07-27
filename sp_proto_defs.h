 #ifndef H_SP_PROTO_DEFS
 #define H_SP_PROTO_DEFS 

#include <stdint.h>

#define ERR_SUCCESS 0
#define ERR_BAD_ID 1
#define ERR_BAD_UNIT 2
#define ERR_BAD_STATE 4
#define ERR_BAD_MODE 8

typedef struct
{
    int start;  // start level: 0: low, 1: high
    uint16_t* pulses;
    int numPulses;
} Command;

typedef struct 
{
    int id;     // device id
    int unit;   // switch number (1..X)
    int mode;   // 0: single, 1: all
    int state;  // 0: off, 1: on
} Arguments;

typedef Command (*buildCommandFunction)(const Arguments*);
typedef int (*validateArgumentsFunction)(const Arguments*);

typedef struct
{
    char compatible[64];
    buildCommandFunction buildCommand;
    validateArgumentsFunction validateArgs;
} Protocol;

#endif