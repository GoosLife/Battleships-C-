#include "PlayState.h"

#include "Enemy.h"
#include "Game.h"
#include "InputHandler.h"
#include "Player.h"
#include "PauseState.h"
#include "TextureManager.h"

#include <iostream>

const std::string PlayState::s_playID = "PLAY";

void PlayState::update()
{
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->getGameStateMachine()->pushState(new PauseState());
	}

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->update();
	}
}

void PlayState::render()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->draw();
	}

	for (int i = 0; i < m_grids.size(); i++)
	{
		m_grids[i]->render();
	}
}

bool PlayState::onEnter()
{
	Grid* g = new Grid(16, 10, 10, false);

	m_grids.push_back(g);

	std::cout << "Entering playstate...\n";
	return true;
}

bool PlayState::onExit()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->clean();
	}

	m_gameObjects.clear();

	TheTextureManager::Instance()->clearFromTextureMap("helicopter");

	std::cout << "Exiting playstate...\n";
	return true;
}
