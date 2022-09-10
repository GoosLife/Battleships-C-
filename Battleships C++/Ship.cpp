#include "Ship.h"
#include "Game.h"
#include "InputHandler.h"

Ship::Ship(ShipType t) : m_type(t), m_size((int)t) 
{
	m_isHorizontal = true;
	m_isVertical = false;

	m_squares = new Square[m_size];

	for (int i = 0; i < m_size; i++)
	{
		m_squares[i] = Square(((i + m_xPos) * 32) * m_isHorizontal, ((i + m_yPos) * 32) * m_isVertical, 32);
	}
}

void Ship::update()
{
	// 32 = Tilesize

	// TODO: Maybe make tilesize a variable of the game, to make it accessible anywhere
	m_xPos = (TheInputHandler::Instance()->getMousePosition()->getX() / 32);
	m_yPos = (TheInputHandler::Instance()->getMousePosition()->getY() / 32);

	updateSquares();

	handleInput();
}

void Ship::render()
{

	SDL_Color color = { 166,166,166 };

	for (int i = 0; i < m_size; i++)
	{
		SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), color.r, color.g, color.b, 0);
		SDL_RenderFillRect(TheGame::Instance()->getRenderer(), &m_squares[i].getRect());

		SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 0, 0, 0, 255);
		SDL_RenderDrawRect(TheGame::Instance()->getRenderer(), &m_squares[i].getRect());
	}
}

void Ship::handleInput()
{
	// The right mouse button changes direction of the placed ship
	if (TheInputHandler::Instance()->getMouseButtonState(RIGHT))
	{
		changeDirection();
		TheInputHandler::Instance()->reset(); // Reset mouse buttons now to check for clicks only
	}
}

void Ship::updateSquares()
{
	for (int i = 0; i < m_size; i++)
	{
		m_squares[i] = Square(((m_xPos + (i * m_isHorizontal)) * (32)), ((m_yPos + (i * m_isVertical)) * (32)), 32);
	}
}

void Ship::changeDirection()
{
	m_isHorizontal = !m_isHorizontal; // Toggle m_isHorizontal
	m_isVertical = !m_isVertical; // Toggle m_isVertical
}