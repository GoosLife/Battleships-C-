#pragma once
#include "Grid.h"
#include "SDLGameObject.h"
#include "Ship.h"
#include <vector>.

class Player : public SDLGameObject
{
public:
	Player(const LoaderParams* pParams);
	void draw();
	void update();
	void clean();

	bool getSetupComplete() const { return m_setupComplete; }

private:
	void handleInput();

	Grid* m_grid;
	std::vector<Ship*> m_ships;
	Ship* m_currentShip;
	int m_currentShipIndex;

	void setupGrid();
	void resetSquares();
	void placeShip();
	void getNextShip();
	bool validatePosition(int mouseX, int mouseY);
	bool validateSquaresAreEmpty(int mouseX, int mouseY);
	void markHoveredSquares(int mouseX, int mouseY);

	bool m_setupComplete;
};

