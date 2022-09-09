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
	// There shouldn't ever be more than 2 gameobjects in the menu, however, sometimes there appears to be.
	// I'm not sure why, but maybe this will catch it?
	// I will keep an eye out for this message in the console.
	if (m_gameObjects.size() > 2)
	{
		std::cout << "(from MenuState::update(): I had a crazy amount of objects in here lol\n";
		m_gameObjects.clear();
		return;
	}

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->update();
	}
}

void MenuState::render()
{
	// There shouldn't ever be more than 2 gameobjects in the menu, however, sometimes there appears to be.
	// I'm not sure why, but maybe this will catch it?
	// I will keep an eye out for this message in the console.
	if (m_gameObjects.size() > 2)
	{
		std::cout << "(from MenuState::render(): I had a crazy amount of objects in here lol\n";
		m_gameObjects.clear();
		return;
	}

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

	if (m_gameObjects.size() > 2)
	{
		std::cout << "WTF I put SO MANY objects in THIS LIST what the HELL is HAPPENING" << '\n';
	}

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
