#pragma once
#include "Square.h"
#include "Game.h"
#include "Ship.h"

class Grid
{
public:
	Grid();
	Grid(int xPos, int yPos, int tileSize, int gridRows, int gridCols, bool hideOccupiedSquares);

	void update();
	void render();

	void placeShip(Ship ship);

	Square** getSquares() const { return m_squares; }
	int getTileSize() const { return m_tileSize; }
	int getXPos() const { return m_xPos; }
	int getYPos() const { return m_yPos; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }

private:
	int m_tileSize;
	bool m_hideOccupiedSquares;
	Square** m_squares;

	int m_xPos;
	int m_yPos;
	int m_width;
	int m_height;

	void resetSquares();
	bool validatePosition(Ship ship);
	bool validateSquaresAreEmpty(Ship ship);
};

