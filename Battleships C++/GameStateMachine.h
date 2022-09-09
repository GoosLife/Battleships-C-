#pragma once
#include "GameState.h"
#include <vector>

class GameStateMachine
{
public:
	// Add a state without removing the current state (fx, pause menu, inventory menu etc).
	void pushState(GameState* pState);

	// Add a state while also removing the current state (fx, moving from main menu to play state).
	void changeState(GameState* pState);

	// Remove state without adding a new state.
	void popState();

	void update();
	void render();

	std::string getCurrentState() { return m_gameStates.front()->getStateID(); }

private:
	// Array containing all the currently active game states.
	std::vector<GameState*> m_gameStates;
};