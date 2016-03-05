PORT=/dev/tty.usbmodem*
MCU=atmega32u4

# Local Library Paths
INCLUDES=../bsp ../../../VirtualSerial
LIBS=hal VirtualSerial

CINC=$(foreach d, $(INCLUDES), -I$(d))
LDINC=$(foreach d, $(INCLUDES), -L$(d))
LIBPARAMS=$(foreach d, $(LIBS), -l$(d))

CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os -std=c99 $(CINC)
LDFLAGS=-Wl,-gc-sections -Wl,-relax $(LDINC) $(LIBPARAMS)
CC=avr-gcc
TARGET=app_main
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
