// LiquidCrystal_I2C V2.0

#include "LCD.h"
#include "TWI.h"
#include <inttypes.h>
#include <string.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#define MAX_ROWS 2
#define MAX_COLS 16

char lcdbuff[MAX_ROWS][MAX_COLS] =
{ };

static const uint8_t charset_1[] PROGMEM =
		{ 0b00110,
		  0b01001,
		  0b01001,
		  0b00110,
		  0b00000,
		  0b00000,
		  0b00000,
		  0b00000};


// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

LCD::LCD(TWI & lcd_twi, uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) : _twi(lcd_twi)
{
	_Addr = lcd_Addr;
	_cols = lcd_cols;
	_rows = lcd_rows;
	_backlightval = LCD_NOBACKLIGHT;
}

void LCD::init()
{
	init_priv();
}

void LCD::init_priv()
{
	_twi.init();
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	begin(_cols, _rows);
	createChar(0, charset_1);

}

void LCD::begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
	if (lines > 1)
	{
		_displayfunction |= LCD_2LINE;
	}
	_numlines = lines;

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (lines == 1))
	{
		_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	_delay_us(50000);

	// Now we pull both RS and R/W low to begin commands
	expanderWrite(_backlightval);// reset expanderand turn backlight off (Bit 8 =1)
	_delay_ms(1000);

	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	// we start in 8bit mode, try to set 4 bit mode
	write4bits(0x03);
	_delay_us(4500); // wait min 4.1ms

	// second try
	write4bits(0x03);
	_delay_us(4500); // wait min 4.1ms

	// third go!
	write4bits(0x03);
	_delay_us(150);

	// finally, set to 4-bit interface
	write4bits(0x02);

	// set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();

	// clear it off
	clear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);

	home();

}

/********** high level commands, for the user! */
void LCD::clear()
{
	command(LCD_CLEARDISPLAY); // clear display, set cursor position to zero
	_delay_us(2000);  // this command takes a long time!
}

void LCD::home()
{
	command(LCD_RETURNHOME);  // set cursor position to zero
	_delay_us(2000);  // this command takes a long time!
}

void LCD::setCursor(uint8_t col, uint8_t row)
{
	int row_offsets[] =
	{ 0x00, 0x40, 0x14, 0x54 };
	if (row > _numlines)
	{
		row = _numlines - 1;    // we count rows starting w/0
	}
	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LCD::noDisplay()
{
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LCD::display()
{
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LCD::noCursor()
{
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LCD::cursor()
{
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LCD::noBlink()
{
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LCD::blink()
{
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LCD::scrollDisplayLeft(void)
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LCD::scrollDisplayRight(void)
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LCD::leftToRight(void)
{
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LCD::rightToLeft(void)
{
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LCD::autoscroll(void)
{
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LCD::noAutoscroll(void)
{
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LCD::createChar(uint8_t location, const uint8_t charmap[])
{
	location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i = 0; i < 8; i++)
	{
		write(pgm_read_byte(&charmap[i]));
	}
}

// Turn the (optional) backlight off/on
void LCD::noBacklight(void)
{
	_backlightval = LCD_NOBACKLIGHT;
	expanderWrite(0);
}

void LCD::backlight(void)
{
	_backlightval = LCD_BACKLIGHT;
	expanderWrite(0);
}

/*********** mid level commands, for sending data/cmds */

inline void LCD::command(uint8_t value)
{
	send(value, 0);
}

void LCD::write(char value)
{
	send(value, Rs);
}

/************ low level data pushing commands **********/

// write either command or data
void LCD::send(uint8_t value, uint8_t mode)
{
	uint8_t highnib = value >> 4;
	uint8_t lownib = value & 0x0F;
	write4bits((highnib) | mode);
	write4bits((lownib) | mode);
}

void LCD::write4bits(uint8_t value)
{
	expanderWrite(value);
	pulseEnable(value);
}

void LCD::expanderWrite(uint8_t _data)
{
	uint8_t s = _data | _backlightval;
	_twi.send(_Addr, &s, 1);
}

void LCD::pulseEnable(uint8_t _data)
{
	expanderWrite(_data | En);	// En high
	_delay_us(1);		// enable pulse must be >450ns

	expanderWrite(_data & ~En);	// En low
	_delay_us(50);		// commands need > 37us to settle
}

// Alias functions

void LCD::cursor_on()
{
	cursor();
}

void LCD::cursor_off()
{
	noCursor();
}

void LCD::blink_on()
{
	blink();
}

void LCD::blink_off()
{
	noBlink();
}

void LCD::load_custom_character(uint8_t char_num, uint8_t *rows)
{
	createChar(char_num, rows);
}

void LCD::setBacklight(uint8_t new_val)
{
	if (new_val)
	{
		backlight();		// turn backlight on
	}
	else
	{
		noBacklight();		// turn backlight off
	}
}

// unsupported API functions
void LCD::off()
{
}
void LCD::on()
{
}
void LCD::setDelay(int cmdDelay, int charDelay)
{
}
uint8_t LCD::status()
{
	return 0;
}
uint8_t LCD::keypad()
{
	return 0;
}
uint8_t LCD::init_bargraph(uint8_t graphtype)
{
	return 0;
}
void LCD::draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len,
		uint8_t pixel_col_end)
{
}
void LCD::draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,
		uint8_t pixel_row_end)
{
}
void LCD::setContrast(uint8_t new_val)
{
}

void LCD::clearBuffer()
{
	for (int i = 0; i < _rows; i++)
	{
		memset(lcdbuff[i], 32, _cols);
	}
}

void LCD::setText(uint8_t col, uint8_t row, const char * txt)
{
	if (row >= _rows)
	{
		return;
	}
	char * buff;
	buff = lcdbuff[row];
	uint8_t i0 = 0;
	for (uint8_t i1 = col; i1 < _cols; i1++)
	{
		if (!txt[i0])
		{
			break;
		}
		buff[i1] = txt[i0];
		i0++;
	}
}

void LCD::center(uint8_t row, const char * txt)
{
	if (row >= _rows)
	{
		return;
	}
	char * buff;
	buff = lcdbuff[row];
	size_t l = strlen(txt);
	uint8_t p = 0;
	uint8_t i = 0;
	if (l > _cols)
	{
		p = (l >> 1) - 8;
	}
	else
	{
		i = (_cols - l) >> 1;
	}
	for (; i < _cols; i++)
	{
		if (!txt[p])
		{
			break;
		}
		buff[i] = txt[p];
		p++;
	}
}

void LCD::right(uint8_t row, const char * txt)
{
	if (row >= _rows)
	{
		return;
	}
	char * buff;
	buff = lcdbuff[row];
	size_t l = strlen(txt);
	uint8_t p = 0;
	uint8_t i = 0;
	if (l > _cols)
	{
		p = l - _cols;
	}
	else
	{
		i = _cols - l;
	}
	for (; i < _cols; i++)
	{
		if (!txt[p])
		{
			break;
		}
		buff[i] = txt[p];
		p++;
	}
}

void LCD::show()
{
	for (int z = 0; z < _rows; z++)
	{
		setCursor(0, z);
		for (int i = 0; i < _cols; i++)
		{
			char c = lcdbuff[z][i];

			switch (c)
			{
			case '~':
				write(0x00);
				break;
			case '|':
				write(0xef);
				break;
			case '}':
				write(0xdc);
				break;
			case '{':
				write(0xdb);
				break;
			case 'á':
				write(0x61);
				break;
			case 'é':
				write(0x65);
				break;
			case 'í':
				write(0x69);
				break;
			case 'ó':
				write(0x6f);
				break;
			case 'õ':
				write(0x6f);
				break;
			case 'ú':
				write(0x75);
				break;
			case 'û':
				write(0x75);
				break;
			case 'ü':
				write(0x75);
				break;
			case 'ö':
				write(0x6f);
				break;
			default:
				write(c);
				break;
			}
		}
	}
}

