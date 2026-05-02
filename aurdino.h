#ifndef __AURDINO__H__
#define __AURDINO__H__
#include"types.h"
void pinmode(u32 pinno,u32 pindir);
void digitalwrite(u32 pinno,u32 bit);
u32 digitalread(u32 pinno);
void portmode(u32 pinstartno,u32 npins,u32 pindir);
void write2pins(u32 pinstartno,u32 npins,u32 data);
u32 readpins(u32 pinstartno, u32 npins);
#endif
