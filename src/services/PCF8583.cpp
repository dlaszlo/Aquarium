#include "PCF8583.h"

uint8_t buffer_i2c[9] =
{ 0 };

void PCF8583::get_clock(PCF8583_t * params)
{
	buffer_i2c[0] = 0x01; // start of time data
	_twi.receive(_addr, buffer_i2c, 1, 7);
	params->second = (buffer_i2c[2] & 0x0F) + (buffer_i2c[2] >> 4) * 10;
	params->minute = ((buffer_i2c[3] & 0x0F) + (buffer_i2c[3] >> 4) * 10);
	params->hour = ((buffer_i2c[4] & 0x0F) + (buffer_i2c[4] >> 4) * 10);
	uint16_t year0 = (buffer_i2c[5] >> 6);
	params->day = ((buffer_i2c[5] & 0x0F) + ((buffer_i2c[5] & 0x3F) >> 4) * 10);
	params->month = ((buffer_i2c[6] & 0x0F) + ((buffer_i2c[6] & 0x1F) >> 4) * 10);
	buffer_i2c[0] = 0x10;
	_twi.receive(_addr, buffer_i2c, 1, 3);
	year0 |= (buffer_i2c[1] << 8);
	year0 |= buffer_i2c[2];
	params->year = year0;
}

void PCF8583::set_clock(PCF8583_t * params)
{
	prepare_time(params);

	buffer_i2c[0] = 0x01;
	buffer_i2c[1] = 0;
	buffer_i2c[2] = ((params->second / 10) << 4) | (params->second % 10);
	buffer_i2c[3] = ((params->minute / 10) << 4) | (params->minute % 10);
	buffer_i2c[4] = ((params->hour / 10) << 4) | (params->hour % 10);
	buffer_i2c[5] = ((params->day / 10) << 4) | (params->day % 10) | ((params->year & 3) << 6);
	buffer_i2c[6] = ((params->month / 10) << 4) | (params->month % 10);
	_twi.send(_addr, buffer_i2c, 7);
	buffer_i2c[0] = 0x10;
	buffer_i2c[1] = params->year >> 8;
	buffer_i2c[2] = params->year & 0xfc;
	_twi.send(_addr, buffer_i2c, 3);
}

int8_t PCF8583::get_num_of_days(int month, int year)
{
	int8_t numOfDays = 0;
	if (month == 2)
	{
		if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
		{
			numOfDays = 29;
		}
		else
		{
			numOfDays = 28;
		}
	}
	else if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		numOfDays = 30;
	}
	else
	{
		numOfDays = 31;
	}
	return numOfDays;
}

void PCF8583::prepare_value(int *val, int min, int max)
{
	if (*val > max)
	{
		*val = min;
	}
	if (*val < min)
	{
		*val = max;
	}
}

void PCF8583::prepare_value_8(int8_t *val, int8_t min, int8_t max)
{
	if (*val > max)
	{
		*val = min;
	}
	if (*val < min)
	{
		*val = max;
	}
}


void PCF8583::prepare_time(PCF8583_t * params)
{
	prepare_value((int *) &params->year, 1970, 2500);
	prepare_value_8((int8_t *) &params->month, 1, 12);
	prepare_value_8((int8_t *) &params->day, 1,
			get_num_of_days(params->year, params->month));
	prepare_value_8((int8_t *) &params->hour, 0, 23);
	prepare_value_8((int8_t *) &params->minute, 0, 59);
	prepare_value_8((int8_t *) &params->second, 0, 59);
}
