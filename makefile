TARGET = main.out
CFILES = list.c kernel.c kernel_avr.c avr/log.c main.c
MCU = atmega16
OPTLEVEL=0
CFLAGS = -mmcu=$(MCU) -std=c99 -g -O$(OPTLEVEL) -Wall -Wextra \
         -pedantic -Iavr -DDEBUG
LDFLAGS = -mmcu=$(MCU) -Wl,-Map,$(TARGET).map
#VPATH = ..
CC = avr-gcc

OBJECTS = $(CFILES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

$(OBJECTS): $(DEPS)

size: $(TARGET) $(OBJECTS)
	avr-size --totals $(OBJECTS)
	avr-size $(TARGET)

program: $(TARGET)
	avarice -g --erase --program --file $(TARGET)

gdbserver: $(TARGET)
	avarice -g --erase --program --file $(TARGET) -B 1000000 localhost:1212

gdb: $(TARGET)
	avr-gdb -x gdbinit

clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).map

# Every non-.PHONY rule must update a file with the exact
# name of its target.  Make sure every command script touches
# the file "$@"-- not "../$@", or "$(notdir $@)", but exactly
# $@. That way you and GNU make always agree.
# http://mad-scientist.net/make/rules.html

