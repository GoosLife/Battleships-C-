#include "Grid.h"

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
				color.r = 0;
				color.g = 0;
				color.b = 0;
				break;

			}

			SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), color.r, color.g, color.b, 0);
			SDL_RenderFillRect(TheGame::Instance()->getRenderer(), &m_squares[i][j].getRect());

			SDL_SetRenderDrawColor(TheGame::Instance()->getRenderer(), 0, 0, 0, 255);
			SDL_RenderDrawRect(TheGame::Instance()->getRenderer(), &m_squares[i][j].getRect());
		}
	}
}