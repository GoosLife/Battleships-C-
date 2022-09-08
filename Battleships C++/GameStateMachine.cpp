#include "GameStateMachine.h"
#include <iostream>

void GameStateMachine::pushState(GameState* pState)
{
	// Pushes the state to the back of the array, then enters the new state.
	m_gameStates.push_back(pState);
	m_gameStates.back()->onEnter();
}

void GameStateMachine::changeState(GameState* pState)
{
	if (!m_gameStates.empty())
	{
		// Check game state isn't the same as the currently active state.
		if (m_gameStates.back()->getStateID() == pState->getStateID())
		{
			std::cout << "WARNING: Tried changing gamestate (" << m_gameStates.back()->getStateID() << ") while it is already active.\n";
			return;
		}

		// Delete old gamestate.
		if (m_gameStates.back()->onExit())
		{
			delete m_gameStates.back();
			m_gameStates.pop_back();
		}
	}

	// Change gamestate to new gamestate
	m_gameStates.push_back(pState);
	m_gameStates.back()->onEnter();
}

void GameStateMachine::popState()
{
	if (!m_gameStates.empty())
	{
		if (m_gameStates.back()->onExit())
		{
			delete m_gameStates.back();
			m_gameStates.pop_back();
		}
	}
}

void GameStateMachine::update()
{
	if (!m_gameStates.empty())
	{
		m_gameStates.back()->update();
	}
}

void GameStateMachine::render()
{
	if (!m_gameStates.empty())
	{
		m_gameStates.back()->render();
	}
}