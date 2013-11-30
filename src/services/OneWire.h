#ifndef OneWire_h
#define OneWire_h

#define ONEWIRE_PIN			PINB
#define ONEWIRE_DDR			DDRB
#define ONEWIRE_PN			1

class OneWire
{
public:
	OneWire();

	// Perform a 1-Wire reset cycle
	uint8_t reset(void);

	// Read a byte
	uint8_t read(void);

	// Write a byte
	void write(uint8_t byte);

private:
	// Read a bit
	uint8_t readBit(void);

	// Write a bit
	void writeBit(uint8_t bit);
};

#endif
