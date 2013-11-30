#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "OneWire.h"

typedef struct
{
	volatile uint8_t * pinreg;
	uint8_t pin;
} onewire_t;

volatile onewire_t onewire;

#define ONEWIRE_BIT			(PINB & _BV(1))
#define ONEWIRE_PULL_LOW()	{ DDRB |= _BV(1);  }
#define ONEWIRE_LISTEN()	{ DDRB &= ~_BV(1); }

OneWire::OneWire()
{
	//
}

// Returns 0 if the sensor is present, 1 otherwise
uint8_t OneWire::reset(void)
{
	uint8_t status;
	cli();
	ONEWIRE_PULL_LOW()
	_delay_us(480);
	ONEWIRE_LISTEN()
	_delay_us(70);
	status = ONEWIRE_BIT;
	sei();
	_delay_us(410);
	return status;
}

// Read Bit Function
uint8_t OneWire::readBit(void)
{
	uint8_t bit;
	cli();
	ONEWIRE_PULL_LOW()
	_delay_us(3);
	ONEWIRE_LISTEN()
	_delay_us(10);
	bit = ONEWIRE_BIT;
	sei();
	_delay_us(53);
	return bit;
}

// Write bit Function
void OneWire::writeBit(uint8_t bit)
{
	cli();
	ONEWIRE_PULL_LOW()
	if (bit)
	{
		_delay_us(10);
	}
	else
	{
		_delay_us(55);
	}
	ONEWIRE_LISTEN()
	sei();
	if (bit)
	{
		_delay_us(65);
	}
	else
	{
		_delay_us(5);
	}
}

// Read BYTE Function
uint8_t OneWire::read(void)
{
	uint8_t byte = 0;
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		byte >>= 1;
		if (readBit())
		{
			byte |= 0x80;
		}
	}
	return byte;
}

// Write BYTE Function
void OneWire::write(uint8_t byte)
{
	uint8_t i;
	uint8_t temp;

	for (i = 0; i < 8; i++)
	{
		temp = byte >> i;
		temp &= 1;
		writeBit(temp);
	}
}
