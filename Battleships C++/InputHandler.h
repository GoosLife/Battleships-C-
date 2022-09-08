#pragma once
#include "Vector2D.h"
#include <SDL.h>
#include <vector>

enum mouse_buttons {
	LEFT = 0,
	MIDDLE = 1,
	RIGHT = 2
};

class InputHandler
{
public:
	static InputHandler* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new InputHandler();
		}

		return s_pInstance;
	}

	void initializeJoysticks();
	bool joysticksInitialized();

	/*
		JOYSTICK STICK INPUT
	*/

	/// <summary>
	/// Get values from sticks on joystick
	/// </summary>
	/// <param name="joy">The controller to read values from.</param>
	/// <param name="stick">The stick to read values from. 1 = Left stick, 2 = Right stick</param>
	/// <returns></returns>
	int xvalue(int joy, int stick);
	/// <summary>
	/// Get values from sticks on joystick
	/// </summary>
	/// <param name="joy">The controller to read values from.</param>
	/// <param name="stick">The stick to read values from. 1 = Left stick, 2 = Right stick</param>
	/// <returns></returns>
	int yvalue(int joy, int stick);

	/*
		JOYSTICK BUTTON INPUT
	*/

	// Return the current state of a specific button on a specific controller.
	bool getButtonState(int joy, int buttonNumber);

	/*
		MOUSE BUTTON INPUT
	*/

	// Get the current up/down state of a specific mouse button.
	// Button number for left	= 0 or LEFT (enum)
	// Button number for middle	= 1 or MIDDLE (enum)
	// Right number for right	= 2 or RIGHT (enum)
	bool getMouseButtonState(int buttonNumber);

	/*
		MOUSE MOTION INPUT
	*/

	Vector2D* getMousePosition();

	/*
		KEYBOARD INPUT
	*/

	bool isKeyDown(SDL_Scancode key);

	/*
		GAME LOOP FUNCTIONS
	*/

	void update();
	void clean();

	// Reset all mouse button states to false
	void reset();

private:
	std::vector<SDL_Joystick*> m_joysticks;
	bool m_bJoysticksInitialized;

	/*
		JOYSTICK STICK INPUT 
	*/

	// Joystick input values
	std::vector<std::pair<Vector2D*, Vector2D*>> m_joystickValues;

	// Joystick deadzone. TODO: Make this a variable that can be changed by the user.
	int m_joystickDeadZone{ 10000 };

	void onJoystickAxisMove(SDL_Event& event);

	/*
		JOYSTICK BUTTON INPUT
	*/

	/// <summary>
	// The first index into the array represents each controller connected to the system.
	// The second index represents each button associated with that controller.
	// Button state will be TRUE when button is pressed DOWN,
	// and FALSE when button is UP.
	/// </summary>
	std::vector<std::vector<bool>> m_buttonStates;

	void onJoystickButtonDown(SDL_Event& event);
	void onJoystickButtonUp(SDL_Event& event);

	/*
		MOUSE BUTTON INPUT
	*/

	// Array of mouse buttons and their current state.
	// True  = Button is DOWN.
	// False = Button is UP.
	std::vector<bool> m_mouseButtonStates;
	
	void onMouseButtonDown(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);

	/*
		MOUSE MOTION INPUT
	*/

	Vector2D* m_mousePosition;
	void onMouseMove(SDL_Event& event);

	/*
		KEYBOARD INPUT
	*/

	// Array containing the current state of every key on the keyboard.
	const Uint8* m_keystates;

	void onKeyDown();
	void onKeyUp();

	/*
		CONSTRUCTORS
	*/
	InputHandler();
	~InputHandler();

	static InputHandler* s_pInstance;
};

typedef InputHandler TheInputHandler;