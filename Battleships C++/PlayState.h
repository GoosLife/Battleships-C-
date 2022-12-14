#pragma once
#include "Enemy.h"
#include "GameObject.h"
#include "GameState.h"
#include "Grid.h"
#include "Player.h"
#include <vector>

class PlayState : public GameState
{
public:
	PlayState(Player* player) : m_player(player) {}
	virtual void update();
	virtual void render();

	virtual bool onEnter();
	virtual bool onExit();

	virtual std::string getStateID() const { return s_playID; }

private:
	static const std::string s_playID;
	std::vector<GameObject*> m_gameObjects;

	Player* m_player;
	Enemy* m_enemy;
};

