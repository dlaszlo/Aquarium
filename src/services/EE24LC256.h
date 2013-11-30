#ifndef EE24LC256_H_
#define EE24LC256_H_

#include "TWI.h"

class EE24LC256
{
public:
	EE24LC256(TWI & twi, uint8_t addr) : _twi(twi) {
		_addr = addr;
	};

	uint8_t write_eeprom(uint16_t eeaddr, uint16_t length, uint8_t * data);
	uint8_t read_eeprom(uint16_t eeaddr, uint16_t length, uint8_t * data);

private:

	TWI & _twi;
	uint8_t _addr;

};

#endif
