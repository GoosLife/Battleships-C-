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

	m_player->update();
	m_enemy->update();

	if (m_player->getIsMyTurn())
	{
		if (m_enemy->beShotAt())
			m_player->setIsMyTurn(false);
	}

	if (m_enemy->getIsMyTurn())
	{
		m_player->beShotAt();
		m_enemy->setIsMyTurn(false);
	}
}

void PlayState::render()
{
	m_player->draw();
	m_enemy->draw();
}

bool PlayState::onEnter()
{
	m_enemy = new Enemy(new LoaderParams(0, 0, 0, 0, ""));

	std::cout << "Entering playstate...\n";
	return true;
}

bool PlayState::onExit()
{
	delete m_player;
	delete m_enemy;

	std::cout << "Exiting playstate...\n";
	return true;
}