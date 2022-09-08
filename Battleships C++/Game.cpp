#include "Enemy.h"
#include "Game.h"
#include "InputHandler.h"
#include "Player.h"
#include "TextureManager.h"

#include "MenuState.h"
#include "SetupState.h"

Game* Game::s_pInstance = 0;

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "SDL initialization successful.\n";

		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if (m_pWindow != 0)
		{
			std::cout << "Window creation successful.\n";

			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

			if (m_pRenderer != 0)
			{
				std::cout << "Renderer creation successful.\n";
				SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
			}
			else
			{
				std::cout << "Renderer initialization failed.\n";
				return false;
			}
		}
		else
		{
			std::cout << "Window initialization failed.\n";
			return false;
		}
	}
	else
	{
		std::cout << "SDL initialization failed.\n";
		return false;
	}

	std::cout << "Initialization completed succesfully.\n";

	// Initialize game state machine
	m_pGameStateMachine = new GameStateMachine();
	m_pGameStateMachine->changeState(new MenuState());

	// Initialize inputhandler
	TheInputHandler::Instance()->initializeJoysticks();

	// Load sprites/textures
	TheTextureManager::Instance()->load("assets/animate_cut.png", "animate", m_pRenderer);

	// Add game object to list of game objects
	m_gameObjects.push_back(new Player(new LoaderParams(100,100,16,18,"animate")));
	m_gameObjects.push_back(new Enemy(new LoaderParams(0,0,16,18,"animate")));
	
	m_bRunning = true;

	return true;
}

void Game::update()
{
	m_pGameStateMachine->update();
}

void Game::render()
{
	SDL_RenderClear(m_pRenderer); // Clear the renderer to the draw color.

	m_pGameStateMachine->render(); // Render game objects based on current gamestate

	SDL_RenderPresent(m_pRenderer); // Draw the renderer to the screen.
}

void Game::handleEvents()
{
	TheInputHandler::Instance()->update();

	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN))
	{
		m_pGameStateMachine->changeState(new SetupState());
	}
}

void Game::clean()
{
	std::cout << "Cleaning game...\n";
	TheInputHandler::Instance()->clean();
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}

void Game::quit()
{
	m_bRunning = false;
}