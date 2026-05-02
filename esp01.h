#ifndef ESP01_H
#define ESP01_H

void esp01_connectAP(void);
void esp01_sendToThingspeak(float val);
void esp01_sendToThingspeak_set_point(float val);
void esp01_readFromThingspeak(void);

#endif
