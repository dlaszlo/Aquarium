#include <avr/io.h>
#include <util/delay.h>

#include "GPIO.h"


GPIO::GPIO()
{
	DDRB |= _BV(4) | _BV(3) | _BV(2);
	DDRD |= _BV(6);
	PORTB &= ~(_BV(4) | _BV(3) | _BV(2));
	PORTD &= ~_BV(6);
}

void GPIO::GPIOB2(int state)
{
	if (state)
	{
		PORTB |= _BV(2);
	}
	else
	{
		PORTB &= ~_BV(2);
	}
}

void GPIO::GPIOB3(int state)
{
	if (state)
	{
		PORTB |= _BV(3);
	}
	else
	{
		PORTB &= ~_BV(3);
	}
}

void GPIO::GPIOB4(int state)
{
	if (state)
	{
		PORTB |= _BV(4);
	}
	else
	{
		PORTB &= ~_BV(4);
	}
}

void GPIO::GPIOD6(int state)
{
	if (state)
	{
		PORTD |= _BV(6);
	}
	else
	{
		PORTD &= ~_BV(6);
	}
}
