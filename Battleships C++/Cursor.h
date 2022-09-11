#pragma once
#include "Square.h"

// TODO: I've considered using this class instead of the ship class to also handle placing ships.
// So I've prepared some things in here that I've commented out and might come back to if I have time.
class Cursor
{
public:
	Cursor();

	//int getSize() const { return m_size; }
	int getXPos() const { return m_xPos; }
	int getYPos() const { return m_yPos; }
	//bool getIsHorizontal() const { return m_isHorizontal; }
	//bool getIsVertical() const { return m_isVertical; }

	void update();
	void render();
	void handleInput();

private:
	//int m_size;
	int m_xPos;
	int m_yPos;

	// Square* m_squares;
	Square m_square;

	//bool m_isHorizontal;
	//bool m_isVertical;

	void calculatePosition();
	void updateSquare();
	// void changeDirection();
};

