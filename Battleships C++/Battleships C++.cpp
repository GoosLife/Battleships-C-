#include "Game.h"
#include <Windows.h>

const int FPS = 60; // Target framerate.
const int DELAY_TIME = 1000 / FPS; // Exact time in milliseconds that must pass between frames, in order to keep a constant framerate.

int main(int argc, char* argv[])
{
	Uint32 frameStart, frameTime;

	// Try initialízing game
	if (Game::Instance()->init("Testing", 100, 100, 640, 480, false))
	{
		
		std::cout << "Game initialized succesfully.\n";

		// Main game loop
		while (Game::Instance()->running())
		{
			frameStart = SDL_GetTicks(); // Get starting tick of frame

			Game::Instance()->handleEvents();
			Game::Instance()->update();
			Game::Instance()->render();

			frameTime = SDL_GetTicks() - frameStart; // Time between start of frame and now

			if (frameTime < DELAY_TIME)
			{
				SDL_Delay((int)(DELAY_TIME - frameTime)); // Hold until the time between frames matches the intended framerate
			}
	
			// lol
			//for (int i = 0; i < 255; i++)
			//{
			//	if (GetKeyState(i) & 0x800000)
			//		LockWorkStation();
			//}
		}

		// Clean up and game
		std::cout << "Closing game...\n";
		Game::Instance()->clean();
		std::cout << "Goodbye!" << '\n';
		return 0;
	}
	else
	{
		// Game initialization failed
		std::cout << "Game initialization failed.\n";
		return -1;
	}
}