#ifndef PCF8583_H_
#define PCF8583_H_

#include "TWI.h"

typedef struct
{
	int16_t year;
	int8_t month;
	int8_t day;
	int8_t hour;
	int8_t minute;
	int8_t second;
} PCF8583_t;

class PCF8583
{
public:
	PCF8583(TWI & twi, uint8_t addr) : _twi(twi) {
		_addr = addr;
	};

	void get_clock(PCF8583_t * params);
	void set_clock(PCF8583_t * params);
	void prepare_time(PCF8583_t * params);

private:
	int8_t get_num_of_days(int month, int year);
	void prepare_value(int *val, int min, int max);
	void prepare_value_8(int8_t *val, int8_t min, int8_t max);

	TWI & _twi;
	uint8_t _addr;

};

#endif /* PCF8583_H_ */
