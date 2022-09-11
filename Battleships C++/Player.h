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

	void beShotAt();

	bool getSetupComplete() { return m_setupComplete; }
	bool getIsMyTurn() { return m_isMyTurn; }
	void setIsMyTurn(bool value) { m_isMyTurn = value; }

private:
	void handleInput();

	Grid* m_grid;
	std::vector<Ship*> m_ships;

	// Setup phase
	Ship* m_currentShip;
	int m_currentShipIndex;
	bool m_setupComplete;
	
	void setupInput();
	void getNextShip();

	// Play phase
	int m_lives;
	bool m_isMyTurn;
};

