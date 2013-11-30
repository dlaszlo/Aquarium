#include <util/delay.h>
#include "EE24LC256.h"
#include "TWI.h"

uint8_t EE24LC256::read_eeprom(uint16_t eeaddr, uint16_t length, uint8_t * data)
{
	_twi.start();
	if (_twi._status != START)
	{
		return TWI_ERROR;
	}
	_twi.write(_addr << 1);
	if (_twi._status != MTX_ADR_ACK)
	{
		return TWI_ERROR;
	}
	_twi.write((uint8_t)(eeaddr >> 8));
	if (_twi._status != MTX_DATA_ACK)
	{
		return TWI_ERROR;
	}
	_twi.write((uint8_t)(eeaddr & 0x0ff));
	if (_twi._status != MTX_DATA_ACK)
	{
		return TWI_ERROR;
	}
	_twi.start();
	if (_twi._status != REP_START)
	{
		return TWI_ERROR;
	}
	_twi.write((_addr << 1) | 1);
	if (_twi._status != MRX_ADR_ACK)
	{
		return TWI_ERROR;
	}
	for (uint16_t i = 0; i < length; i++)
	{
		if (i + 1 >= length)
		{
			data[i] = _twi.readNack();
			if (_twi._status != MRX_DATA_NACK)
			{
				return TWI_ERROR;
			}
		}
		else
		{
			data[i] = _twi.readAck();
			if (_twi._status != MRX_DATA_ACK)
			{
				return TWI_ERROR;
			}
		}
	}
	_twi.stop();
	return TWI_OK;
}

uint8_t EE24LC256::write_eeprom(uint16_t eeaddr, uint16_t length, uint8_t * data)
{
	uint16_t ea = eeaddr;
	uint16_t i = 0;
	while (i < length)
	{
		_twi.start();
		if (_twi._status != START)
		{
			return TWI_ERROR;
		}
		_twi.write(_addr << 1);
		if (_twi._status != MTX_ADR_ACK)
		{
			return TWI_ERROR;
		}
		_twi.write((uint8_t)(ea >> 8));
		if (_twi._status != MTX_DATA_ACK)
		{
			return TWI_ERROR;
		}
		_twi.write((uint8_t)(ea & 0x0ff));
		if (_twi._status != MTX_DATA_ACK)
		{
			return TWI_ERROR;
		}
		for (; i < length; i++)
		{
			_twi.write(data[i]);
			if (_twi._status != MTX_DATA_ACK)
			{
				return TWI_ERROR;
			}
			ea++;
			if (!(ea & 0x3f))
			{
				break;
			}
		}
		_twi.stop();

		_delay_ms(6);
	}
	return TWI_OK;
}

