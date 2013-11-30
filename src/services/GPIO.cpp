#include <avr/io.h>
#include <util/delay.h>

#include "GPIO.h"


GPIO::GPIO()
{
	DDRB |= _BV(4) | _BV(3) | _BV(2);
	DDRD |= _BV(6);
	DDRD &= _BV(7);
	PORTB &= ~(_BV(4) | _BV(3) | _BV(2));
	PORTD &= ~_BV(6);
	PORTD |= _BV(7);

}

void GPIO::setB2(int state)
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

void GPIO::setB3(int state)
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

void GPIO::setB4(int state)
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

void GPIO::setD6(int state)
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

uint8_t GPIO::getD7()
{
        return PIND & _BV(7);
}
