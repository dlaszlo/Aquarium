#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "PCF8583.h"
#include "LCD.h"
#include "TWI.h"
#include "IRremote.h"
#include "GPIO.h"
#include "DS18B20.h"
#include "EE24LC256.h"

#define COOLER_MIN_TEMP 15
#define COOLER_MAX_TEMP 35
#define COOLER_DEF_TEMP 25.4

typedef struct
{
	uint8_t ir_event;
	uint16_t ir_key;
	float temperature;
	PCF8583_t datetime;
	uint8_t selected_menu;
} displayInfo_t;

typedef struct
{
	float cooler_temperature;
	int8_t light_on_hour;
	int8_t light_on_minute;
	int8_t light_off_hour;
	int8_t light_off_minute;
	int8_t silent_on_hour;
	int8_t silent_on_minute;
	int8_t silent_off_hour;
	int8_t silent_off_minute;
} settings_t;

class Context
{
public:

	Context() : twi(), lcd(twi, 0x38, 16, 2), pcf8583(twi, 0x50), ee24lc256(twi, 0x57), ir(), gpio(), oneWire(), ds18b20(oneWire) {};

	TWI twi;
	LCD lcd;
	PCF8583 pcf8583;
	EE24LC256 ee24lc256;
	IRremote ir;
	GPIO gpio;
	OneWire oneWire;
	DS18B20 ds18b20;

	displayInfo_t displayInfo;
	settings_t settings;
};

#endif /* CONTEXT_H_ */
