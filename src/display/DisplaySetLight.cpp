#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include "DisplaySetLight.h"
#include "../services/IRcodes.h"

int8_t field_light;

int8_t light_on_hour;
int8_t light_on_minute;
int8_t light_off_hour;
int8_t light_off_minute;

#define FIELD_LIGHT_ON_HOUR 0
#define FIELD_LIGHT_ON_MINUTE 1
#define FIELD_LIGHT_OFF_HOUR 2
#define FIELD_LIGHT_OFF_MINUTE 3

void DisplaySetLight::init()
{
	context.displayInfo.ir_event = 0;
	field_light = 0;
	light_off_hour = context.settings.light_off_hour;
	light_on_hour = context.settings.light_on_hour;
	light_off_minute = context.settings.light_off_minute;
	light_on_minute = context.settings.light_on_minute;

}

uint8_t DisplaySetLight::action()
{
	context.displayInfo.ir_event = 0;
	uint16_t key = context.displayInfo.ir_key;
	context.displayInfo.ir_key = 0;
	switch (key)
	{
	case KEY_TRACK_NEXT:
		field_light++;
		if (field_light > 3)
		{
			field_light = 0;
		}
		break;
	case KEY_TRACK_PREV:
		field_light--;
		if (field_light < 0)
		{
			field_light = 3;
		}
		break;
	case KEY_PLUS:
		switch (field_light)
		{
		case FIELD_LIGHT_ON_HOUR:
			light_on_hour++;
			if (light_on_hour > 23)
			{
				light_on_hour = 0;
			}
			break;
		case FIELD_LIGHT_ON_MINUTE:
			light_on_minute++;
			if (light_on_minute > 23)
			{
				light_on_minute = 0;
			}
			break;
		case FIELD_LIGHT_OFF_HOUR:
			light_off_hour++;
			if (light_off_hour > 23)
			{
				light_off_hour = 0;
			}
			break;
		case FIELD_LIGHT_OFF_MINUTE:
			light_off_minute++;
			if (light_off_minute > 23)
			{
				light_off_minute = 0;
			}
			break;
		}
		break;
	case KEY_MINUS:
		switch (field_light)
		{
		case FIELD_LIGHT_ON_HOUR:
			light_on_hour--;
			if (light_on_hour < 0)
			{
				light_on_hour = 23;
			}
			break;
		case FIELD_LIGHT_ON_MINUTE:
			light_on_minute--;
			if (light_on_minute < 0)
			{
				light_on_minute = 59;
			}
			break;
		case FIELD_LIGHT_OFF_HOUR:
			light_off_hour--;
			if (light_off_hour < 0)
			{
				light_off_hour = 23;
			}
			break;
		case FIELD_LIGHT_OFF_MINUTE:
			light_off_minute--;
			if (light_off_minute < 0)
			{
				light_off_minute = 59;
			}
			break;
		}
		break;
	case KEY_PLAY_PAUSE:
		context.settings.light_off_hour = light_off_hour;
		context.settings.light_on_hour = light_on_hour;
		context.settings.light_off_minute = light_off_minute;
		context.settings.light_on_minute = light_on_minute;
		context.ee24lc256.write_eeprom(0, sizeof(context.settings),
				(uint8_t *) &context.settings);
		return DISPLAY_SETTINGS;
	case KEY_REPEAT:
		return DISPLAY_SETTINGS;
	}
	return DISPLAY_SETLIGHT;
}

void DisplaySetLight::render()
{
	context.lcd.setText(0, 0, "Light: on - off");
	char txt[17] = "";
	sprintf(txt, " %02d:%02d - %02d:%02d", light_on_hour, light_on_minute,
			light_off_hour, light_off_minute);
	context.lcd.setText(0, 1, txt);
	switch (field_light)
	{
	case FIELD_LIGHT_ON_HOUR:
		context.lcd.setText(0, 1, "}");
		context.lcd.setText(3, 1, "{");
		break;
	case FIELD_LIGHT_ON_MINUTE:
		context.lcd.setText(3, 1, "}");
		context.lcd.setText(6, 1, "{");
		break;
	case FIELD_LIGHT_OFF_HOUR:
		context.lcd.setText(8, 1, "}");
		context.lcd.setText(11, 1, "{");
		break;
	case FIELD_LIGHT_OFF_MINUTE:
		context.lcd.setText(11, 1, "}");
		context.lcd.setText(14, 1, "{");
		break;
	}
}

void DisplaySetLight::close()
{
//
}

uint8_t DisplaySetLight::get_id()
{
	return DISPLAY_SETLIGHT;
}
