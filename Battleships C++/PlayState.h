#pragma once
#include "Cursor.h"
#include "GameObject.h"
#include "GameState.h"
#include "Grid.h"
#include <vector>

class PlayState : public GameState
{
public:
	PlayState(GameObject* player) { m_gameObjects.push_back(player); }
	virtual void update();
	virtual void render();

	virtual bool onEnter();
	virtual bool onExit();

	virtual std::string getStateID() const { return s_playID; }

private:
	static const std::string s_playID;
	std::vector<GameObject*> m_gameObjects;
	Cursor m_cursor;
};

