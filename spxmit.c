#include <time.h>
#include <getopt.h>

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "sp_gpio.h"
#include "sp_proto_defs.h"
#include "sp_proto_intertechno.h"

#define GPIO_DEVICE "/dev/gpiochip0"
#define NUM_REPEATS 3


static Protocol protocols[] =
{
    { "arctech,intertechno,kaku", &intertechno_command, &intertechno_validate_args },
    { "", NULL, NULL }
};


void transmit(const Command* cmd, int fd, int rep)
{
    for(int n = 0; n < rep; ++n)
    {
        int state = cmd->start;
        for(int i = 0; i < cmd->numPulses; ++i)
        {
            spxmit_io_setpin(fd, state);
            state = !state;

            struct timespec ts;
            ts.tv_sec = 0;
            ts.tv_nsec = 1000 * cmd->pulses[i];
            nanosleep(&ts, NULL);
        }
    }

    spxmit_io_setpin(fd, !cmd->start);
}


Protocol* find_protocol(const char* name)
{
    Protocol* p = protocols;
    while(p->buildCommand)
    {       
        if(strstr(p->compatible, name) != NULL)
        {
            return p;
        }
        ++p;
    }
    return NULL;
}


void help()
{
    printf("spxmit - single pin transmit\n");
    printf("Synopsis: \n\n");
    printf("  spxmit -t 14 -p kaku -i 5652480 -u 1 -s 1\n\n");
    printf("  -t  <tx-pin>     pin number on chip for TX\n");
    printf("  -p  <protocol>   protocol name\n");
    printf("  -i  <id>         device id\n");
    printf("  -u  <unit>       unit number\n");
    printf("  -s  <state>      0=inactive, 1=active\n");
    printf("  -m  <mode>       0=single, 1=all\n");
    printf("  -h               this help\n");
}


int main(int argc, char** argv)
{
    Protocol* proto = NULL;
    Arguments args = { 
        .id = -1, 
        .unit = -1, 
        .state = -1, 
        .mode = 0 // valid default
    };

    int pin = -1;
    int has_p = 0;
    while(1)
    {
        int result = getopt(argc, argv, "t:p:i:u:s:m:h");
        if (result == -1) break;
        switch (result)
        {
            case 't': pin = atoi(optarg); break;
            case 'p': {
                if(!(proto = find_protocol(optarg)))
                {
                    printf("No matching protocol for '%s'\n", optarg);
                }
                has_p = 1;
            } break;
            case 'i': args.id = atoi(optarg); break;
            case 'u': args.unit = atoi(optarg); break;
            case 's': args.state = atoi(optarg) ? 1 : 0; break;
            case 'm': args.mode = atoi(optarg) ? 1 : 0; break;
            case 'h': help(); return 0;
        }
    }

    if(pin < 0)
        printf("Bad or missing option -t <tx-pin>\n");
    if(has_p == 0)
        printf("Missing option -p <protocol>\n");
    if((pin < 0) || (has_p == 0) || !proto)
    {
        return 1;
    }

    int err = proto->validateArgs(&args);
    if(err & ERR_BAD_ID)
        printf("Bad or missing option -i <id>\n");
    if(err & ERR_BAD_UNIT)
        printf("Bad or missing option -u <unit>\n");
    if(err & ERR_BAD_STATE)
        printf("Bad or missing option -s <state>\n");
    if(err & ERR_BAD_MODE)
        printf("Bad or missing option -m <mode>\n");
    if(err)
    {
        return 1;
    }

    int fd = spxmit_io_request(GPIO_DEVICE, pin);
    if(fd < 0)
    {
        printf("Could not request pin %d on '%s'\n", pin, GPIO_DEVICE);
        return 1;
    }

    Command cmd = proto->buildCommand(&args);
    transmit(&cmd, fd, NUM_REPEATS);
    free(cmd.pulses);
    spxmit_io_close(fd);
    return 0;
}