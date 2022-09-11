#include "Game.h"
#include "MenuButton.h"
#include "MenuState.h"
#include "PlayState.h"
#include "SetupState.h"
#include "TextureManager.h"
#include <iostream>

const std::string MenuState::s_menuID = "MENU";

void MenuState::update() 
{	
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->update();
	}
}

void MenuState::render()
{
	// Calling the setup state from the menu button in this loop can sometimes cause
	// the game to crash, as it tries to continue running through this loop after the
	// menu has been deleted, causing a read access violation.
	// TODO: fix


	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->draw();
	}
}

bool MenuState::onEnter()
{
	if (!TheTextureManager::Instance()->load("assets/btnPlay.png", "playbutton", TheGame::Instance()->getRenderer()))
	{
		return false;
	}

	if (!TheTextureManager::Instance()->load("assets/btnExit.png", "exitbutton", TheGame::Instance()->getRenderer()))
	{
		return false;
	}

	GameObject* button1 = new MenuButton(new LoaderParams(100, 100, 400, 100, "playbutton"), s_menuToPlay);
	GameObject* button2 = new MenuButton(new LoaderParams(100, 300, 400, 100, "exitbutton"), s_exitFromMenu);

	m_gameObjects.push_back(button1);
	m_gameObjects.push_back(button2);

	std::cout << "Entering menu state...\n";

	return true;
}

bool MenuState::onExit()
{
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->clean();
	}

	m_gameObjects.clear();

	TheTextureManager::Instance()->clearFromTextureMap("playbutton");
	TheTextureManager::Instance()->clearFromTextureMap("exitbutton");

	std::cout << "Exiting menu state...\n";
	return true;
}

void MenuState::s_menuToPlay()
{
	TheGame::Instance()->getGameStateMachine()->changeState(new SetupState());
}

void MenuState::s_exitFromMenu()
{
	TheGame::Instance()->quit();
}
