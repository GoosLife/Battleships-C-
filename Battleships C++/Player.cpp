#include <SDL.h>

#include "InputHandler.h"
#include "Player.h"

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

	m_isHorizontal = true;
	m_isVertical = false;
	m_xOffset = m_currentShip->getSize();
	m_yOffset = 0;

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
	// Convert the current position of the mouse to an X and Y coordinate on the grid
	int mouseX = (TheInputHandler::Instance()->getMousePosition()->getX() / m_grid->getTileSize()) - m_grid->getXPos();
	int mouseY = (TheInputHandler::Instance()->getMousePosition()->getY()/  m_grid->getTileSize()) - m_grid->getYPos();

	// The right mouse button changes direction of the placed ship
	if (TheInputHandler::Instance()->getMouseButtonState(RIGHT))
	{
		changeShipPlacementDirection();
		TheInputHandler::Instance()->reset(); // Reset mouse buttons now to check for clicks only
	}

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
	if (TheInputHandler::Instance()->getMouseButtonState(LEFT) && canPlaceShip)
	{
		placeShip(); 
		TheInputHandler::Instance()->reset(); // Resets mouse button input - I only want to check for clicks,
											  // the user can't hold the mousebutton/drag the cursor.
		getNextShip();
	}
}

void Player::changeShipPlacementDirection()
{
	m_isHorizontal = !m_isHorizontal; // Toggle m_isHorizontal
	m_isVertical = !m_isVertical; // Toggle m_isVertical

	// When toggling horizontal/vertical placement, the x and y offset swaps between 0 and the current ships size.
	// If the x offset is currently equal to the ships size, and m_isHorizontal is toggled, it must now be zero.
	// Since the y offset would then have been 0 before, it now becomes equal to the current ships size.
	m_xOffset = m_xOffset == m_currentShip->getSize() ? 0 : m_currentShip->getSize();
	m_yOffset = m_yOffset == m_currentShip->getSize() ? 0 : m_currentShip->getSize();
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
			if (m_grid->getSquares()[i][j].getState() == State::eState_Empty ||
				m_grid->getSquares()[i][j].getState() == State::eState_Hover)
			{
				m_grid->getSquares()[i][j].setState(State::eState_Empty);
			}
		}
	}
}

/// <summary>
/// Validate that a ship can be placed from where the mouse is currently positioned.
/// </summary>
/// <param name="mouseX"></param>
/// <param name="mouseY"></param>
/// <returns></returns>
bool Player::validatePosition(int mouseX, int mouseY)
{
	// If mouse is on the grid, check that the full length of the ship fits within the mouses' position to 
	// the edge of the grid.
	//
	// Dirty solution to a fix bug where during vertical placement, moving the mouse too far right will result in a crash. 
	// I have to find a better way to check for mouse placement/input
	//
	// Update: This is getting messier and messier,
	// I have no idea what the math behind this is anymore
	// I only know that these values work
	// and that I _NEED_ to figure out wtf I am doing here :')
	if ((m_isHorizontal &&
			mouseX + m_xOffset <= m_grid->getWidth() &&
			mouseY + m_yOffset <= m_grid->getHeight()) ||
		(m_isVertical &&
			mouseX + m_xOffset < m_grid->getWidth() &&
			mouseY + m_yOffset <= m_grid->getHeight()))
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
		if (!(m_grid->getSquares()[(mouseX) + (i * m_isHorizontal)][(mouseY) + (i * m_isVertical)].getState() == State::eState_Empty ||
			m_grid->getSquares()[(mouseX) + (i * m_isHorizontal)][(mouseY) + (i * m_isVertical)].getState() == State::eState_Hover))
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
		m_grid->getSquares()[(mouseX) + (i * m_isHorizontal)][(mouseY) + (i * m_isVertical)].setState(State::eState_Hover);
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
			if (m_grid->getSquares()[i][j].getState() == State::eState_Hover)
			{
				m_grid->getSquares()[i][j].setState(State::eState_Occupied);
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
	{
		m_currentShip = m_ships[++m_currentShipIndex];
		
		// Set the x or y offset to the new ships size
		m_xOffset = m_xOffset != 0 ? m_currentShip->getSize() : 0;
		m_yOffset = m_yOffset != 0 ? m_currentShip->getSize() : 0;
	}
	else
		m_setupComplete = true;
}
