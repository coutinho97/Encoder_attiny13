# ===============
MCU     	= attiny13
F_CPU   	= 9600000UL   	# clock (9.6MHz, 1.2MHz, etc.)

# ===============
PROGRAMMER 	= usbasp    	# usbasp, usbtiny, arduino, etc.

CC        	= avr-gcc
OBJCOPY   	= avr-objcopy
AVRDUDE   	= avrdude

CFLAGS    	= -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -std=gnu11 -ffunction-sections -fdata-sections
LDFLAGS   	= -mmcu=$(MCU)

#FILE      	= emissor.c
FILE    	= recetor.c     

PROJECT   	= $(basename $(FILE))
TARGET    	= $(PROJECT).hex

# ====================

all: $(TARGET)

$(PROJECT).elf: $(FILE)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(TARGET): $(PROJECT).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET)
	$(AVRDUDE) -c $(PROGRAMMER) -p t13 -U flash:w:$(TARGET):i

fuse:
	$(AVRDUDE) -c $(PROGRAMMER) -p t13 -U lfuse:w:0x7A:m -U hfuse:w:0xFF:m

clean:
	rm -f *.elf *.hex *.o

# Comandos
show:
	@echo "Compiling: $(FILE)"
	@echo "Project: $(PROJECT)"

.PHONY: all flash fuse clean show