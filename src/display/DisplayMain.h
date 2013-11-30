#ifndef DISPLAYMAIN_H_
#define DISPLAYMAIN_H_

#include "Display.h"

class DisplayMain : public Display
{
public:
	DisplayMain(Context & _context) : Display(_context) {};
	void init();
	uint8_t action();
	void render();
	uint8_t get_id();
	void close();
};

#endif /* DISPLAYMAIN_H_ */
