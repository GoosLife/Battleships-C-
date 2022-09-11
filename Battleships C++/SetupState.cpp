#include "InputHandler.h"
#include "Player.h"
#include "PlayState.h"
#include "SetupState.h"

const std::string SetupState::s_setupID = "SETUP";

void SetupState::update()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->update();
	}
	
	// Exit state when player has placed all ships
	if (((Player*)m_gameObjects[0])->getSetupComplete())
	{
		TheGame::Instance()->getGameStateMachine()->changeState(new PlayState((Player*)m_gameObjects[0]));
	}

	//for (int i = 0; i < m_gameObjects.size(); i++)
	//{
	//	if (dynamic_cast<Player*>(m_gameObjects[i]))
	//		if (((Player*)m_gameObjects[i])->getSetupComplete())
	//			// Start the game with the player and grid that was created during the setup phase
	//			TheGame::Instance()->getGameStateMachine()->changeState(new PlayState(m_gameObjects[i]));
	//}
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

	TheInputHandler::Instance()->reset();

	return true;
}
bool SetupState::onExit()
{
	//for (int i = 0; i < m_gameObjects.size(); i++)
	//{
	//	m_gameObjects[i]->clean();
	//}

	m_gameObjects.clear();

	std::cout << "Exiting setup state...\n";

	return true;
}