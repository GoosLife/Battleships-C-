#include <SDL.h>

#include "InputHandler.h"
#include "Player.h"
#include "PlayState.h"

#include <iostream>

Player::Player(const LoaderParams* pParams) : SDLGameObject(pParams) 
{
	// Create the players grid
	m_grid = new Grid(0, 0, 32, 10, 10, false);

	// Create the players ships
	m_ships.push_back(new Ship(eShipType_HangarShip));
	m_ships.push_back(new Ship(eShipType_Slagskib));
	m_ships.push_back(new Ship(eShipType_Destroyer));
	m_ships.push_back(new Ship(eShipType_Destroyer));
	m_ships.push_back(new Ship(eShipType_PatrolBoat));

	m_currentShipIndex = 0;
	m_currentShip = m_ships[m_currentShipIndex];
}

void Player::draw() 
{
	// SDLGameObject::draw();
	m_grid->render();
	m_currentShip->render();
}

void Player::update() 
{
	m_grid->update(); // Update the grid and get information 
	m_currentShip->update();
	handleInput();
}

void Player::clean() 
{
	delete m_grid;

	for (int i = 0; i < m_ships.size(); i++)
	{
		delete m_ships[i];
	}
	
	SDLGameObject::clean();
}

void Player::handleInput()
{
	// INPUT HANDLING DURING THE SETUP PHASE
	if (TheGame::Instance()->getCurrentState() == "SETUP")
	{
		setupInput();
	}

	if (TheGame::Instance()->getCurrentState() == "PLAY")
	{
		
	}
}

/// <summary>
/// Handle input for setting up the player grid.
/// </summary>
void Player::setupInput()
{
	// Place ship on mouseclick
	if (TheInputHandler::Instance()->getMouseButtonState(LEFT))
	{
		m_grid->placeShip(*m_currentShip); // Try to place the current ship on the grid
		TheInputHandler::Instance()->reset(); // Resets mouse button input - I only want to check for clicks,
											  // the user can't hold the mousebutton/drag the cursor.
		getNextShip();
	}
}

/// <summary>
/// Get the next ship to place. If all ships have been placed, calling this ends the setup phase.
/// </summary>
void Player::getNextShip()
{
	// Give user the next ship to place, or end the setup state
	if (m_currentShipIndex + 1 < m_ships.size())
	{
		m_currentShip = m_ships[++m_currentShipIndex];
	}
	else
	{
		TheGame::Instance()->getGameStateMachine()->changeState(new PlayState(this));
	}
}
