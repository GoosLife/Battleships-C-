#include "Square.h"

Square::Square() {}

Square::Square(int x, int y, int size)
{
	m_xPos = x;
	m_yPos = y;

	squareRect.x = m_xPos;
	squareRect.y = m_yPos;
	squareRect.w = squareRect.h = size;

	object = nullptr;

	m_state = eState_Empty;
}