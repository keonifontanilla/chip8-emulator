// Chip8Emu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Graphics.h"
#include "Chip8.h"

int main(int argc, char **argv)
{
    const char* romFileName = argv[1];

    Graphics graphics(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    Chip8 chip8;
    chip8.LoadRom(romFileName);

    bool quit = false;

    while (!quit)
    {
        quit = graphics.HandleEvents(chip8.key);

        chip8.EmulateCycle();

        if (chip8.drawflag)
        {
            graphics.Update(chip8.display);
            chip8.drawflag = false;
            SDL_Delay(10);
        }
    }

    return 0;
}
