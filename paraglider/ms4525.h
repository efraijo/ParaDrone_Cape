/*Speed sensor driver. unfinished*/
#ifndef MS4525_H
#define MS4525_H

#define MS4525_CHANNEL 2 //I2C1 map to /dev/i2c-2
#define MS4525_DEVICE_ADDR (0x28)
#define MS4525_REG_ADDR	(0x51)

#define MS4525_STAT_NORMAL (0)
#define MS4525_STAT_STALE  (2)
#define MS4525_STAT_FAULT  (3)

long ms4525_press_raw;
long ms4525_temp_raw;


int ms4525_init();
int ms4525_read_press();
int ms4525_read_all();

#endif /*MS4525_H*/