#ifndef _motor_h
#define _motor_h
#include <inttypes.h>


/* Types and constants */
typedef struct {
    int8_t kp;
    int8_t ki;
    int8_t kd;
} pid_gains_t;


/* Functions */
int16_t get_position();
void update_position(int8_t val);
void motor_init();
void reset_position();
void set_setpoint(int16_t setpoint);

int16_t update_pid(pid_gains_t * pid_gains);
void update_gains(int8_t kp, int8_t kd, int8_t ki, pid_gains_t * gains);

#endif
