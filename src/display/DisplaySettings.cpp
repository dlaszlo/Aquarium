#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include "DisplaySettings.h"
#include "../services/IRcodes.h"

static const char * menuitems[] =
{ "*** settings ***", "clock", "cooler", "light", "filter clean", "silent hours" };
int8_t selected = 1;
uint8_t menulength = sizeof(menuitems) / sizeof(menuitems[0]);

void DisplaySettings::init()
{
	context.displayInfo.ir_event = 0;
	if (!context.displayInfo.selected_menu)
	{
		selected = 1;
	}
	else
	{
		selected = context.displayInfo.selected_menu;
	}
}

uint8_t DisplaySettings::action()
{
	context.displayInfo.ir_event = 0;
	uint16_t key = context.displayInfo.ir_key;
	context.displayInfo.ir_key = 0;
	switch (key)
	{
	case KEY_DOWN:
		selected++;
		if (selected >= menulength)
		{
			selected = 1;
		}
		break;
	case KEY_UP:
		selected--;
		if (selected <= 0)
		{
			selected = menulength - 1;
		}
		break;
	case KEY_EDIT:
		context.displayInfo.selected_menu = selected;
		if (selected == 1)
		{
			return DISPLAY_SETCLOCK;
		}
		else if (selected == 2)
		{
			return DISPLAY_SETCOOLER;
		}
		else if (selected == 3)
		{
			return DISPLAY_SETLIGHT;
		}
		else if (selected == 5)
		{
			return DISPLAY_SETSILENTHOURS;
		}
		break;
	case KEY_BACK:
		context.displayInfo.selected_menu = 0;
		return DISPLAY_MAIN;
	}
	return DISPLAY_SETTINGS;
}

void DisplaySettings::render()
{
	int st = selected & 0xfe;
	for (int i = 0; i < 2; i++)
	{
		if (st + i < menulength)
		{
			context.lcd.center(i, menuitems[st + i]);
			if (st + i == selected)
			{
				context.lcd.setText(0, i, "}");
				context.lcd.setText(15, i, "{");
			}
		}
	}
}

void DisplaySettings::close()
{
//
}

uint8_t DisplaySettings::get_id()
{
	return DISPLAY_SETTINGS;
}
