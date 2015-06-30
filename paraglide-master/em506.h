#ifndef EM506_H
#define EM506_H

#include <nmea/nmea.h>

nmeaINFO em506_info;

int em506_init(int channel);
void em506_read();
void em506_cleanup();

#endif /*EM506_H*/