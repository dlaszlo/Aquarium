#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include "DisplayMain.h"
#include "../services/IRcodes.h"

void DisplayMain::init()
{
	context.displayInfo.ir_event = 0;
}

uint8_t DisplayMain::action()
{
	if (!context.gpio.getD7())
        {
	        return DISPLAY_MAIN;
        }

	uint16_t key = context.displayInfo.ir_key;
	context.displayInfo.ir_event = 0;
	context.displayInfo.ir_key = 0;
	if (key == KEY_PLAY_PAUSE)
	{
		return DISPLAY_SETTINGS;
	}
	else
	{
		return DISPLAY_MAIN;
	}
}

void DisplayMain::render()
{
	char txt[17] = "";
	char tmp[17] = "";
        if (!context.gpio.getD7())
        {
            sprintf(txt, "keycode=%06d", context.displayInfo.ir_key);
            context.lcd.setText(0, 0, txt);
            context.displayInfo.ir_event = 0;
            context.displayInfo.ir_key = 0;
        }
        else
        {
          context.displayInfo.ir_event = 0;
          context.displayInfo.ir_key = 0;
          sprintf(txt, "%04d.%02d.%02d", context.displayInfo.datetime.year, context.displayInfo.datetime.month,
                          context.displayInfo.datetime.day);
          context.lcd.center(0, txt);
          memset(txt, 0, 17);
          sprintf(txt, "%02d:%02d:%02d", context.displayInfo.datetime.hour, context.displayInfo.datetime.minute,
                          context.displayInfo.datetime.second);
          context.lcd.setText(0, 1, txt);
          memset(txt, 0, 17);
          memset(tmp, 0, 17);
          dtostrf(context.displayInfo.temperature, 1, 1, tmp);
          sprintf(txt, "%s~C", tmp);
          context.lcd.right(1, txt);
        }
}

void DisplayMain::close()
{
	//
}

uint8_t DisplayMain::get_id()
{
	return DISPLAY_MAIN;
}
