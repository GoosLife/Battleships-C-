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

private:
	void handleInput();

	Grid* m_grid;
	std::vector<Ship*> m_ships;
};

