/*main*/

#include "global.h"
#include <robotics_cape.h>
#include "log.h"
#include "task.h"
#include "bmp180.h"
#include "em506.h"
//#include "ms4525.h"

int main()
{
	/////////// Initialization////////////////////
	// initial cape
	if(initialize_cape()<0){
		return -1;
	}
	
	// initial logging file
	if(log_start()<0){
		return -1;
	}
	log_flag = 0;
	// set interrupt callback
	set_pause_unpressed_func(&task_pause_unpress);
	
	// initial barometer
	printf("\nInitializing BAROMETER\n");
	if(bmp_init()<0)
	{
		printf("Barometer initialization failed!\n");
		cleanup_cape();
		return -1;
	}
	printf("DONE!\n");
	
	// initial GPS
	printf("\nInitializing GPS\n");
	if(em506_init(2)<0)
	{
		printf("GPS initialization failed!\n");
		cleanup_cape();
		return -1;
	}
	printf("DONE!\n");

	// printf("\nInitializing SPEED SENSOR\n");
	// if(ms4525_init()<0)
	// {
		// printf("SPEED SENSOR initialization failed!\n");
		// cleanup_cape();
		// return -1;
		
	// }
	
	// initial dsm radio channel
	if(initialize_dsm2()){
		// if init returns -1 if there was a problem 
		// most likely no calibration file found
		printf("run calibrate_dsm2 first\n");
		return -1;
	}
	
	
	/////////// StartThreads///////////////////////
	signed char orientation[9] = ORIENTATION_FLAT;
	if(initialize_imu(SAMPLE_HZ, orientation)){
		printf("IMU initialization failed, please reboot\n");
		cleanup_cape();
		return -1;
	}
	set_imu_interrupt_func(&task_sample_imu);

		

	
	/////////// Main loop//////////////////////////
	while(get_state()!=EXITING)
	{
		usleep(20000);
		if(is_new_dsm2_data())
		{
			// passing through dsm message. NOTE: channel mapping might be different from device to device.
			log_buffer_r.Thr = get_dsm2_ch_raw(1);
			log_buffer_r.Roll = get_dsm2_ch_raw(2);
			log_buffer_r.Pitch = get_dsm2_ch_raw(3);
			log_buffer_r.Yaw = get_dsm2_ch_raw(4);
			log_buffer_r.Kill = get_dsm2_ch_raw(5);
			log_buffer_r.Mode = get_dsm2_ch_raw(6);
			log_buffer_r.Aux1 = get_dsm2_ch_raw(7);
			log_buffer_r.Aux2 = get_dsm2_ch_raw(8);
			log_buffer_r.Aux3 = get_dsm2_ch_raw(9);
			send_servo_pulse_us(1,log_buffer_r.Thr);
			send_servo_pulse_us(2,log_buffer_r.Roll);
			send_servo_pulse_us(3,log_buffer_r.Pitch);
			send_servo_pulse_us(4,log_buffer_r.Yaw);
			send_servo_pulse_us(5,log_buffer_r.Kill);
			send_servo_pulse_us(6,log_buffer_r.Mode);
			
			// printf("\r");
			// printf("%0.2f   ", log_buffer_r.Thr);
			// printf("%0.2f   ", log_buffer_r.Roll);
			// printf("%0.2f   ", log_buffer_r.Pitch);
			// printf("%0.2f   ", log_buffer_r.Yaw);
			// printf("%0.2f   ", log_buffer_r.Kill);
			// printf("%0.2f   ", log_buffer_r.Mode);
			// printf("%0.2f   ", log_buffer_r.Aux1);
			// printf("%0.2f   ", log_buffer_r.Aux2);
			// printf("%0.2f   ", log_buffer_r.Aux3);
			// fflush(stdout);
		}
		// Service GPS chip
		em506_read();
	}
	
	/////////// Exit//////////////////////////////
	setGRN(0);
	setRED(0);
	cleanup_cape();
	log_stop();
	em506_cleanup();
	return 0;
}