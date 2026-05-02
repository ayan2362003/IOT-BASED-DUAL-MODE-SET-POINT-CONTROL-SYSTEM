 #include"types.h"
void delay_us(u32 delay_us)
{
   delay_us*=12;
   while(delay_us--);
}
void delay_ms(u32 delay_ms)
  {
      delay_ms*=12000;
	  while(delay_ms--);
  }
void delay_s(u32 delay_s)
  {
      delay_s*=12000000;
			  while(delay_s--);
  }

