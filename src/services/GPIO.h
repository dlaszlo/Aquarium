#ifndef GPIO_H_
#define GPIO_H_

#define GPIO_ON  1
#define GPIO_OFF 0

class GPIO
{
public:
	GPIO();
	void GPIOB2(int state);
	void GPIOB3(int state);
	void GPIOB4(int state);
	void GPIOD6(int state);
};

#endif /* GPIO_H_ */
