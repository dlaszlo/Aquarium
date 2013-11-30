#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>
#include "DisplayWelcome.h"

void DisplayWelcome::init()
{
	//
}

uint8_t DisplayWelcome::action()
{
	_delay_ms(1000);
	return DISPLAY_MAIN;
}

void DisplayWelcome::render()
{
	context.lcd.center(0, "Aquarium");
	context.lcd.center(1, "v1.1");
}

void DisplayWelcome::close()
{
	//
}

uint8_t DisplayWelcome::get_id()
{
	return DISPLAY_WELCOME;
}
