#pragma once

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

	int getType() const { return m_size; }
private:
	ShipType m_type;
	int m_size;
};

