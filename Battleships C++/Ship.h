#pragma once
#include "Square.h"

enum ShipType
{
	eShipType_PatrolBoat = 2,
	eShipType_Destroyer = 3,
	eShipType_Slagskib = 4,
	eShipType_HangarShip = 5
};

class Ship
{
public:
	Ship(ShipType t);

	ShipType getType() const { return m_type; }
	int getSize() const { return m_size; }
	int getXPos() const { return m_xPos; }
	int getYPos() const { return m_yPos; }
	bool getIsHorizontal() const { return m_isHorizontal; }
	bool getIsVertical() const { return m_isVertical; }
	
	void update();
	void render();
	void handleInput();

private:
	Square* m_squares;

	ShipType m_type;
	int m_size;

	int m_xPos;
	int m_yPos;

	bool m_isHorizontal;
	bool m_isVertical;

	void updateSquares();
	void changeDirection();
};

