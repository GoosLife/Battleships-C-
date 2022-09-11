#include "Grid.h"
#include "Ship.h"

Grid::Grid() {}

Grid::Grid(int xPos, int yPos, int tileSize, int gridRows, int gridCols, bool hideOccupiedSquares) : m_xPos(xPos), m_yPos(yPos), m_tileSize(tileSize), m_height(gridCols), m_width(gridRows), m_hideOccupiedSquares(hideOccupiedSquares)
{
	m_squares = new Square* [gridRows];
	
	for (int i = 0; i < gridRows; i++)
	{
		m_squares[i] = new Square[gridCols];
	}

	for (int i = 0; i < gridRows; i++)
	{
		for (int j = 0; j < gridCols; j++) 
		{
			m_squares[i][j] = Square((i + xPos) * tileSize, (j + yPos) * tileSize, tileSize);
		}
	}
}

void Grid::update()
{
	// Resets hovered squares, so that squares that are no longer hovered over become empty again.
	resetSquares();
}

void Grid::render()
{
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			SDL_Color color = { 255,255,255 };

			/*if (filledSquares[i][j] == 1)
			{
				color.r = 90;
				color.g = 90;
				color.b = 90;
			}*/

			switch (m_squares[i][j].getState())
			{
			case (State::eState_Empty):
				color.r = 51;
				color.g = 95;
				color.b = 196;
				break;
			case (State::eState_Hover):
				color.r = 166;
				color.g = 166;
				color.b = 166;
				break;
			case (State::eState_Occupied):
				if (!m_hideOccupiedSquares)
				{
					color.r = 74;
					color.g = 74;
					color.b = 74;
				}
				else
				{
					color.r = 51;
					color.g = 95;
					color.b = 196;
				}
				break;
			case (State::eState_Hit):
				color.r = 255;
				color.g = 0;
				color.b = 0;
				break;
			case (State::eState_Miss):
				color.r = 255;
				color.g = 255;
				color.b = 255;
				break;

			}

			SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), color.r, color.g, color.b, 0);
			SDL_RenderFillRect(TheGame::Instance()->getRenderer(), &m_squares[i][j].getRect());

			SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 0, 0, 0, 255);
			SDL_RenderDrawRect(TheGame::Instance()->getRenderer(), &m_squares[i][j].getRect());
		}
	}
}

bool Grid::placeShip(Ship ship)
{
	if (validatePosition(ship))
	{
		for (int i = 0; i < ship.getSize(); i++)
		{
			m_squares[(i * ship.getIsHorizontal()) + ship.getXPos()][(i * ship.getIsVertical()) + ship.getYPos()].setState(State::eState_Occupied);
		}
		return true;
	}

	return false;
}

void Grid::resetSquares()
{
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			// Squares that are empty or were hovered above before the mouse was moved away
			// are reset to empty squares
			if (m_squares[i][j].getState() == State::eState_Empty ||
				m_squares[i][j].getState() == State::eState_Hover)
			{
				m_squares[i][j].setState(State::eState_Empty);
			}
		}
	}
}

bool Grid::validatePosition(Ship ship)
{
	if ((ship.getIsHorizontal() &&
		ship.getXPos() + ship.getSize() <= m_width &&
		ship.getYPos() <= m_height) ||
		(ship.getIsVertical() &&
			ship.getXPos() < m_width &&
			ship.getYPos() + ship.getSize() <= m_height))
	{
		// If any of the squares we want to check are occupied, we can't place the ship here.
		if (!validateSquaresAreEmpty(ship))
			return false;
	}
	// The mouse was outside of the grid, or too close to the edge to fit the whole ship. The ship can't be placed.
	else
	{
		return false;
	}

	// We didn't hit any of the checks making the ships position invalid, return true
	return true;
}

bool Grid::validateSquaresAreEmpty(Ship ship)
{
	// Check all squares to make sure that none of the squares for the ship are occupied
	for (int i = 0; i < ship.getSize(); i++)
	{
		if (!(m_squares[(ship.getXPos())+(i * ship.getIsHorizontal())][(ship.getYPos())+(i * ship.getIsVertical())].getState() == State::eState_Empty ||
			m_squares[(ship.getXPos())+(i * ship.getIsHorizontal())][(ship.getYPos())+(i * ship.getIsVertical())].getState() == State::eState_Hover))
		{
			// One of the squares was occupied - now we can't place the ship
			return false;
		}
	}
	return true;
}

/// <summary>
/// Attempt to shoot at a coordinate on the grid.
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns>Returns true if the target square was occupied, otherwise returns false.</returns>
bool Grid::shootSquare(int x, int y)
{
	switch (m_squares[x][y].getState())
	{
	case State::eState_Occupied:
		m_squares[x][y].setState(State::eState_Hit);
		return true;

	case State::eState_Empty:
		m_squares[x][y].setState(State::eState_Miss);
	default:
		return false;
	}
}

/// <summary>
/// Get the current state of a specific square in the grid.
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
State Grid::getSquareState(int x, int y)
{
	return m_squares[x][y].getState();
}