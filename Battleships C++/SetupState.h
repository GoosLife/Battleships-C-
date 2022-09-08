#pragma once
#include "GameState.h"
#include "Grid.h"
#include <vector>

class SetupState : public GameState
{
public:
	virtual void update();
	virtual void render();
	
	virtual bool onEnter();
	virtual bool onExit();

	virtual std::string getStateID() const { return s_setupID; }

private:
	static const std::string s_setupID;
	std::vector<GameObject*> m_gameObjects;
	std::vector<Grid*> m_grids;
};