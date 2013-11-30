#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "DS18B20.h"
#include "OneWire.h"

DS18B20::DS18B20(OneWire & _oneWire) : oneWire(_oneWire)
{
	if (!oneWire.reset())
	{
		// Skip ROM command
		oneWire.write(0xCC);
		// Start Conversion command
		oneWire.write(0x44);
	}

}

float DS18B20::get_temperature(void)
{
	cli();
	if (!oneWire.reset())
	{
		// Skip ROM command
		oneWire.write(0xCC);
		// Start Conversion command
		oneWire.write(0x44);
		_delay_ms(100);
		if (!oneWire.reset())
		{
			// Skip ROM command
			oneWire.write(0xCC);
			// Read Scratchpad command
			oneWire.write(0xBE);
			uint8_t low = oneWire.read();
			uint8_t high = oneWire.read();
			for (int i = 0; i < 7; i++)
			{
				oneWire.read();
			}
			uint16_t temp = (high << 8) | low;
			uint8_t f = temp & 15;
			temp >>= 4;
			float rt = temp;

			rt += (float) f / (float) 16;

			sei();
			return rt;
		}
	}
	sei();
	return -1;
}
