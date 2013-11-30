#ifndef DS18B20_H_
#define DS18B20_H_

#include "OneWire.h"

class DS18B20
{

public:
	DS18B20(OneWire & _oneWire);

	float get_temperature(void);

private:
	OneWire & oneWire;

};

#endif /* DS18B20_H_ */
