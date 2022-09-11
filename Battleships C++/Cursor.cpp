#include "Cursor.h"
#include "Game.h"
#include "InputHandler.h"

Cursor::Cursor()
{
	update(); // Running the update function once actually initializes all of our variables
}

void Cursor::update() 
{
	calculatePosition();
	updateSquare();
}

void Cursor::render() 
{
	SDL_Color color = { 255,255,255 };

	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), color.r, color.g, color.b, 0);
	SDL_RenderFillRect(TheGame::Instance()->getRenderer(), &m_square.getRect());

	SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawRect(TheGame::Instance()->getRenderer(), &m_square.getRect());
}

void Cursor::handleInput() 
{
	// TODO: Implement handling shooting mechanics
}

void Cursor::calculatePosition()
{
	m_xPos = (TheInputHandler::Instance()->getMousePosition()->getX() / 32);
	m_yPos = (TheInputHandler::Instance()->getMousePosition()->getY() / 32);
}

void Cursor::updateSquare() 
{
	m_square = Square(m_xPos * 32, m_yPos * 32, 32);
}