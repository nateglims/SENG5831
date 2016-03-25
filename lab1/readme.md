# Lab 1

## Cloning
Nothing special.

## Building
1. Edit the port at the top of the makefile.
2. `make clean all program`

## Questions
1. Describe your method for WCET analysis of the Hough Transform and report
   your findings. How confident are you in those results? Briefly explain.
   What factors influence the accuracy of a WCET analysis?

To measure the WCET of the Hough Transform, first I made an assumption that given
a fixed input, the algorithm would take roughly the same amount of time. I then
loaded the hough transform code, disabled all interrupts, and set and cleared a
GPIO before and after the hough transform executed. I initially measured an
execution time of 220ms and had to decrease the size of the input file. By
dropping it to 6x6, the timing dropped to 49ms. Interesting side note, if I
enabled videos

I am very confident in the results. The execution time was very consistent. As
for if it is actually the "Worst" execution case, I would be more concerned if
it had a dynamic input.

Factors that could influence the accuracy of this method include hardware
factors such as clock speed.

2. Define the period for the Hough Transform, RED LED, JITTER LED, and YELLOW
   LED tasks. If you can, provide an exact unit of time with a brief
   justification, otherwise describe the factors that influence the period of
   the task.

The Hough transform is released every 10hz which would give it a period of
100ms.
The Red LED has a period of... 100ms.
Yellow has a period of 100ms.
Jitter LED has a period of 1/40hz = 25ms.

3. In experiment #1,
   a. Did you observe any “drift” in the blinking of the LEDs, meaning they
      were in sync but then one seemed to blink slightly slower or faster than
      the other? Briefly explain how drift can occur.
   b. Were the LEDs synchronized at the start, meaning were they all on, then
      all off at the exact same time? Describe the factors that influence the
      ability to synchronize these events.

Yes, the LEDs drifted over time. Drift can occur in several ways. Some timers
that use pre-scalers have a wider window (the size of the prescaler). The PWM
led (green) also likely does not have the instruction overhead that the other
timer ISR led (yellow) has. Also the clock could be blocked by the other two
ISRs and cause the red LED to drift even if it hits all of it's deadlines.

The LEDs were synchronized at the beginning (other than the jitter). They were
synchronized because they were all running at the same speed (2 or 10hz) and
starting with the same state at system startup. If they all used a common format
(all in an ISR for example), future synchronization events would be easier.

4. Using the data collected in the above experiments, describe the behavior
   for each of the experiments #2 through #7, including number of job releases,
   missed deadlines, and expected number of releases. Explain the reason for
   the behavior for each of the experiments #2 through #7 (you can discuss each
   individually or describe them collectively).

For each, I executed the experiment, counted to 10 and interrupted it with a button press.
Experiment 2:
-------- Task Execution Counts -----------
[Red LED]       Executed: 28    Expected: 106
[Green LED]     Executed: 118   Expected: 106
[Yellow LED]    Executed: 119   Expected: 106
[Jitter LED]    Executed: 5     Expected: 106
[Hough Transform]       Executed: 60    Expected: 106
Time elapsed: 10696ms

The hough transform and red LED clearly are missing deadlines. The global time also missed some deadlines as shown by the yellow and green LED exceeding their expected release count.

Experiment 3:
-------- Task Execution Counts -----------
[Red LED]       Executed: 0     Expected: 25
[Green LED]     Executed: 106   Expected: 25
[Yellow LED]    Executed: 105   Expected: 25
[Jitter LED]    Executed: 1     Expected: 25
[Hough Transform]       Executed: 18    Expected: 25
Time elapsed: 2518ms

The hough transform and red LED are missing deadlines and also the global time is clearly losing deadlines too.

Experiment 4:
-------- Task Execution Counts -----------
[Red LED]       Executed: 29    Expected: 103
[Green LED]     Executed: 122   Expected: 103
[Yellow LED]    Executed: 122   Expected: 103
[Jitter LED]    Executed: 6     Expected: 103
[Hough Transform]       Executed: 62    Expected: 103
Time elapsed: 10376ms

The global time again missed deadlines, but not as many. The RED led also did not miss as many deadlines.

Experiment 5:
 Could not collect data. Button polling method stopped working and I had to reset. The yellow light clearly blinks slower.

Experiment 6:
-------- Task Execution Counts -----------
[Red LED]       Executed: 0     Expected: 53
[Green LED]     Executed: 116   Expected: 53
[Yellow LED]    Executed: 63    Expected: 53
[Jitter LED]    Executed: 5     Expected: 53
[Hough Transform]       Executed: 38    Expected: 53
Time elapsed: 5395ms


Experiment 7:
 Could not collect data again. Button polling stopped working. The yellow light again clearly blinks slower.

5. Using the data collected in experiment #8, describe and explain the
   behavior of the system.

Experiment 8: I am unable to regain control of the system again.


6. Consider the various scheduling methods used here and discussed in class.
   For each method below, discuss the control over the timing of that task,
   responsiveness of the system, and the impact on other tasks with respect to
   timing.

   a. Time­driven execution inside an ISR (e.g. Time­Keeper Task),
   b. Time­driven release from within an ISR (e.g. Hough Transform Task),
   c. External interrupt with execution inside an ISR (e.g. Communication Task, if inside ISR),
   d. Periodic polling for an event from within an ISR, then release of task (e.g. JITTER LED task).

a. This gives good granularity for timing control if used sparingly, but if many
   tasks are executing inside of ISRs and they do not preempt each other, than
   response to events can become limited and deadlines can be missed.
b. ??? Does releasing also mean we preempt and start a different task if a task
   is running too long?
c. This gives good responsiveness if the interrupts are occasional.
d. The responsiveness depends on the polling and, if not pre-empting, the length
   of the currently running task.
