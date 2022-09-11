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

	m_lives = 0;

	for (int i = 0; i < m_ships.size(); i++)
	{
		for (int j = 0; j < m_ships[i]->getSize(); j++)
		{
			m_lives++;
		}
	}

	m_setupComplete = false;
	
	m_isMyTurn = true;
}

void Player::draw()
{
	// SDLGameObject::draw();

	// Always render the players grid
	m_grid->render();

	// During the setup phase, render the boat the player is currently placing
	if (TheGame::Instance()->getCurrentState() == "SETUP")
	{
		m_currentShip->render();
	}

	// During the play phase
	if (TheGame::Instance()->getCurrentState() == "PLAY")
	{

	}
}

void Player::update()
{
	m_grid->update(); // Update the grid and get information 

	// During setup, update the currently placed ship
	if (TheGame::Instance()->getCurrentState() == "SETUP")
	{
		m_currentShip->update();
	}

	// During gameplay, update the cursor
	if (TheGame::Instance()->getCurrentState() == "PLAY")
	{

	}

	handleInput();
}

void Player::clean()
{
	delete m_grid;

	for (int i = 0; i < m_ships.size(); i++)
	{
		delete m_ships[i];
	}

	std::cout << m_currentShip->getSize() << '\n';

	SDLGameObject::clean();
}

void Player::beShotAt()
{
	// Generate random seed
	srand(time(NULL));

	int xPos;
	int yPos;

	// Enemy won't shoot at the same square twice
	do
	{
		xPos = rand() % m_grid->getWidth();
		yPos = rand() % m_grid->getHeight();
	} while (m_grid->getSquareState(xPos, yPos) == State::eState_Hit ||
		m_grid->getSquareState(xPos, yPos) == State::eState_Miss);

	// When a square that hasn't already been shot at is found, try shooting at it
	if (m_grid->shootSquare(xPos, yPos))
	{
		m_lives--;
		std::cout << "Player lives left: " << m_lives << '\n';
	}

	m_isMyTurn = true;

	if (m_lives == 0)
	{
		std::cout << "All player ships were destroyed. Enemy wins\n";
	}
}

void Player::handleInput()
{
	// INPUT HANDLING DURING THE SETUP PHASE
	if (TheGame::Instance()->getCurrentState() == "SETUP")
	{
		setupInput();
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
		if (m_grid->placeShip(*m_currentShip)) // Try to place the current ship on the grid
		{
			getNextShip();
		}
		TheInputHandler::Instance()->reset(); // Resets mouse button input - I only want to check for clicks,
											  // the user can't hold the mousebutton/drag the cursor.
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
		m_setupComplete = true;
	}
}
