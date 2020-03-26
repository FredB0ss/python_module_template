/*******************************************************************************

   Module      : RC4
   Author      : fred
   Creation    : 2019-10-06 20:52
   Description : Module Implementation

Derived from http://cypherpunks.venona.com/archive/1994/09/msg00304.html
Thanks to an hint from David: https://github.com/DavidBuchanan314/rc4

*******************************************************************************/

#include <stddef.h>

#include "rc4.h"

static void swap_byte(unsigned char *a, unsigned char *b)
{
    unsigned char tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

/*******************************************************************************
 Public Functions
*******************************************************************************/

void rc4_init(unsigned char *key, size_t key_len, rc4_ctxt_t *p)
{
    unsigned char* state;
    unsigned i;
    unsigned j;

    state = p->state;
    for(i = 0; i < 256; i++) {
        state[i] = i;
    }

    j = 0;
    for(i = 0; i < 256; i++)
    {
        j = (state[i] + key[i % key_len] + j) & 0xff;

        swap_byte(&state[i], &state[j]);
    }

    p->x = 0;
    p->y = 0;
}

unsigned char rc4_next(rc4_ctxt_t *p)
{
    unsigned char* state;
    unsigned char i;

    state = p->state;

    p->x = (p->x + 1) & 0xff;
    p->y = (state[p->x] + p->y) & 0xff;
    swap_byte(&state[p->x], &state[p->y]);

    i = (state[p->x] + state[p->y]) & 0xff;

    return state[i];
}


/*EOF**************************************************************************/
