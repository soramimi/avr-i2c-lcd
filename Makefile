AVRISP = /dev/ttyUSB0

TARGET = main

OBJ = \
	lcd.o \
	main.o

MCU = atmega32u2
F_CPU = 16000000


CFLAGS = -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall -Wextra -Werror=return-type
CC = avr-gcc $(CFLAGS)
CXX = avr-g++ $(CFLAGS) -std=c++11
LDFLAGS = -Os

all: $(TARGET).hex

%.elf: $(OBJ)
	$(CXX) $^ --output $@ $(LDFLAGS)

%.hex: %.elf
	avr-objcopy -O ihex $< $@


clean:
	rm -f *.o
	rm -f *.hex


write: main.hex
	avrdude -c avrisp -P $(AVRISP) -b 19200 -p m32u2 -U hfuse:w:0xd9:m  -U lfuse:w:0x5e:m -U flash:w:$(TARGET).hex

