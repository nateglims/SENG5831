# BSP Make File.
MCU=atmega32u4
INCLUDES=../../../VirtualSerial
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -std=c99 -I$(INCLUDES)
LDFLAGS=-Wl,-gc-sections -Wl,-relax -L$(INCLUDES) -lserial
CC=avr-gcc
AR=avr-ar
TARGET=hal

lib$(TARGET).a: $(TARGET).o
	$(AR) rcs $@ $^

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) $^ -c -o $@

clean:
	rm -rf *.o
