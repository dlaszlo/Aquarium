#ifndef DISPLAYSETTINGS_H_
#define DISPLAYSETTINGS_H_

#include "Display.h"

class DisplaySettings : public Display
{
public:
	DisplaySettings(Context & _context) : Display(_context) {};
	void init();
	uint8_t action();
	void render();
	uint8_t get_id();
	void close();
};

#endif /* DISPLAYSETTINGS_H_ */
