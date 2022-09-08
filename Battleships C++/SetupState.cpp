#include "InputHandler.h"
#include "Player.h"
#include "SetupState.h"

const std::string SetupState::s_setupID = "SETUP";

void SetupState::update()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->update();
	}
}

void SetupState::render()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->draw();
	}
}

bool SetupState::onEnter()
{
	Player* player = new Player(new LoaderParams(0, 0, 0, 0, ""));
	m_gameObjects.push_back(player);

	std::cout << "Entering setup state...\n";

	return true;
}
bool SetupState::onExit() { return true;  }
