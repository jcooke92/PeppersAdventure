#include <gb/gb.h>

void better_delay(UINT8 numloops)
{
    numloops = numloops / 8;
    UINT8 i;
    for(i = 0; i< numloops; i++) { wait_vbl_done(); }
}