#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>

#include "main.h"

#define ZOOM 50

int leds[LED_HEIGHT][LED_WIDTH];
int interval;

uint16_t volatile current_adc_value = 900;

tick_fun tick_fp;
key_fun key_fp;

void setLedXY(uint8_t x, uint8_t y, uint8_t b) {
	assert(x < LED_WIDTH);
	assert(y < LED_HEIGHT);
	assert(b < 8);
	leds[y][x] = b;
}

void registerAnimation(tick_fun tick, uint16_t t, uint16_t ignore)
{
	tick_fp = tick;
	key_fp = NULL;
    
	assert(t > 0);
	// 122Hz / tick
	interval = 1000000 / 122 * t;
}
    
void registerApp(tick_fun tick, key_fun key, uint16_t t)
{
	tick_fp = tick;
	key_fp = key;
    
	assert(t > 0);
	interval = 1000000 / 122 * t;
}

void key_emit(key_type key, SDL_EventType type) {
	if(key_fp == NULL) return;

	event_type event;
	if(type == SDL_KEYDOWN) event = DOWN;
	else if(type == SDL_KEYUP) event = UP;
	else return;
	key_fp(key, event);
}

int main(int argc, char *argv[]) {
	srand(time(NULL));

	SDL_Surface* screen = SDL_SetVideoMode(LED_WIDTH * ZOOM, LED_HEIGHT * ZOOM,
		32, SDL_SWSURFACE | SDL_DOUBLEBUF);

	int running = 1;
	while(running) {
		SDL_Event ev;
		while(SDL_PollEvent(&ev)) {
			switch(ev.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = 0;
					break;

				case SDLK_q:
					key_emit(KEY_A, ev.type);
					break;

				case SDLK_w:
					key_emit(KEY_B, ev.type);
					break;

				case SDLK_e:
					key_emit(KEY_C, ev.type);
					break;

				case SDLK_r:
					key_emit(KEY_D, ev.type);
					break;

				default: break;
			}
		}

		running &= !tick_fp();

		const unsigned int color[] = {
			0x000000, 0x240000, 0x480000, 0x6c0000,
			0x900000, 0xb40000, 0xd90000, 0xff3f3f
		};
		int x, y;
		for(x = 0; x < LED_WIDTH; x++) {
			for(y = 0; y < LED_HEIGHT; y++) {

				SDL_Rect rect = {
					x * ZOOM + 3,
					(LED_HEIGHT - y - 1) * ZOOM + 3,
					ZOOM - 6,
					ZOOM - 6
				};

				SDL_FillRect(screen, &rect, color[leds[y][x]]);
			}
		}
		SDL_Flip(screen);

		usleep(interval);
	}

	SDL_Quit();
	return 0;
}

