#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "../services/Context.h"

#define MAIN_DISPLAY 0

#define DISPLAY_WELCOME 0
#define DISPLAY_MAIN 1
#define DISPLAY_SETTINGS 2
#define DISPLAY_SETCLOCK 3
#define DISPLAY_SETCOOLER 4
#define DISPLAY_SETLIGHT 5
#define DISPLAY_SETSILENTHOURS 6

class Display
{
public:
	Display(Context & _context) : context(_context)
	{
	}

	virtual void init()
	{
		//
	}

	virtual uint8_t action()
	{
		return 0;
	}

	virtual void render()
	{
		//
	}

	virtual void close()
	{
		//
	}

	virtual uint8_t get_id()
	{
		return 0;
	}

protected:
	Context & context;
};

#endif /* DISPLAY_H_ */
