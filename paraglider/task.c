/*Interrupt service functions*/

#include <robotics_cape.h>
#include "global.h"
#include "task.h"
#include "log.h"
#include "bmp180.h"
#include "em506.h"

// sample time counter
volatile int task_sample_counter = 0;
// red led state
volatile int task_led_red = 0;
// green led state
volatile int task_led_grn = 0;
// BMP180 temperature raw reading
volatile long bmp_temp_raw = 0;
// BMP180 pressure raw reading
volatile long bmp_press_raw = 0;
// BMP180 temperature engineer unit value
volatile long bmp_temp = 0;
// BMP180 pressure engineer unit value
volatile long bmp_press = 0;
// current system time
unsigned long time_ms;


int task_sample_imu()
{
	mpudata_t mpu; //struct to read IMU data into
	memset(&mpu, 0, sizeof(mpudata_t)); //make sure it's clean before starting
	// sampling imu
	if (mpu9150_read(&mpu) == 0) {
		// get timestamp
		linux_get_ms(&time_ms);
		
		// printout for debug reason
		/*printf("\r");
		printf("X: %0.1f Y: %0.1f Z: %0.1f ",
		mpu.fusedEuler[VEC3_X] * RAD_TO_DEGREE, 
		mpu.fusedEuler[VEC3_Y] * RAD_TO_DEGREE, 
		mpu.fusedEuler[VEC3_Z] * RAD_TO_DEGREE);
		
		printf("Xg: %05d Yg: %05d Zg: %05d ",
		mpu.rawGyro[VEC3_X], 
		mpu.rawGyro[VEC3_Y], 
		mpu.rawGyro[VEC3_Z]);  
		
		printf("Xa: %05d Ya: %05d Za: %05d ",
		mpu.calibratedAccel[VEC3_X], 
		mpu.calibratedAccel[VEC3_Y], 
		mpu.calibratedAccel[VEC3_Z]);
		
		delay_ms(100);*/
		
		// sampling barometer
		bmp_temp_raw = bmp_read_temp();
		bmp_press_raw = bmp_read_press();
		// printf("TEMP raw: %li PRESS raw: %li",bmp_temp_raw,bmp_press_raw);
		bmp_temp = bmp_calc_temp(bmp_temp_raw);
		bmp_press = bmp_calc_press(bmp_press_raw);
		// printf("TEMP 0.1C: %li PRESS PA: %li",bmp_temp,bmp_press);
		
		// fflush(stdout);
		
		// update logging entry
		log_buffer.time = time_ms;
		log_buffer.pitch	= -mpu.fusedEuler[VEC3_X];
		log_buffer.roll =  mpu.fusedEuler[VEC3_Y];
		log_buffer.yaw 	=  mpu.fusedEuler[VEC3_Z];
		
		log_buffer.xa	= mpu.calibratedAccel[VEC3_X];
		log_buffer.ya =  mpu.calibratedAccel[VEC3_Y];
		log_buffer.za	=  mpu.calibratedAccel[VEC3_Z];
		
		log_buffer.xg	= mpu.rawGyro[VEC3_X];
		log_buffer.yg =  mpu.rawGyro[VEC3_Y];
		log_buffer.zg	=  mpu.rawGyro[VEC3_Z];
		
		log_buffer.temperature_raw = bmp_temp_raw;
		log_buffer.pressure_raw = bmp_press_raw;
		log_buffer.temperature = bmp_temp;
		log_buffer.pressure = bmp_press;
		
		// log write imu data
		if(log_flag!=0)
			log_write_entry(&log_buffer);
		
		task_sample_counter++;
		
		// service dsm channel
		if(is_new_dsm2_data())
		{					
			// logging raw dsm input
			/* log_buffer_r.time = time_ms;
			log_buffer_r.Thr = get_dsm2_ch_raw(1);
			log_buffer_r.Roll = get_dsm2_ch_raw(2);
			log_buffer_r.Pitch = get_dsm2_ch_raw(3);
			log_buffer_r.Yaw = get_dsm2_ch_raw(4);
			log_buffer_r.Kill = get_dsm2_ch_raw(5);
			log_buffer_r.Mode = get_dsm2_ch_raw(6);
			log_buffer_r.Aux1 = get_dsm2_ch_raw(7);
			log_buffer_r.Aux2 = get_dsm2_ch_raw(8);
			log_buffer_r.Aux3 = get_dsm2_ch_raw(9);
			 */
			 
			// logging normalized dsm input 
			/* log_buffer_r.Thr = get_dsm2_ch_normalized(1);
			log_buffer_r.Roll = get_dsm2_ch_normalized(2);
			log_buffer_r.Pitch = get_dsm2_ch_normalized(3);
			log_buffer_r.Yaw = get_dsm2_ch_normalized(4);
			log_buffer_r.Kill = get_dsm2_ch_normalized(5);
			log_buffer_r.Mode = get_dsm2_ch_normalized(6);
			log_buffer_r.Aux1 = get_dsm2_ch_normalized(7);
			log_buffer_r.Aux2 = get_dsm2_ch_normalized(8);
			log_buffer_r.Aux3 = get_dsm2_ch_normalized(9); */
			
			// printout for debug
			/* printf("\r");
			printf("%0.2f   ", log_buffer_r.Thr);
			printf("%0.2f   ", log_buffer_r.Roll);
			printf("%0.2f   ", log_buffer_r.Pitch);
			printf("%0.2f   ", log_buffer_r.Yaw);
			printf("%0.2f   ", log_buffer_r.Kill);
			printf("%0.2f   ", log_buffer_r.Mode);
			printf("%0.2f   ", log_buffer_r.Aux1);
			printf("%0.2f   ", log_buffer_r.Aux2);
			printf("%0.2f   ", log_buffer_r.Aux3);
			fflush(stdout); */
			
			// passing through dsm message. NOTE: too slow. moved to idle loop
			/* send_servo_pulse_us(1,log_buffer_r.Thr);
			send_servo_pulse_us(2,log_buffer_r.Roll);
			send_servo_pulse_us(3,log_buffer_r.Pitch);
			send_servo_pulse_us(4,log_buffer_r.Yaw);
			send_servo_pulse_us(5,log_buffer_r.Kill);
			send_servo_pulse_us(6,log_buffer_r.Mode);
			send_servo_pulse_us(7,log_buffer_r.Aux1);
			send_servo_pulse_us(8,log_buffer_r.Aux2);
			send_servo_pulse_us(9,log_buffer_r.Aux3); */
			
			// log write dsm message
			if(log_flag!=0)
				log_write_entry_r(&log_buffer_r);
		}
		
		// update GPS logging entry
		{
			log_buffer_g.year = em506_info.utc.year;
			log_buffer_g.mon = em506_info.utc.mon;
			log_buffer_g.day = em506_info.utc.day;
			log_buffer_g.hour = em506_info.utc.hour;
			log_buffer_g.min = em506_info.utc.min;
			log_buffer_g.sec = em506_info.utc.sec;
			log_buffer_g.hsec = em506_info.utc.hsec;
			log_buffer_g.lat = em506_info.lat;
			log_buffer_g.lon = em506_info.lon;
			log_buffer_g.elv = em506_info.elv;
			log_buffer_g.speed = em506_info.speed;
		}
		
		// toggle led
		if(task_sample_counter%SAMPLE_HZ==0)
		{
			task_led_red ^=1;
			task_led_grn ^=1;
			task_led_grn &=log_flag;
			setGRN(task_led_grn);
			setRED(task_led_red);
			task_sample_counter = 0;	
			//em506_read();
			if(log_flag!=0)
				log_write_entry_g(&log_buffer_g);
		}

		
	}
	return 0; 
}

// toggle logging enable/disable when button pressed
int task_pause_unpress()
{
	log_flag^=1;
	return 0;
}