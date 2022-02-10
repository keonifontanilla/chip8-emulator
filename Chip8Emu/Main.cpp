// Chip8Emu.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Graphics.h"
#include "Chip8.h"

using Clock = std::chrono::high_resolution_clock;

int main(int argc, char **argv)
{
    const char* romFileName = argv[1];

    Graphics graphics(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    Chip8 chip8;
    chip8.LoadRom(romFileName);

    bool quit = false;

    auto startTime = Clock().now();
    auto t1 = Clock().now();
    int counter = 0;

    int fps = 60;
    int framerate = 1000 / fps;
    int instructionsPerStep = 15;

    while (!quit)
    {
        auto currentTime = Clock().now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - t1).count();
        auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();

        for (int i = 0; i < instructionsPerStep; ++i)
        {
            chip8.EmulateCycle();
            counter++;
            std::cout << "Cycle Count: " << counter << " TimePassed: " << time << std::endl;
        }

        if (timePassed >= framerate)
        {
            startTime = currentTime;
            chip8.TimerCycle();

            if (chip8.drawflag)
            {
                graphics.Update(chip8.display);
                chip8.drawflag = false;
                SDL_Delay(10);
            }

            quit = graphics.HandleEvents(chip8.key);
        }
    }

    return 0;
}
