#ifndef __LCD_H__
#define __LCD_H__
#include "types.h"
void init_lcd(void);
void cmdlcd(u8 cmdbyte);
void writelcd(u8 byte);
void charlcd(u8 asciival);
void strlcd(s8 *p);
void u32lcd(u32 n);
void s32lcd(s32 n);
void f32lcd(f32 fn,u32 ndp);
void buildcgram(u8 *p,u8 nbytes);
#endif
