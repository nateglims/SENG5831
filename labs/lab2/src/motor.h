#ifndef _motor_h
#define _motor_h
#include <inttypes.h>


/* Types and constants */
typedef struct {
    int16_t kp;
    int16_t ki;
    int16_t kd;
} pid_gains_t;


/* Functions */
int16_t get_position();
void update_position(int8_t val);
void motor_init();
void reset_position();
void set_setpoint(int16_t setpoint);
uint16_t get_setpoint();
int32_t get_error();
int32_t get_torque();
void run_motor();
uint8_t is_motor_running();
void print_motor_stats_for_interpolator();

int16_t update_pid(pid_gains_t * pid_gains);
void update_gains(int16_t kp, int16_t kd, int16_t ki, pid_gains_t * gains);

#endif
