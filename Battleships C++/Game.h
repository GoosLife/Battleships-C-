#pragma once
#include <iostream>
#include <SDL.h>
#include <vector>

#include "GameObject.h"
#include "GameStateMachine.h"

class SDLGameObject;

class Game
{
public:
	static Game* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new Game();
			return s_pInstance;
		}
		return s_pInstance;
	}
	~Game() {}

	bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	
	void render();
	void update();
	void handleEvents();
	void clean();

	void quit();

	// Access private variables

	bool running() { return m_bRunning; }
	SDL_Renderer* getRenderer() { return m_pRenderer; }
	GameStateMachine* getGameStateMachine() { return m_pGameStateMachine; }

private:

	Game() {}
	static Game* s_pInstance;

	SDL_Window* m_pWindow{NULL}; // Set to null to initialize member variable, will be set in init();
	SDL_Renderer* m_pRenderer{NULL}; // Set to null to initialize member variable, will be set in init();

	// Game state handling
	GameStateMachine* m_pGameStateMachine;

	// Graphics stuff

	int m_currentFrame{0};

	// Game objects
	
	std::vector<GameObject*> m_gameObjects;

	// Is the game running

	bool m_bRunning{true};
};
	typedef Game TheGame;