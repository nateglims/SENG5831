#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "VirtualSerial.h"
#include "time.h"
#include "led.h"
#include "globals.h"

#define F_CPU 16000000
#include <util/delay.h>

#define COMMAND_BUFF_LEN 64

uint32_t start_time;

/* Print the help menu */
void print_menu()
{
    printf("****************\r\n");
    printf("p : Print data collected for experiment, including job releases and missed deadlines.\r\n");
    printf("e # : Set­Up this experiment number (e.g. Set flags to activate/deactivate delays in ISRs).\r\n");
    printf("r # : Set the toggle period of the GREEN LED Task to # ms. (Check that value is in bound.)\r\n");
    printf("z : Reset all variables to set up for a new experiment.\r\n");
    printf("g : Go signal for start experiment. (Try to synchronize all activities when this signal is given).\r\n");
    printf("****************\r\n");
}

/* Set the counts to 0 */
void reset_vars(counts_t * counts)
{
    printf("Reset!\r\n");
    memset(counts, 0, sizeof(*counts));
}

/*  This sets the global experiment number. */
bool setup(uint8_t experiment_number)
{
    global_experiment = experiment_number;
    printf("Setup!\r\n");
    return true;
}

/* Set the period of the green LED */
bool set_green_period(uint16_t num)
{
    if (num > 0 && num < 1048)
    {
        printf("Setting new period for green LED.\r\n");
        led_set_green_led_period(num);
        return true;
    }
    else
    {
        printf("Unable to set new period for green LED. Please set a value between 1 and 1048 inclusive.\r\n");
        return false;
    }

    return 0;
}

/* ? */
void reinit()
{
    printf("Reinit!\r\n");
}

/* Print those counts and stuff */
void print_report(counts_t * counts, uint32_t the_time)
{
    uint32_t tenhz_tasks_expected = the_time/100; /* Oh No! Division :(. Time is in ms and 10hz things should execute every 100ms... */
    printf("-------- Task Execution Counts -----------\r\n");
    printf("[Red LED]\tExecuted: %u\tExpected: %lu\r\n", counts->red_led, tenhz_tasks_expected);
    printf("[Green LED]\tExecuted: %u\tExpected: %lu\r\n", counts->green_led, tenhz_tasks_expected);
    printf("[Yellow LED]\tExecuted: %u\tExpected: %lu\r\n", counts->yellow_led, tenhz_tasks_expected);
    printf("[Jitter LED]\tExecuted: %u\tExpected: %lu\r\n", counts->jitter_led, tenhz_tasks_expected);
    printf("[Hough Transform]\tExecuted: %u\tExpected: %lu\r\n", counts->hough, tenhz_tasks_expected);
    printf("Time elapsed: %lums\r\n", the_time);
}

/* Initializer funciton */
void menu_init()
{
    SetupHardware();
}

/* This is from the forum */
void handle_prompt(counts_t * counts) {
    int i;
    bool done = false;
    char command[COMMAND_BUFF_LEN];
    uint32_t current_time = get_time();
    print_menu();
    USB_Mainloop_Handler();
    while (!done) {
        printf("Command (h for help)> ");
        USB_Mainloop_Handler();
        for (i = 0; i < COMMAND_BUFF_LEN; i++) {
            char c = -1;
            while (c == -1)
                c = fgetc(stdin);
            if (c == '\b') {
                i -= 2;
                printf("\b \b");
                USB_Mainloop_Handler();
                continue;
            }
            else if (c == '\n') {
                i--;
                continue;
            }
            else if (c == '\r') {
                break;
            }
            printf("%c", c);
            USB_Mainloop_Handler();
            command[i] = c;
        }
        printf("\r\n");
        USB_Mainloop_Handler();
        if (i == COMMAND_BUFF_LEN) {
            printf("Buffer full\r\n");
            USB_Mainloop_Handler();
            continue;
        }
        command[i] = '\0';
        if (strlen(command) == 0) {
            continue;
        }
        USB_Mainloop_Handler();
        uint16_t num = 0;
        if (!strcmp(command, "h")) {
            print_menu();
        }
        else if (!strcmp(command, "p")) {
            print_report(counts, current_time - start_time);
        }
        else if (sscanf(command, "e %u", &num) == 1) {
            printf("Setting up experiment #%d\r\n", num);
            if (setup(num))
                printf("Setup done\r\n");
            else
                printf("Setup failed\r\n");
        }
        else if (sscanf(command, "r %u", &num) == 1) {
            if (!set_green_period(num))
                printf("Period out of bounds\r\n");
        }
        else if (!strcmp(command, "z")) {
            reset_vars(counts);
            current_time = 0;
            start_time = 0;
        }
        else if (!strcmp(command, "g")) {
            printf("Starting trial\r\n");
            start_time = get_time();
            enable_leds();
            done = true;
        }
        else {
            printf("Invalid command: %s\r\n", command);
        }
        USB_Mainloop_Handler();
    }
}
