#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include "DisplaySetClock.h"
#include "../services/IRcodes.h"
#include "../services/PCF8583.h"

int8_t field_clock;
uint8_t get_time;
PCF8583_t pcf8583;

#define FIELD_YEAR 0
#define FIELD_MONTH 1
#define FIELD_DAY 2
#define FIELD_HOUR 3
#define FIELD_MINUTE 4
#define FIELD_SECOND 5

void DisplaySetClock::init()
{
	context.displayInfo.ir_event = 0;
	get_time = 1;
	field_clock = 0;
	pcf8583.year = context.displayInfo.datetime.year;
	pcf8583.month = context.displayInfo.datetime.month;
	pcf8583.day = context.displayInfo.datetime.day;
	pcf8583.hour = context.displayInfo.datetime.hour;
	pcf8583.minute = context.displayInfo.datetime.minute;
	pcf8583.second = context.displayInfo.datetime.second;
}

uint8_t DisplaySetClock::action()
{
	context.displayInfo.ir_event = 0;
	uint16_t key = context.displayInfo.ir_key;
	context.displayInfo.ir_key = 0;
	switch (key)
	{
	case KEY_TRACK_NEXT:
		field_clock++;
		if (field_clock > 5)
		{
			field_clock = 0;
		}
		break;
	case KEY_TRACK_PREV:
		field_clock--;
		if (field_clock < 0)
		{
			field_clock = 5;
		}
		break;
	case KEY_PLUS:
		get_time = 0;
		switch (field_clock)
		{
		case FIELD_YEAR:
			pcf8583.year++;
			break;
		case FIELD_MONTH:
			pcf8583.month++;
			break;
		case FIELD_DAY:
			pcf8583.day++;
			break;
		case FIELD_HOUR:
			pcf8583.hour++;
			break;
		case FIELD_MINUTE:
			pcf8583.minute++;
			break;
		case FIELD_SECOND:
			pcf8583.second++;
			break;
		}
		context.pcf8583.prepare_time(&pcf8583);
		break;
	case KEY_MINUS:
		get_time = 0;
		switch (field_clock)
		{
		case FIELD_YEAR:
			pcf8583.year--;
			break;
		case FIELD_MONTH:
			pcf8583.month--;
			break;
		case FIELD_DAY:
			pcf8583.day--;
			break;
		case FIELD_HOUR:
			pcf8583.hour--;
			break;
		case FIELD_MINUTE:
			pcf8583.minute--;
			break;
		case FIELD_SECOND:
			pcf8583.second--;
			break;
		}
		context.pcf8583.prepare_time(&pcf8583);
		break;
	case KEY_PLAY_PAUSE:
		context.pcf8583.set_clock(&pcf8583);
		return DISPLAY_SETTINGS;
	case KEY_REPEAT:
		return DISPLAY_SETTINGS;
	}
	return DISPLAY_SETCLOCK;
}

void DisplaySetClock::render()
{
	if (get_time)
	{
		pcf8583.year = context.displayInfo.datetime.year;
		pcf8583.month = context.displayInfo.datetime.month;
		pcf8583.day = context.displayInfo.datetime.day;
		pcf8583.hour = context.displayInfo.datetime.hour;
		pcf8583.minute = context.displayInfo.datetime.minute;
		pcf8583.second = context.displayInfo.datetime.second;
	}
	char txt[17] = "";
	sprintf(txt, "%04d.%02d.%02d", pcf8583.year, pcf8583.month, pcf8583.day);
	context.lcd.center(0, txt);
	memset(txt, 0, 17);
	sprintf(txt, "%02d:%02d:%02d", pcf8583.hour, pcf8583.minute,
			pcf8583.second);
	context.lcd.center(1, txt);
	switch (field_clock)
	{
	case FIELD_YEAR:
		context.lcd.setText(2, 0, "}");
		context.lcd.setText(7, 0, "{");
		break;
	case FIELD_MONTH:
		context.lcd.setText(7, 0, "}");
		context.lcd.setText(10, 0, "{");
		break;
	case FIELD_DAY:
		context.lcd.setText(10, 0, "}");
		context.lcd.setText(13, 0, "{");
		break;
	case FIELD_HOUR:
		context.lcd.setText(3, 1, "}");
		context.lcd.setText(6, 1, "{");
		break;
	case FIELD_MINUTE:
		context.lcd.setText(6, 1, "}");
		context.lcd.setText(9, 1, "{");
		break;
	case FIELD_SECOND:
		context.lcd.setText(9, 1, "}");
		context.lcd.setText(12, 1, "{");
		break;
	}
}

void DisplaySetClock::close()
{
//
}

uint8_t DisplaySetClock::get_id()
{
	return DISPLAY_SETCLOCK;
}

