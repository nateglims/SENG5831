PORT=/dev/tty.usbmodem*
MCU=atmega32u4
INCLUDES=../VirtualSerial/
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -std=c99 -I$(INCLUDES)
LDFLAGS=-Wl,-gc-sections -Wl,-relax -L$(INCLUDES) -lVirtualSerial
CC=avr-gcc
TARGET=serial
OBJECT_FILES=$(TARGET).o

all: $(TARGET).hex

.PHONY: clean program
.SECONDARY: $(OBJECT_FILES)

clean:
		rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
		avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
		$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
		avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:$(TARGET).hex

