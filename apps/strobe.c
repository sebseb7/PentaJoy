#include <main.h>

static void init(void) ATTRIBUTES;
static uint8_t tick(void);

static void init(void) {
	registerAnimation(tick, 5, 60);
}

uint8_t tick() {
	static int8_t d = 1;

	for(uint8_t x = 0; x < LED_WIDTH * LED_HEIGHT; x++)
		setLedXY(x & 3, x >> 2, 7 * (d & 1));
	d++; 
	return 0;
}

