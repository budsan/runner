#pragma once

#include <vector>
#include "guyframework/math/algebra3.h"
#include "guyframework/math/bbox.h"

class World;
class Tilemap
{
public:
	Tilemap(double unitsPerTile);
	virtual ~Tilemap() {}

	virtual void setColl(int x, int y, bool col) = 0; //set tile collisionable
	virtual bool  isColl(int x, int y) = 0; //is collisionable

	//what tile is in world coordinate
	math::vec2i tilePos(math::vec2d pos);
	math::vec2i tilePos(double x, double y);
	int tilePosX(double x);
	int tilePosY(double y);

	double Top(int y);
	double Bottom(int y);

	double Left(int x);
	double Right(int x);

protected:
	double m_unitsPerTile;
};
