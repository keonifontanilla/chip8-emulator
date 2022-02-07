#include "Graphics.h"

Graphics::Graphics(int windowWidth, int windowHeight)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError() << std::endl;
		success = false;
	}
	else 
	{
		window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth * scaleX, windowHeight * scaleY, SDL_WINDOW_SHOWN);
		if (window == nullptr)
		{
			std::cout << "Window could not be created! SDL_Error: %s\n" << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			SDL_RenderSetScale(renderer, scaleX, scaleY);
			if (renderer == nullptr)
			{
				std::cout << "Renderer could not be created! SDL Error: %s\n" << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
	}
}

Graphics::~Graphics()
{
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_Quit();
}

bool Graphics::HandleEvents(uint8_t* key)
{
	SDL_Event e;
	bool quit = false;

	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
			case SDL_QUIT:
				quit = true;
				std::cout << "QUITTING" << std::endl;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
					case SDLK_ESCAPE: quit = true; break;
					case SDLK_x: key[0x0] = 1; break;
					case SDLK_1: key[0x1] = 1; break;
					case SDLK_2: key[0x2] = 1; break;
					case SDLK_3: key[0x3] = 1; break;
					case SDLK_4: key[0xC] = 1; break;
					case SDLK_q: key[0x4] = 1; break;
					case SDLK_w: key[0x5] = 1; break;
					case SDLK_e: key[0x6] = 1; break;
					case SDLK_r: key[0xD] = 1; break;
					case SDLK_a: key[0x7] = 1; break;
					case SDLK_s: key[0x8] = 1; break;
					case SDLK_d: key[0x9] = 1; break;
					case SDLK_f: key[0xE] = 1; break;
					case SDLK_z: key[0xA] = 1; break;
					case SDLK_c: key[0xB] = 1; break;
					case SDLK_v: key[0xF] = 1; break;
				}
				break;
			case SDL_KEYUP:
				switch (e.key.keysym.sym)
				{
					case SDLK_x: key[0x0] = 0; break;
					case SDLK_1: key[0x1] = 0; break;
					case SDLK_2: key[0x2] = 0; break;
					case SDLK_3: key[0x3] = 0; break;
					case SDLK_4: key[0xC] = 0; break;
					case SDLK_q: key[0x4] = 0; break;
					case SDLK_w: key[0x5] = 0; break;
					case SDLK_e: key[0x6] = 0; break;
					case SDLK_r: key[0xD] = 0; break;
					case SDLK_a: key[0x7] = 0; break;
					case SDLK_s: key[0x8] = 0; break;
					case SDLK_d: key[0x9] = 0; break;
					case SDLK_f: key[0xE] = 0; break;
					case SDLK_z: key[0xA] = 0; break;
					case SDLK_c: key[0xB] = 0; break;
					case SDLK_v: key[0xF] = 0; break;
				}
				break;
		}
	}

	return quit;
}

void Graphics::Update(uint8_t* display)
{

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	for (int y = 0; y < 32; ++y)
	{
		for (int x = 0; x < 64; ++x)
		{
			if (display[(y * 64) + x] != 0)
			{
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
	}

	SDL_RenderPresent(renderer);
}
