/*gps driver*/
#ifndef EM506_H
#define EM506_H
// nmea protocol lib
#include <nmea/nmea.h>

// nmea gps data entry structure
nmeaINFO em506_info;

// init serial port and nmea phaser 
int em506_init(int channel);
// read from serial 
void em506_read();
// clean up before exit
void em506_cleanup();

#endif /*EM506_H*/