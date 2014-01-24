#include "tilemap.h"
#include <math.h>

Tilemap::Tilemap(double unitsPerTile) : m_unitsPerTile(unitsPerTile)
{
}

math::vec2i Tilemap::tilePos(math::vec2d pos)
{
	return tilePos(pos.x, pos.y);
}

math::vec2i Tilemap::tilePos(double x, double y)
{
	return math::vec2i(
		(int) floor(x/m_unitsPerTile),
		(int) floor(y/m_unitsPerTile));
}

int Tilemap::tilePosX(double x)
{
	return (int) floor(x/m_unitsPerTile);
}

int Tilemap::tilePosY(double y)
{
	return (int) floor(y/m_unitsPerTile);
}

double Tilemap::Top(int y)
{
	return static_cast<double>(y+1)*m_unitsPerTile;
}

double Tilemap::Bottom(int y)
{
	return static_cast<double>(y)*m_unitsPerTile;
}

double Tilemap::Left(int x)
{
	return static_cast<double>(x)*m_unitsPerTile;
}

double Tilemap::Right(int x)
{
	return static_cast<double>(x+1)*m_unitsPerTile;
}

