#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "services/Context.h"
#include "display/DisplayMain.h"
#include "display/DisplayWelcome.h"
#include "display/DisplaySettings.h"
#include "display/DisplaySetClock.h"
#include "display/DisplaySetCooler.h"
#include "display/DisplaySetLight.h"
#include "display/DisplaySetSilentHours.h"

#define LCD_TIMEOUT 20
#define DISPLAY_TIMEOUT 60

Context context;

decode_results results;

DisplayMain displayMain(context);
DisplayWelcome displayWelcome(context);
DisplaySettings displaySettings(context);
DisplaySetClock displaySetClock(context);
DisplaySetCooler displaySetCooler(context);
DisplaySetLight displaySetLight(context);
DisplaySetSilentHours displaySetSilentHours(context);

Display * d[] =
{ &displayWelcome, &displayMain, &displaySettings, &displaySetClock,
		&displaySetCooler, &displaySetLight, &displaySetSilentHours };

volatile uint8_t last_display = 0xff;
volatile uint8_t current_display = DISPLAY_WELCOME;
volatile uint8_t error = 0;
volatile uint8_t t = 0;

volatile int lcd_timeout;
volatile int display_timeout;

void init_settings()
{
	uint8_t save_eeprom = 0;
	context.ee24lc256.read_eeprom(0, sizeof(context.settings),
			(uint8_t *) &context.settings);
	if (context.settings.cooler_temperature
			!= context.settings.cooler_temperature // nan
			|| context.settings.cooler_temperature < COOLER_MIN_TEMP
			|| context.settings.cooler_temperature > COOLER_MAX_TEMP)
	{
		context.settings.cooler_temperature = COOLER_DEF_TEMP;
		save_eeprom = 1;
	}
	if (context.settings.light_on_hour > 23
			|| context.settings.light_on_hour < 0
			|| context.settings.light_on_minute > 59
			|| context.settings.light_on_minute < 0
			|| context.settings.light_off_hour > 23
			|| context.settings.light_off_hour < 0
			|| context.settings.light_off_minute > 59
			|| context.settings.light_off_minute < 0
			|| (!context.settings.light_on_hour
					&& !context.settings.light_on_minute
					&& !context.settings.light_off_hour
					&& !context.settings.light_off_minute))
	{
		context.settings.light_on_hour = 7;
		context.settings.light_on_minute = 0;
		context.settings.light_off_hour = 20;
		context.settings.light_off_minute = 0;
		save_eeprom = 1;
	}

	if (context.settings.silent_on_hour > 23
			|| context.settings.silent_on_hour < 0
			|| context.settings.silent_on_minute > 59
			|| context.settings.silent_on_minute < 0
			|| context.settings.silent_off_hour > 23
			|| context.settings.silent_off_hour < 0
			|| context.settings.silent_off_minute > 59
			|| context.settings.silent_off_minute < 0
			|| (!context.settings.silent_on_hour
					&& !context.settings.silent_on_minute
					&& !context.settings.silent_off_hour
					&& !context.settings.silent_off_minute))
	{
		context.settings.silent_on_hour = 20;
		context.settings.silent_on_minute = 0;
		context.settings.silent_off_hour = 6;
		context.settings.silent_off_minute = 0;
		save_eeprom = 1;
	}

	if (save_eeprom)
	{
		context.ee24lc256.write_eeprom(0, sizeof(context.settings),
				(uint8_t *) &context.settings);
	}
}

void setup()
{
	if (MCUSR & _BV(WDRF))
	{
		error = 1;
	}
	MCUSR = 0;
	wdt_disable();

	context.lcd.init();
	context.lcd.backlight();

	context.gpio.setB2(1);
	context.gpio.setB4(1);

	context.pcf8583.get_clock(&context.displayInfo.datetime);
	if (context.displayInfo.datetime.year == 0)
	{
		context.displayInfo.datetime.year = 2013;
		context.displayInfo.datetime.month = 8;
		context.displayInfo.datetime.day = 19;
		context.displayInfo.datetime.hour = 21;
		context.displayInfo.datetime.minute = 30;
		context.displayInfo.datetime.second = 25;
		context.pcf8583.set_clock(&context.displayInfo.datetime);
	}

	init_settings();

	context.displayInfo.ir_event = 0;
	context.displayInfo.ir_key = 0;

	current_display = DISPLAY_WELCOME;

	// LCD backlight
	cli();
	lcd_timeout = LCD_TIMEOUT;
	display_timeout = DISPLAY_TIMEOUT;
	TCCR1A = 0;
	TCCR1B = 0;
	// TCNT1 = 3036; // 65536 - (16000000 Hz / 256 / 1 Hz)
	TCNT1 = 49911; // 65536 - (16000000 Hz / 256 / 4 Hz)
	TCCR1B = _BV(CS12);    // 256 prescaler
	TIMSK1 = _BV(TOIE1);   // timer overflow interrupt

	sei();
	wdt_enable(WDTO_8S);
	wdt_reset();
}

ISR(TIMER1_OVF_vect)
{
	cli();
	// TCNT1 = 3036; // 65536 - (16000000 Hz / 256 / 1 Hz)
	TCNT1 = 49911; // 65536 - (16000000 Hz / 256 / 4 Hz)
	if (t == 0)
	{
		if (lcd_timeout >= 0)
		{
			lcd_timeout--;
		}
		if (display_timeout >= 0)
		{
			display_timeout--;
		}
	}
	if (error)
	{
		context.gpio.setD6(t & 1);
	}
	t++;
	t &= 3;
	sei();
}

void loop()
{
	wdt_reset();
	if (context.ir.decode(&results))
	{
		context.ir.resume();
		cli();
		lcd_timeout = LCD_TIMEOUT;
		display_timeout = DISPLAY_TIMEOUT;
		sei();
		if (!context.displayInfo.ir_event)
		{
			uint16_t key = results.value & 0x7ff;
                        context.displayInfo.ir_key = key;
                        context.displayInfo.ir_event = 1;
		}
	}
	if (lcd_timeout >= 0)
	{
		context.lcd.backlight();
	}
	else
	{
		context.lcd.noBacklight();
	}

	context.pcf8583.get_clock(&context.displayInfo.datetime);
	float t = context.ds18b20.get_temperature();
	context.displayInfo.temperature = t;

	if (current_display != DISPLAY_MAIN)
	{
		cli();
		if (display_timeout < 0)
		{
			sei();
			for (uint8_t i = 0; i < (sizeof d / sizeof d[0]); i++)
			{
				Display * display = d[i];
				if (display->get_id() == current_display)
				{
					display->close();
					break;
				}
			}
			current_display = DISPLAY_MAIN;
			context.displayInfo.selected_menu = 0;
		}
		sei();
	}

	for (uint8_t i = 0; i < (sizeof d / sizeof d[0]); i++)
	{
		Display * display = d[i];
		if (display->get_id() == current_display)
		{
			if (last_display != current_display)
			{
				display->init();
				last_display = current_display;
			}
			else
			{
				current_display = display->action();
				if (last_display != current_display)
				{
					display->close();
					cli();
					lcd_timeout = LCD_TIMEOUT;
					display_timeout = DISPLAY_TIMEOUT;
					sei();

				}
			}
			break;
		}
	}

	if (last_display == current_display)
	{
		for (uint8_t i = 0; i < (sizeof d / sizeof d[0]); i++)
		{
			Display * display = d[i];
			if (display->get_id() == current_display)
			{
				context.lcd.clearBuffer();
				display->render();
				context.lcd.show();
				break;
			}
		}
	}

	_delay_ms(100);

}

int main(void)
{
	setup();
	for (;;)
	{
		loop();
	}
}

