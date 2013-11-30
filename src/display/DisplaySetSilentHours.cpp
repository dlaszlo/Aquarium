#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include "DisplaySetSilentHours.h"
#include "../services/IRcodes.h"

int8_t field_silenthours;

int8_t silent_on_hour;
int8_t silent_on_minute;
int8_t silent_off_hour;
int8_t silent_off_minute;

#define FIELD_SILENT_ON_HOUR 0
#define FIELD_SILENT_ON_MINUTE 1
#define FIELD_SILENT_OFF_HOUR 2
#define FIELD_SILENT_OFF_MINUTE 3

void DisplaySetSilentHours::init()
{
	context.displayInfo.ir_event = 0;
	field_silenthours = 0;
	silent_off_hour = context.settings.silent_off_hour;
	silent_on_hour = context.settings.silent_on_hour;
	silent_off_minute = context.settings.silent_off_minute;
	silent_on_minute = context.settings.silent_on_minute;

}

uint8_t DisplaySetSilentHours::action()
{
	context.displayInfo.ir_event = 0;
	uint16_t key = context.displayInfo.ir_key;
	context.displayInfo.ir_key = 0;
	switch (key)
	{
	case KEY_TRACK_NEXT:
		field_silenthours++;
		if (field_silenthours > 3)
		{
			field_silenthours = 0;
		}
		break;
	case KEY_TRACK_PREV:
		field_silenthours--;
		if (field_silenthours < 0)
		{
			field_silenthours = 3;
		}
		break;
	case KEY_PLUS:
		switch (field_silenthours)
		{
		case FIELD_SILENT_ON_HOUR:
			silent_on_hour++;
			if (silent_on_hour > 23)
			{
				silent_on_hour = 0;
			}
			break;
		case FIELD_SILENT_ON_MINUTE:
			silent_on_minute++;
			if (silent_on_minute > 23)
			{
				silent_on_minute = 0;
			}
			break;
		case FIELD_SILENT_OFF_HOUR:
			silent_off_hour++;
			if (silent_off_hour > 23)
			{
				silent_off_hour = 0;
			}
			break;
		case FIELD_SILENT_OFF_MINUTE:
			silent_off_minute++;
			if (silent_off_minute > 23)
			{
				silent_off_minute = 0;
			}
			break;
		}
		break;
	case KEY_MINUS:
		switch (field_silenthours)
		{
		case FIELD_SILENT_ON_HOUR:
			silent_on_hour--;
			if (silent_on_hour < 0)
			{
				silent_on_hour = 23;
			}
			break;
		case FIELD_SILENT_ON_MINUTE:
			silent_on_minute--;
			if (silent_on_minute < 0)
			{
				silent_on_minute = 59;
			}
			break;
		case FIELD_SILENT_OFF_HOUR:
			silent_off_hour--;
			if (silent_off_hour < 0)
			{
				silent_off_hour = 23;
			}
			break;
		case FIELD_SILENT_OFF_MINUTE:
			silent_off_minute--;
			if (silent_off_minute < 0)
			{
				silent_off_minute = 59;
			}
			break;
		}
		break;
	case KEY_PLAY_PAUSE:
		context.settings.silent_off_hour = silent_off_hour;
		context.settings.silent_on_hour = silent_on_hour;
		context.settings.silent_off_minute = silent_off_minute;
		context.settings.silent_on_minute = silent_on_minute;
		context.ee24lc256.write_eeprom(0, sizeof(context.settings),
				(uint8_t *) &context.settings);
		return DISPLAY_SETTINGS;
	case KEY_REPEAT:
		return DISPLAY_SETTINGS;
	}
	return DISPLAY_SETSILENTHOURS;
}

void DisplaySetSilentHours::render()
{
	context.lcd.setText(0, 0, "Silent: on - off");
	char txt[17] = "";
	sprintf(txt, " %02d:%02d - %02d:%02d", silent_on_hour, silent_on_minute,
			silent_off_hour, silent_off_minute);
	context.lcd.setText(0, 1, txt);
	switch (field_silenthours)
	{
	case FIELD_SILENT_ON_HOUR:
		context.lcd.setText(0, 1, "}");
		context.lcd.setText(3, 1, "{");
		break;
	case FIELD_SILENT_ON_MINUTE:
		context.lcd.setText(3, 1, "}");
		context.lcd.setText(6, 1, "{");
		break;
	case FIELD_SILENT_OFF_HOUR:
		context.lcd.setText(8, 1, "}");
		context.lcd.setText(11, 1, "{");
		break;
	case FIELD_SILENT_OFF_MINUTE:
		context.lcd.setText(11, 1, "}");
		context.lcd.setText(14, 1, "{");
		break;
	}
}

void DisplaySetSilentHours::close()
{
//
}

uint8_t DisplaySetSilentHours::get_id()
{
	return DISPLAY_SETSILENTHOURS;
}
