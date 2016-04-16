#include <stdio.h>

#include "menu.h"
#include "motor.h"
#include "VirtualSerial.h"

/* Welp. This table idea didn't pan out because C. I"m leaving in this comment as a reference:
 *   {"r", "Set reference position", set_reference_position, INT_ARG},
 *   {"P", "increase K_p", increase_kp, INT_ARG},
 *   {"p", "decrease K_p", decrease_kp, INT_ARG},
 *   {"D", "increase K_d", increase_kd, INT_ARG},
 *   {"d", "decrease K_d", decrease_kd, INT_ARG},
 *   {"v", "View parameters", print_params, NO_ARGS},
 *   {"t", "Execute trajectory", execute, NO_ARGS}
 */

/* Forward declarations */
void set_reference_position();
void increase_kp(pid_gains_t * gains);
void decrease_kp(pid_gains_t * gains);
void increase_kd(pid_gains_t * gains);
void decrease_kd(pid_gains_t * gains);
void print_params(pid_gains_t * gains);
void execute();
void print_help();
void reset_e();

/* Public Functions */
void menu_init()
{

}

void run_menu(pid_gains_t * gains)
{
    char command = 0xff;
    uint8_t done = 0;

    print_help();
    USB_Mainloop_Handler();
    while (!done)
    {
        char c = -1;
        printf("Command > ");
        USB_Mainloop_Handler();
        while (c == -1)
            c = fgetc(stdin);

        printf("%c\r\n", c);
        USB_Mainloop_Handler();
        command = c;


            switch (command)
            {
                case 'r':
                    set_reference_position();
                    break;

                case 'P':
                    increase_kp(gains);
                    break;

                case 'p':
                    decrease_kp(gains);
                    break;

                case 'D':
                    increase_kd(gains);
                    break;

                case 'd':
                    decrease_kd(gains);
                    break;

                case 'v':
                    print_params(gains);
                    break;

                case 't':
                    execute();
                    break;

                case 'q':
                    reset_e();
                    break;

                case 'h':
                    print_help();
                    break;

                default:
                    printf("Not a valid menu option: %x!\r\n", command);
                    break;
            }
    }
}


/* Not public functions */
void set_reference_position()
{
    int16_t setpoint;
    setpoint = get_setpoint();
    set_setpoint(setpoint + 400);
}

void increase_kp(pid_gains_t * gains)
{
    int16_t gain;
    gain = gains->kp;
    gains->kp = gain + 1;
}

void decrease_kp(pid_gains_t * gains)
{
    int16_t gain;
    gain = gains->kp;
    gains->kp = gain - 1;
}

void increase_kd(pid_gains_t * gains)
{
    int16_t gain;
    gain = gains->kd;
    gains->kd = gain + 1;
}

void decrease_kd(pid_gains_t * gains)
{
    int16_t gain;
    gain = gains->kd;
    gains->kd = gain - 1;
}

void print_params(pid_gains_t * gains)
{
    printf("------------------------------\r\n");
    printf("Gains are:\r\n");
    printf("Kp: %d\r\n", gains->kp);
    printf("Kd: %d\r\n", gains->kd);
    printf("Ki: %d\r\n", gains->ki);
    printf("Setpoint: %d\r\n", get_setpoint());
    printf("Torque setting: %ld\r\n", get_torque());
    printf("Error: %ld\r\n", get_error());
    printf("Motor running? %s\r\n", is_motor_running() ? "Yes" : "No");
    printf("------------------------------\r\n");
}

void execute()
{
    run_motor();
}

void reset_e()
{
    reset_position();
}

void print_help()
{
    printf("------------------------------\r\n");
    printf("Commands:\r\n");
    printf("r Set reference position\r\n");
    printf("P increase K_p\r\n");
    printf("p decrease K_p\r\n");
    printf("D increase K_d\r\n");
    printf("d decrease K_d\r\n");
    printf("v View parameters\r\n");
    printf("t Execute trajectory\r\n");
    printf("q reset encoder\r\n");
}
