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
}

bool PlayState::onEnter()
{
	GameObject* enemy = new Enemy(new LoaderParams(0, 0, 0, 0, ""));
	m_gameObjects.push_back(enemy);

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

	std::cout << "Exiting playstate...\n";
	return true;
}
