#ifndef DISPLAYSETCOOLER_H_
#define DISPLAYSETCOOLER_H_

#include "Display.h"

class DisplaySetCooler : public Display
{
public:
	DisplaySetCooler(Context & _context) : Display(_context) {};
	void init();
	uint8_t action();
	void render();
	uint8_t get_id();
	void close();
};

#endif /* DISPLAYSETCOOLER_H_ */
