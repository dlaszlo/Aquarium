#ifndef DISPLAYWELCOME_H_
#define DISPLAYWELCOME_H_

#include "Display.h"

class DisplayWelcome : public Display
{
public:
	DisplayWelcome(Context & _context) : Display(_context) {};
	void init();
	uint8_t action();
	void render();
	void close();
	uint8_t get_id();
};

#endif /* DISPLAYWELCOME_H_ */
