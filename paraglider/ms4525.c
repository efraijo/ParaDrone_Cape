/*Speed sensor driver. unfinished*/
#include "ms4525.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int i2c2_fd;
unsigned char txBuff[10];

int _delay_ms_2(unsigned long num_ms)
{
	struct timespec ts;

	ts.tv_sec = num_ms / 1000;
	ts.tv_nsec = (num_ms % 1000) * 1000000;

	return nanosleep(&ts, NULL);
}


int _i2c_write_2(unsigned char slave_addr, unsigned char reg_addr,
       unsigned char length, unsigned char const *data)
{
	int result, i;

	if (length == 0) {
		result = write(i2c2_fd, &reg_addr, 1);

		if (result < 0) {
			perror("write:1");
			return result;
		}
		else if (result != 1) {
			printf("Write fail:1 Tried 1 Wrote 0\n");
			return -1;
		}
	}
	else {
		txBuff[0] = reg_addr;

		for (i = 0; i < length; i++)
			txBuff[i+1] = data[i];

		result = write(i2c2_fd, txBuff, length + 1);

		if (result < 0) {
			perror("write:2");
			return result;
		}
		else if (result < (int)length) {
			printf("Write fail:2 Tried %u Wrote %d\n", length, result); 
			return -1;
		}
	}

	return 0;
}

int _i2c_read_2(unsigned char slave_addr, unsigned char reg_addr,
       unsigned char length, unsigned char *data)
{
	int tries, result, total;

	if (_i2c_write_2(slave_addr, reg_addr, 0, NULL))
		return -1;

	total = 0;
	tries = 0;

	while (total < length && tries < 5) {
		result = read(i2c2_fd, data + total, length - total);

		if (result < 0) {
			perror("read");
			break;
		}

		total += result;

		if (total == length)
			break;

		tries++;		
		_delay_ms_2(10);
	}

	if (total < length)
		return -1;

	return 0;
}



int ms4525_init()
{
	char portf[32];
	sprintf(portf,"/dev/i2c-%d",MS4525_CHANNEL);
	printf("%s \n",portf);
	i2c2_fd = open(portf,O_RDWR);
	if(i2c2_fd ==-1)
	{
		perror("Open(I2C1) ");
		printf("Cannot opent port /dev/i2c-%d",MS4525_CHANNEL);
		return -1;
	}
	
	
	if (ioctl(i2c2_fd, I2C_SLAVE, MS4525_DEVICE_ADDR) < 0) {
		perror("ioctl(I2C_SLAVE)");
		printf("Cannot find speed sensor\n");
		return -1;
	}
	printf("SS found!\n");
	return 0;
}

int ms4525_read_press()
{
	unsigned char data[2];
	_i2c_read_2(MS4525_DEVICE_ADDR,MS4525_REG_ADDR,2,data);
	_delay_ms_2(5);
	return 0;
}



int ms4525_read_all()
{
	unsigned char data[4];
	_i2c_read_2(MS4525_DEVICE_ADDR,MS4525_REG_ADDR,4,data);
	_delay_ms_2(5);
	return 0;
}