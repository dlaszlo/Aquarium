#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "TWI.h"

TWI::TWI()
{
	_status = 0;
	_init = 0;
}

void TWI::waitInt()
{
	while (!(TWCR & (1 << TWINT)))
		;
}

void TWI::getStatus(void)
{
	_status = TWSR & 0xF8;
}

void TWI::start()
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	waitInt();
	getStatus();
}

void TWI::stop()
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI::write(uint8_t b)
{
	TWDR = b;
	TWCR = (1 << TWINT) | (1 << TWEN);
	waitInt();
	getStatus();
}

uint8_t TWI::readAck(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	waitInt();
	getStatus();
	return TWDR ;
}

uint8_t TWI::readNack(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN);
	waitInt();
	getStatus();
	return TWDR ;
}

void TWI::init(void)
{
	if (!_init)
	{
		_init = 1;
		//set SCL to 400kHz
		TWSR = 0x00;
		TWBR = 0x0C;
		//enable TWI
		TWCR = (1 << TWEN);
	}
}

uint8_t TWI::send(uint8_t addr, uint8_t * sb, uint8_t sb_size)
{
	start();
	if (_status != START)
	{
		return TWI_ERROR;
	}
	write(addr << 1);
	if (_status != MTX_ADR_ACK)
	{
		return TWI_ERROR;
	}
	for (int i = 0; i < sb_size; i++)
	{
		write(sb[i]);
		if (_status != MTX_DATA_ACK)
		{
			return TWI_ERROR;
		}
	}
	stop();
	return TWI_OK;
}

uint8_t TWI::receive(uint8_t addr, uint8_t * buff, uint8_t sb_size,
		uint8_t rb_size)
{
	start();
	if (_status != START)
	{
		return TWI_ERROR;
	}
	write(addr << 1);
	if (_status != MTX_ADR_ACK)
	{
		return TWI_ERROR;
	}
	int i = 0;
	for (; i < sb_size; i++)
	{
		write(buff[i]);
		if (_status != MTX_DATA_ACK)
		{
			return TWI_ERROR;
		}
	}
	start();
	if (_status != REP_START)
	{
		return TWI_ERROR;
	}
	write((addr << 1) | 1);
	if (_status != MRX_ADR_ACK)
	{
		return TWI_ERROR;
	}
	for (; i < rb_size; i++)
	{
		if (i + 1 >= rb_size)
		{
			buff[i] = readNack();
			if (_status != MRX_DATA_NACK)
			{
				return TWI_ERROR;
			}
		}
		else
		{
			buff[i] = readAck();
			if (_status != MRX_DATA_ACK)
			{
				return TWI_ERROR;
			}
		}
	}
	stop();
	return TWI_OK;

}
