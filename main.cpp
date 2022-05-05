
#include "lcd.h"
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>

#define CLOCK 16000000UL
#define SCALE 125
static unsigned short _scale = 0;
//static unsigned long _system_tick_count;
//static unsigned long _tick_count;
//static unsigned long _time_s;
static unsigned short _time_ms = 0;
uint8_t interval_1ms_flag = 0;
ISR(TIMER0_OVF_vect, ISR_NOBLOCK)
{
//	_system_tick_count++;
	_scale += 16;
	if (_scale >= SCALE) {
		_scale -= SCALE;
//		_tick_count++;
		_time_ms++;
		if (_time_ms >= 1000) {
//			_time_ms = 0;
//			_time_s++;
		}
		interval_1ms_flag = 1;
	}
}

enum {
	LCD_NONE = 0,
	LCD_HOME = 0x0c,
};

uint8_t lcd_queue[128];
uint8_t lcd_head = 0;
uint8_t lcd_size = 0;

extern "C" void lcd_putchar(uint8_t c)
{
	cli();
	if (lcd_size < 128 && c != 0) {
		lcd_queue[(lcd_head + lcd_size) & 0x7f] = c;
		lcd_size++;
	}
	sei();
}

extern "C" void lcd_print(char const *p)
{
	while (*p) {
		lcd_putchar(*p++);
	}
}

extern "C" void lcd_puthex8(uint8_t v)
{
	static char hex[] = "0123456789ABCDEF";
	lcd_putchar(hex[(v >> 4) & 0x0f]);
	lcd_putchar(hex[v & 0x0f]);
}

extern "C" void lcd_home()
{
	lcd_putchar(LCD_HOME);
}

static uint8_t lcd_popfront()
{
	uint8_t c = 0;
	cli();
	if (lcd_size > 0) {
		c = lcd_queue[lcd_head];
		lcd_head = (lcd_head + 1) & 0x7f;
		lcd_size--;
	}
	sei();
	return c;
}

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
	TIMSK0 |= 1 << TOIE0;


	lcd::init();
	lcd::clear();
	lcd::home();
	lcd_print("Hello, world");
}

void loop()
{
	led(1);
	_delay_ms(500);
	led(0);
	_delay_ms(500);
}

int main()
{
	setup();
	sei();
	while (1) {
		uint8_t c = lcd_popfront();
		if (c == LCD_NONE) {
			loop();
		} else if (c < ' ') {
			if (c == LCD_HOME) {
				lcd::home();
			}
		} else {
			lcd::putchar(c);
		}
	}
}

