#pragma once
#include "GameObject.h"
#include <SDL.h>

enum State
{
	eState_Empty, // Square is completely empty, no visual representation
	eState_Hover, // User is hovering over the square, such as when ready to place a boat or strike.
	eState_Occupied, // Users own occupied squares
	eState_Hit, // A ship has been hit on this square
	eState_Miss // User has tried this square but the opponent had no ship there.
};

class Square
{
public:
	Square();
	Square(int x, int y, int size);

	int getX() const { return m_xPos; }
	void setX(int x) { m_xPos = x; }

	int getY() const { return m_yPos; }
	void setY(int y) { m_yPos = y; }

	State getState() const { return m_state; }
	void setState(State s) { m_state = s; }

	SDL_Rect& getRect() { return squareRect; }

private:
	int m_xPos;
	int m_yPos;
	State m_state;

	SDL_Rect squareRect;
	GameObject* object;
};

