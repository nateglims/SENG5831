# BSP Make File
MCU=atmega32u4
INCLUDES=../../../VirtualSerial
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -std=c99 -I$(INCLUDES)
LDFLAGS=-Wl,-gc-sections -Wl,-relax -L$(INCLUDES) -lserial
CC=avr-gcc
AR=avr-ar
TARGET=led delay hal

all: libhal.a

libhal.a: $(foreach obj, $(TARGET), $(obj).o)
	$(AR) -vrcs $@ $^

objs:
	$(foreach file, $(TARGET), $(CC) $(CFLAGS) $(file).c -c -o $(file).o ;)

clean:
	rm -rf *.o
