
#include "isplcd.h"

ISPLCD lcd;

void setup()
{
	// 16 MHz clock
	CLKPR = 0x80; CLKPR = 0;
	// Disable JTAG
	MCUCR |= 0x80; MCUCR |= 0x80;

	PORTB = 0x00;
	PORTC = 0x00;
	DDRB = 0x01;
	DDRC = 0x04;
	TCCR0B = 0x02; // 1/8 prescaling

	lcd.start();
	lcd.print("Hello, world");
}

void loop()
{
	lcd.led(1);
	_delay_ms(500);
	lcd.led(0);
	_delay_ms(500);
}

int main()
{
	setup();
	while (1) {
		loop();
	}
}

