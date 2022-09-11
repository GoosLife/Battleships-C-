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

	bool beShotAt();

	bool getIsMyTurn() { return m_isMyTurn; }
	void setIsMyTurn(bool value) { m_isMyTurn = value; }

private:
	Grid* m_grid;

	// SETUP PHASE
	std::vector<Ship*> m_ships;
	Ship* m_currentShip;
	int m_currentShipIndex;

	bool m_setupComplete;

	// These functions might fit better on the (enemy?) grid class (inheriting?) from Grid
	bool validateSquaresAreEmpty(int xCoord, int yCoord, bool isHorizontal, bool isVertical);
	void placeShip(int xCoord, int yCoord, bool isHorizontal, bool isVertical);
	void getNextShip();

	bool setupGrid();

	// PLAY PHASE
	int m_lives; // How many lives the enemy has left
	bool m_isMyTurn; // Whether or not it is the enemys turn

	bool hasBeenShotAt();
};