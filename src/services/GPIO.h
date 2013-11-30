#ifndef GPIO_H_
#define GPIO_H_

#define IO_ON  1
#define IO_OFF 0

class GPIO
{
public:
	GPIO();
	void setB2(int state);
	void setB3(int state);
	void setB4(int state);
	void setD6(int state);
	uint8_t getD7();
};

#endif /* GPIO_H_ */
