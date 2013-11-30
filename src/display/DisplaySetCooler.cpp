#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include "DisplaySetCooler.h"
#include "../services/IRcodes.h"
#include "../services/PCF8583.h"

float temperature;

void DisplaySetCooler::init()
{
	context.displayInfo.ir_event = 0;
	temperature = context.settings.cooler_temperature;
}

uint8_t DisplaySetCooler::action()
{
	context.displayInfo.ir_event = 0;
	uint16_t key = context.displayInfo.ir_key;
	context.displayInfo.ir_key = 0;
	switch (key)
	{
	case KEY_TRACK_NEXT:
		temperature += 1;
		if (temperature > COOLER_MAX_TEMP)
		{
			temperature = COOLER_MAX_TEMP;
		}
		break;
	case KEY_TRACK_PREV:
		temperature -= 1;
		if (temperature < COOLER_MIN_TEMP)
		{
			temperature = COOLER_MIN_TEMP;
		}
		break;
	case KEY_PLUS:
		temperature += 0.1;
		if (temperature > COOLER_MAX_TEMP)
		{
			temperature = COOLER_MAX_TEMP;
		}
		break;
	case KEY_MINUS:
		temperature -= 0.1;
		if (temperature < COOLER_MIN_TEMP)
		{
			temperature = COOLER_MIN_TEMP;
		}
		break;
	case KEY_EQ:
		temperature = COOLER_DEF_TEMP;
		break;
	case KEY_PLAY_PAUSE:
		context.settings.cooler_temperature = temperature;
		context.ee24lc256.write_eeprom(0, sizeof(context.settings), (uint8_t *) &context.settings);
		return DISPLAY_SETTINGS;
	case KEY_REPEAT:
		return DISPLAY_SETTINGS;
	}
	return DISPLAY_SETCOOLER;
}

void DisplaySetCooler::render()
{

	char txt[17] = "";
	char tmp[17] = "";
	memset(tmp, 0, 17);
	dtostrf(temperature, 1, 1, tmp);
	sprintf(txt, "}%s~C{", tmp);

	context.lcd.setText(0, 0, "Cooler temp.:");
	context.lcd.center(1, txt);
}

void DisplaySetCooler::close()
{
//
}

uint8_t DisplaySetCooler::get_id()
{
	return DISPLAY_SETCOOLER;
}

