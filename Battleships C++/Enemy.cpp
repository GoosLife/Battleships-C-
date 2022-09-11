#include "Enemy.h"
#include "InputHandler.h"
#include <SDL.h>


Enemy::Enemy(const LoaderParams* pParams) : SDLGameObject(pParams)
{
	// Create the computer's grid.
	m_grid = new Grid(11, 0, 32, 10, 10, true);

	// Create the computer's ships.
	m_ships.push_back(new Ship(eShipType_HangarShip));
	m_ships.push_back(new Ship(eShipType_Slagskib));
	m_ships.push_back(new Ship(eShipType_Destroyer));
	m_ships.push_back(new Ship(eShipType_Destroyer));
	m_ships.push_back(new Ship(eShipType_PatrolBoat));

	m_currentShipIndex = 0;
	m_currentShip = m_ships[m_currentShipIndex];

	m_setupComplete = false;

	m_lives = 0;

	for (int i = 0; i < m_ships.size(); i++)
	{
		for (int j = 0; j < m_ships[i]->getSize(); j++)
		{
			m_lives++;
		}
	}

	m_isMyTurn = false;

	// Place ships on computer's grid.
	setupGrid();
}

void Enemy::draw()
{
	m_grid->render();
}

void Enemy::update()
{
	m_grid->update();
}

void Enemy::clean() {}

bool Enemy::beShotAt()
{
	if (hasBeenShotAt())
	{
		m_isMyTurn = true;

		if (m_lives == 0)
		{
			std::cout << "All enemy ships were destroyed. Player wins\n";
		}

		return true;
	}
	return false;
}

/// <summary>
/// Takes input and checks if a valid shot has been taken at the enemy grid
/// </summary>
bool Enemy::hasBeenShotAt()
{
	if (TheInputHandler::Instance()->getMouseButtonState(LEFT))
	{
		int xPos = (TheInputHandler::Instance()->getMousePosition()->getX() / 32);
		int yPos = (TheInputHandler::Instance()->getMousePosition()->getY() / 32);

		TheInputHandler::Instance()->reset(); // Reset mouse button states to make sure we only check for clicks.

		// Get coordinate relative to the grids position
		xPos = xPos - m_grid->getXPos();
		yPos = yPos - m_grid->getYPos();

		// Check that coordinate is valid
		if (xPos >= 0 && xPos < m_grid->getWidth() &&
			yPos >= 0 && yPos < m_grid->getHeight())
		{
			// Shots won't register on squares that have already been shot at
			if (m_grid->getSquareState(xPos, yPos) == State::eState_Hit ||
				m_grid->getSquareState(xPos, yPos) == State::eState_Miss)
			{
				return false;
			}

			if (m_grid->shootSquare(xPos, yPos))
			{
				m_lives--;
				std::cout << "Enemy lives left: " << m_lives << '\n';
			}
			return true;
		}
	}

	return false;
}

// TODO: This is a horrible mess that should be re-done
bool Enemy::setupGrid()
{
	/*
	Phases for each ship:
		Determine placement direction (horizontal or vertical)
		Find out if ship fits on board
		Make sure no squares are already occupied
		Place the ship
	*/

	// Generate random seed for ship placement
	srand(time(NULL));

	for (int i = 0; i < m_ships.size(); i++)
	{
		int yOffset = 0;
		int xOffset = 0;

		// Determine placement direction
		bool isHorizontal = rand() % 2;

		// If 0 is drawn, place the ship horizontally, otherwise place it vertically
		if (isHorizontal == 1)
			xOffset = m_currentShip->getSize();
		else
			yOffset = m_currentShip->getSize();

		bool isVertical = !isHorizontal; // The answer to whether or not this ship will be placed vertically is logically
										 // the opposite of whether the ship will be placed horizontally

		// Determine a coordinate where the ship will fit on the board
		int xCoord = rand() % (m_grid->getWidth() - xOffset);
		int yCoord = rand() % (m_grid->getHeight() - yOffset);

		// Make sure no squares are already occupied
		while (!validateSquaresAreEmpty(xCoord, yCoord, isHorizontal, isVertical))
		{
			// DEBUG: Uncomment this to print enemy board to console while it is setting up
			//
			// std::cout << "NEXT ITERATION\n";
			//for (int i = 0; i < m_grid->getWidth(); i++)
			//{
			//	for (int j = 0; j < m_grid->getHeight(); j++)
			//	{
			//		std::cout << (m_grid->getSquares()[i][j].getState() == State::eState_Occupied ? "[1]" : "[0]");
			//	}
			//	std::cout << '\n';
			//}

			if (isHorizontal == 0) // For ships that are being placed horizontally, try the line below
				yCoord = yCoord + 1 >= m_grid->getHeight() ? 0 : yCoord + 1;
			else				// For ships that are being placed vertically, try the line to the right
				xCoord = xCoord + 1 >= m_grid->getWidth() ? 0 : xCoord + 1;
		}

		// Place the ship
		placeShip(xCoord, yCoord, isHorizontal, isVertical);

		// Get next ship
		getNextShip();
	}

	// DEBUG: Uncomment this to print the finished enemy board to console
	//
	// std::cout << "ENEMY BOARD\n";
	//for (int i = 0; i < m_grid->getWidth(); i++)
	//{
	//	for (int j = 0; j < m_grid->getHeight(); j++)
	//	{
	//		std::cout << (m_grid->getSquares()[i][j].getState() == State::eState_Occupied ? "[1]" : "[0]");
	//	}
	//	std::cout << '\n';
	//}

	return true;
}

// TODO: This could actually be on the grid base class, and be used on the player grid as well
bool Enemy::validateSquaresAreEmpty(int xCoord, int yCoord, bool isHorizontal, bool isVertical)
{
	// Check all squares to make sure that none of the squares for the ship are occupied
	for (int i = 0; i < m_currentShip->getSize(); i++)
	{
		if (!(m_grid->getSquares()[xCoord + (i * isHorizontal)][yCoord + (i * isVertical)].getState() == State::eState_Empty ||
			m_grid->getSquares()[xCoord + (i * isHorizontal)][yCoord + (i * isVertical)].getState() == State::eState_Hover))
		{
			// One of the squares was occupied - now we can't place the ship
			return false;
		}
	}
	return true;
}

void Enemy::placeShip(int xCoord, int yCoord, bool isHorizontal, bool isVertical)
{
	for (int i = 0; i < m_currentShip->getSize(); i++)
	{
		m_grid->getSquares()[xCoord + (i * isHorizontal)][yCoord + (i * isVertical)].setState(State::eState_Occupied);
	}
}

void Enemy::getNextShip()
{
	// Give user the next ship to place, or end the setup state
	if (m_currentShipIndex + 1 < m_ships.size())
	{
		m_currentShip = m_ships[++m_currentShipIndex];
	}
	// I shouldn't hit this in this class, since it should only run while there are ships to place,
	// however, for now, I'm gonna leave it in, just in case.
	else
		return;
}