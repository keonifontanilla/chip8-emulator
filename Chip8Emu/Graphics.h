#pragma once

#include <SDL.h>
#include <iostream>

class Graphics
{
public:
	Graphics(int windowHeight, int windowWidth);
	~Graphics();

	bool HandleEvents(uint8_t* key);
	void Update(uint8_t* display);

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	int scaleX = 12;
	int scaleY = 12;

	bool success = true;
};