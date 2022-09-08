#pragma once
#include "Square.h"
#include "Game.h"

class Grid
{
public:
	Grid();
	Grid(int tileSize, int gridRows, int gridCols, bool hideOccupiedSquares);

	void update();
	void render();

	Square** getSquares() const { return m_squares; }
	int getTileSize() const { return m_tileSize; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }

private:
	int m_tileSize;
	bool m_hideOccupiedSquares;
	Square** m_squares;

	int m_width;
	int m_height;
};

