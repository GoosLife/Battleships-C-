#include "InputHandler.h"
#include "Game.h"
#include <iostream>

InputHandler* InputHandler::s_pInstance = 0;

void InputHandler::onKeyDown()
{
	m_keystates = SDL_GetKeyboardState(NULL);
}

void InputHandler::onKeyUp()
{
	m_keystates = SDL_GetKeyboardState(NULL);
}

InputHandler::InputHandler()
{
	m_mousePosition = new Vector2D(0.0f, 0.0f); // Initialize mouse position
	m_bJoysticksInitialized = false;

	// Set initial mouse button states to "up"
	for (int i = 0; i < 3; i++) 
	{
		m_mouseButtonStates.push_back(false);
	}
}

InputHandler::~InputHandler() {}

void InputHandler::initializeJoysticks()
{
	// Initialize joystick subsystem if not done already
	if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0) 
	{
		SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	}

	if (SDL_NumJoysticks() > 0) 
	{
		// Open each available joystick
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			SDL_Joystick* joy = SDL_JoystickOpen(i);

			// Check that SDL_JoystickOpen didn't return null pointer before adding joystick to our vector of joysticks
			if (joy)
			{
				m_joysticks.push_back(joy);
				m_joystickValues.push_back(std::make_pair(new Vector2D(0.0f, 0.0f), new Vector2D(0.0f, 0.0f))); // Add pair of sticks from joystick

				// Temporary array to store all the buttons found on this joystick.
				std::vector<bool> tempButtons;

				// Add joystick button states.
				for (int j = 0; j < SDL_JoystickNumButtons(joy); j++) 
				{
					tempButtons.push_back(false);
				}

				// Add to the array of all controllers' button states
				m_buttonStates.push_back(tempButtons);
			}
			else 
			{
				std::cout << SDL_GetError(); // In case of error, print error to console
			}
		}
		SDL_JoystickEventState(SDL_ENABLE); // Tell SDL to start listening for joystick events
		m_bJoysticksInitialized = true; // Lets us know that joysticks were initialized succesfully.

		std::cout << "Initialized " << m_joysticks.size() << " joystick(s)" << '\n'; // Print success statement to console for fast and easy overview..
	}
	else
	{
		m_bJoysticksInitialized = false; // Joystick initialization flag set to false because no joysticks were detected/initialized.
		std::cout << "Joystick initialization aborted (no joysticks detected).\n"; // Print failure to find joysticks to console.
	}
}

bool InputHandler::joysticksInitialized()
{
	return m_bJoysticksInitialized;
}

// Close each joystick that has been opened during initialization
void InputHandler::clean()
{
	if (m_bJoysticksInitialized)
	{
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			SDL_JoystickClose(m_joysticks[i]);
		}
	}
}

void InputHandler::onJoystickAxisMove(SDL_Event& event)
{
	int joy = event.jaxis.which; // Which joystick is sending the event?

	// Left stick x movement 
	if (event.jaxis.axis == 0)
	{
		if (event.jaxis.value > m_joystickDeadZone)
		{
			m_joystickValues[joy].first->setX(1);
		}
		else if (event.jaxis.value < -m_joystickDeadZone)
		{
			m_joystickValues[joy].first->setX(-1);
		}
		else
		{
			m_joystickValues[joy].first->setX(0); // Stops game from detecting stick movement unless the stick is moved outside of the deadzone.
		}
	}

	// Left stick y movement
	if (event.jaxis.axis == 1)
	{
		if (event.jaxis.value > m_joystickDeadZone)
		{
			m_joystickValues[joy].first->setY(1);
		}
		else if (event.jaxis.value < -m_joystickDeadZone)
		{
			m_joystickValues[joy].first->setY(-1);
		}
		else
		{
			m_joystickValues[joy].first->setY(0); // Stops game from detecting stick movement unless the stick is moved outside of the deadzone.
		}
	}

	// Right stick x movement
	if (event.jaxis.axis == 3)
	{
		if (event.jaxis.value > m_joystickDeadZone)
		{
			m_joystickValues[joy].second->setX(1);
		}
		else if (event.jaxis.value < -m_joystickDeadZone)
		{
			m_joystickValues[joy].second->setX(-1);
		}
		else
		{
			m_joystickValues[joy].second->setX(0); // Stops game from detecting stick movement unless the stick is moved outside of the deadzone.
		}
	}

	// Right stick y movement
	if (event.jaxis.axis == 4)
	{
		if (event.jaxis.value > m_joystickDeadZone)
		{
			m_joystickValues[joy].second->setY(1);
		}
		else if (event.jaxis.value < -m_joystickDeadZone)
		{
			m_joystickValues[joy].second->setY(-1);
		}
		else
		{
			m_joystickValues[joy].second->setY(0); // Stops game from detecting stick movement unless the stick is moved outside of the deadzone.
		}
	}
}

void InputHandler::onJoystickButtonDown(SDL_Event& event)
{
	// Get joystick id
	int joy = event.jbutton.which;

	m_buttonStates[joy][event.jbutton.button] = true;
}

void InputHandler::onJoystickButtonUp(SDL_Event& event)
{
	// Get joystick id
	int joy = event.jbutton.which;

	m_buttonStates[joy][event.jbutton.button] = false;
}

void InputHandler::onMouseButtonDown(SDL_Event& event)
{
	if (event.button.button == SDL_BUTTON_LEFT)
	{
		m_mouseButtonStates[LEFT] = true;
	}
	if (event.button.button == SDL_BUTTON_MIDDLE)
	{
		m_mouseButtonStates[MIDDLE] = true;
	}
	if (event.button.button == SDL_BUTTON_RIGHT)
	{
		m_mouseButtonStates[RIGHT] = true;
	}
}

void InputHandler::onMouseButtonUp(SDL_Event& event)
{
	if (event.button.button == SDL_BUTTON_LEFT)
	{
		m_mouseButtonStates[LEFT] = false;
	}
	if (event.button.button == SDL_BUTTON_MIDDLE)
	{
		m_mouseButtonStates[MIDDLE] = false;
	}
	if (event.button.button == SDL_BUTTON_RIGHT)
	{
		m_mouseButtonStates[RIGHT] = false;
	}
}

void InputHandler::onMouseMove(SDL_Event& event)
{
	m_mousePosition->setX(event.motion.x);
	m_mousePosition->setY(event.motion.y);
}

bool InputHandler::getButtonState(int joy, int buttonNumber)
{
	return m_buttonStates[joy][buttonNumber];
}

bool InputHandler::getMouseButtonState(int buttonNumber)
{
	return m_mouseButtonStates[buttonNumber];
}

Vector2D* InputHandler::getMousePosition()
{
	return m_mousePosition;
}

bool InputHandler::isKeyDown(SDL_Scancode key)
{
	if (m_keystates != 0)
	{
		if (m_keystates[key] == 1)
			return true;
	}
	return false;
}

// Get input events
void InputHandler::update()
{
	m_keystates = SDL_GetKeyboardState(0);

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		// New code
		switch (event.type) {
		case SDL_QUIT:
			TheGame::Instance()->quit();
			break;

		case SDL_JOYAXISMOTION:
			onJoystickAxisMove(event);
			break;

		case SDL_JOYBUTTONDOWN:
			onJoystickButtonDown(event);
			break;

		case SDL_JOYBUTTONUP:
			onJoystickButtonUp(event);
			break;

		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonDown(event);
			break;

		case SDL_MOUSEBUTTONUP:
			onMouseButtonUp(event);
			break;

		case SDL_MOUSEMOTION:
			onMouseMove(event);
			break;

		case SDL_KEYDOWN:
			onKeyDown();
			break;

		case SDL_KEYUP:
			onKeyUp();
			break;
		}
	}
}

int InputHandler::xvalue(int joy, int stick)
{
	// Game must have detected some joysticks in order to read these values.
	if (m_joystickValues.size() > 0)
	{
		// Left stick
		if (stick == 1)
		{
			return (int)m_joystickValues[joy].first->getX();
		}
		// Right stick
		else if (stick == 2)
		{
			return (int)m_joystickValues[joy].second->getX();
		}
	}
	return 0;
}

int InputHandler::yvalue(int joy, int stick)
{
	// Game must have detected some joysticks in order to read these values.
	if (m_joystickValues.size() > 0)
	{
		// Left stick
		if (stick == 1)
		{
			return (int)m_joystickValues[joy].first->getY();
		}
		// Right stick
		else if (stick == 2)
		{
			return (int)m_joystickValues[joy].second->getY();
		}
	}
	return 0;
}

void InputHandler::reset()
{
	m_mouseButtonStates[LEFT] = false;
	m_mouseButtonStates[MIDDLE] = false;
	m_mouseButtonStates[RIGHT] = false;
}