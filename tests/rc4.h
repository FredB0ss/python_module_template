/*******************************************************************************
   Module      : RC4
   Author      : fred
   Creation    : 2019-10-06 20:54
   Description : Module API

*******************************************************************************/
#ifndef RC4_H
#define RC4_H

typedef struct
{      
     unsigned char state[256];       
     unsigned char x;        
     unsigned char y;
} rc4_ctxt_t;

void rc4_init(unsigned char *key, size_t key_len, rc4_ctxt_t *p_ctxt);
unsigned char rc4_next(rc4_ctxt_t *p_ctxt);


#endif /* RC4_H */
/*EOF**************************************************************************/
