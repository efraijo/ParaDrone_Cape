/*Interrupt service functions*/

#ifndef TASK_H
#define TASK_H

// sample sensors, triggered by IMU external interrupt. 
int task_sample_imu();
// Robotic cape button interrupt. Toggle logging enable/disable.
int task_pause_unpress();
#endif /*TASK_H*/