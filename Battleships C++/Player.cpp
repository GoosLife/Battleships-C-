#include <SDL.h>

#include "InputHandler.h"
#include "Player.h"

#include <iostream>

Player::Player(const LoaderParams* pParams) : SDLGameObject(pParams) 
{
	// Create the players grid
	m_grid = new Grid(32, 10, 10, false);

	// Create the players ships
	m_ships.push_back(new Ship(eShipType_HangarShip));
	m_ships.push_back(new Ship(eShipType_Slagskib));
	m_ships.push_back(new Ship(eShipType_Destroyer));
	m_ships.push_back(new Ship(eShipType_Destroyer));
	m_ships.push_back(new Ship(eShipType_PatrolBoat));
}

void Player::draw() 
{
	// SDLGameObject::draw();
	m_grid->render();
}

void Player::update() 
{
	m_grid->update();
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
	// Get mouse positions & initial state (to check for click events)
	int mouseX = TheInputHandler::Instance()->getMousePosition()->getX();
	int mouseY = TheInputHandler::Instance()->getMousePosition()->getY();

	// Update state of the players board
	for (int i = 0; i < m_grid->getWidth(); i++)
	{
		for (int j = 0; j < m_grid->getHeight(); j++)
		{
			// Squares that the mouse is hovering over, and are currently empty, will enter 
			// the hovered state
			if ( mouseX / m_grid->getTileSize() == i &&
				 mouseY / m_grid->getTileSize() == j &&
				(m_grid->getSquares()[i][j].getState() == eState_Empty ||
				 m_grid->getSquares()[i][j].getState() == eState_Hover))
			{
				m_grid->getSquares()[i][j].setState(eState_Hover);
			}
			// Squares that are empty or were hovered above before the mouse was moved away
			// are reset to empty squares
			else if (m_grid->getSquares()[i][j].getState() == eState_Empty ||
					 m_grid->getSquares()[i][j].getState() == eState_Hover)
			{
				m_grid->getSquares()[i][j].setState(eState_Empty);
			}
		}
	}

	// Place ship
	if (TheInputHandler::Instance()->getMouseButtonState(LEFT))
	{
		for (int i = 0; i < m_grid->getWidth(); i++)
		{
			for (int j = 0; j < m_grid->getHeight(); j++)
			{
				if (m_grid->getSquares()[i][j].getState() == eState_Hover)
				{
					m_grid->getSquares()[i][j].setState(eState_Occupied);
				}
			}
		}
	}
}