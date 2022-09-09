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

	m_currentShipIndex = 0;
	m_currentShip = m_ships[m_currentShipIndex];

	m_setupComplete = false;
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
	// INPUT HANDLING DURING THE SETUP PHASE
	if (TheGame::Instance()->getCurrentState() == "SETUP")
	{
		setupGrid();
	}

	if (TheGame::Instance()->getCurrentState() == "PLAY")
	{
		
	}
}

/// <summary>
/// Handle input for setting up the player grid.
/// </summary>
void Player::setupGrid()
{
	// Save the current mouse position to int variables, to avoid making multiple calls to the input handler
	int mouseX = TheInputHandler::Instance()->getMousePosition()->getX();
	int mouseY = TheInputHandler::Instance()->getMousePosition()->getY();

	// Resets the grids squares, so that squares that are no longer hovered over become empty again.
	resetSquares();

	// Check if a ship can be placed
	bool canPlaceShip = validatePosition(mouseX, mouseY);
	
	// Mark all the potential squares as "hovered", to show the player where their ship will go if
	// they place it now.
	if (canPlaceShip)
	{
		markHoveredSquares(mouseX, mouseY);
	}

	// Place ship on mouseclick
	if (TheInputHandler::Instance()->getMouseButtonState(LEFT) &&
		canPlaceShip)
	{
		placeShip(); 
		TheInputHandler::Instance()->reset(); // Resets mouse button input - I only want to check for clicks,
											  // the user can't hold the mousebutton/drag the cursor.
		getNextShip();
	}
}

/// <summary>
/// Resets squares for next iteration
/// </summary>
void Player::resetSquares()
{
	for (int i = 0; i < m_grid->getWidth(); i++)
	{
		for (int j = 0; j < m_grid->getHeight(); j++)
		{
			// Squares that are empty or were hovered above before the mouse was moved away
			// are reset to empty squares
			if (m_grid->getSquares()[i][j].getState() == eState_Empty ||
				m_grid->getSquares()[i][j].getState() == eState_Hover)
			{
				m_grid->getSquares()[i][j].setState(eState_Empty);
			}
		}
	}
}

/// <summary>
/// Place a ship on the grid.
/// </summary>
void Player::placeShip()
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

/// <summary>
/// Get the next ship to place. If all ships have been placed, calling this ends the setup phase.
/// </summary>
void Player::getNextShip()
{
	// Give user the next ship to place, or end the setup state
	if (m_currentShipIndex + 1 < m_ships.size())
		m_currentShip = m_ships[++m_currentShipIndex];
	else
		m_setupComplete = true;
}

/// <summary>
/// Validate that a ship can be placed from where the mouse is currently positioned.
/// </summary>
/// <param name="mouseX"></param>
/// <param name="mouseY"></param>
/// <returns></returns>
bool Player::validatePosition(int mouseX, int mouseY)
{
	int offset = 0;

	while (((mouseX / m_grid->getTileSize()) + m_currentShip->getSize() - offset) > m_grid->getWidth())
		offset++;

	// If mouse is on the grid, check that the full length of the ship fits within the mouses' position to 
	// the edge of the grid.
	if ((mouseX / m_grid->getTileSize()) + m_currentShip->getSize() <= m_grid->getWidth() &&
		(mouseY / m_grid->getTileSize()) <= m_grid->getHeight())
	{
		// If any of the squares we want to check are occupied, we can't place the ship here.
		if (!validateSquaresAreEmpty(mouseX, mouseY))
			return false;
	}
	// The mouse was outside of the grid, or too close to the edge to fit the whole ship. The ship can't be placed.
	else
	{
		return false;
	}

	// We didn't hit any of the checks making the ships position invalid, return true
	return true;
}

/// <summary>
/// Validate whether any of the squares from the mouse position have an occupied ship in them.
/// </summary>
/// <param name="mouseX"></param>
/// <param name="mouseY"></param>
/// <returns></returns>
bool Player::validateSquaresAreEmpty(int mouseX, int mouseY)
{
	// Check all squares to make sure that none of the squares for the ship are occupied
	for (int i = 0; i < m_currentShip->getSize(); i++)
	{
		if (!(m_grid->getSquares()[(mouseX / m_grid->getTileSize()) + i][(mouseY / m_grid->getTileSize())].getState() == eState_Empty ||
			m_grid->getSquares()[(mouseX / m_grid->getTileSize()) + i][(mouseY / m_grid->getTileSize())].getState() == eState_Hover))
		{
			// One of the squares was occupied - now we can't place the ship
			return false;
		}
	}
	return true;
}

/// <summary>
/// Mark currently hovered squares if the position is valid, to indicate where a ship would be placed if
/// the player were to place it now.
/// </summary>
void Player::markHoveredSquares(int mouseX, int mouseY)
{
	for (int i = 0; i < m_currentShip->getSize(); i++)
		m_grid->getSquares()[(mouseX / m_grid->getTileSize()) + i][(mouseY / m_grid->getTileSize())].setState(eState_Hover);
}
