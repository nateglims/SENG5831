PORT=/dev/tty.usbmodem*
MCU=atmega32u4
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os
LDFLAGS=-Wl,-gc-sections -Wl,-relax
CC=avr-gcc
TARGET=timer
OBJECT_FILES=timer.o

all: $(TARGET).hex

.PHONY: clean program

clean:
		rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
		avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
		$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
		avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:$(TARGET).hex

