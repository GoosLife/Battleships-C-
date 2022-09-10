#pragma once
#include "Grid.h"
#include "Ship.h"
#include "SDLGameObject.h"

class Enemy : public SDLGameObject
{
public:
	Enemy(const LoaderParams* pParams);
	void draw();
	void update();
	void clean();

private:
	Grid* m_grid;
	std::vector<Ship*> m_ships;
	Ship* m_currentShip;
	int m_currentShipIndex;

	bool m_setupComplete;

	// These functions might fit better on a enemy grid class inheriting from Grid
	// bool validatePosition();
	bool validateSquaresAreEmpty(int xCoord, int yCoord, bool isHorizontal, bool isVertical);
	void placeShip(int xCoord, int yCoord, bool isHorizontal, bool isVertical);
	void getNextShip();

	bool setupGrid();
};