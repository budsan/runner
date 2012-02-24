#include "runnertilemap.h"

#include <cstdlib>
#include <random>

#include "graphics/primitives.h"
#include "graphics/color.h"

#define RESTORE_TIME_DELETED_TILES 5.0f

RunnerTilemap::RunnerTilemap(float unitsPerTile) : Tilemap(unitsPerTile)
{
}

void RunnerTilemap::init(int seed, int wide)
{
	sizes = math::vec2i(wide, 40);
	this->seed = seed;

	tiles.clear();
	rowsid.clear();
	int total = sizes.x * sizes.y;
	tiles.assign(total, false);
	rowsid.assign(sizes.y, -1);
}

void RunnerTilemap::setColl(int x, int y, bool col)
{
	if ( y < 0 ) return;
	if ( x < 0  || x >= sizes.x ) return;

	int row = y%sizes.y;
	if (rowsid[row] == y ) set(x, row, col);
	addDeletedTile(x, y);
}

bool RunnerTilemap::isColl(int x, int y)
{
	if ( y == -2 && (x%32) == 0) return false;
	if ( y < 0 ) return true;

	return false;
}

void RunnerTilemap::update(float deltaTime)
{
	updateDeletedTiles(deltaTime);
}

void RunnerTilemap::draw(const math::bbox2f &screen)
{
	math::vec2i start = tilePos(screen.min);
	math::vec2i end   = tilePos(screen.max) + math::vec2i(1,1);

	for (int j = start.y; j < end.y; ++j)
	{
		for (int i = start.x; i < end.x; ++i)
		{
			math::bbox2f quad(
				math::vec2f(i,j)*unitsPerTile,
				math::vec2f(i+1,j+1)*unitsPerTile);

			if (isColl(i,j))
			{
					glColor(rgb(0, 0, 0));
					::draw(quad);
			}
		}
	}
}

void RunnerTilemap::testRowY(int y)
{
	int row = y % sizes.y;
	if (rowsid[row] != y)
	{
		rowsid[row] = y;
		for (int i = 0; i < sizes.x; i++) set(i, row, false);
		int ydiv = y/5, ymod = y%5;
		unsigned long yseed = (ydiv<<16)|(seed&0xFFFF);
		std::mt19937 random(yseed);
		if (ymod == (3+random()%2))
		{
			int x = 0;
			int xmax = sizes.x - 3;
			while (x < xmax)
			{
				x += (random()%16) + 3;
				int l = x + (random()%3) + 3;
				if ( l >= xmax ) break;
				while (x < l)
				{
					set(x, row, true);
					x++;
				}

				//REDESTROY TILES
				std::map<int, std::map<int, float> >::iterator line = deletedTiles.find(y);
				if (line != deletedTiles.end())
				{
					std::map<int, float>::iterator it = line->second.begin();
					for (;it != line->second.end(); it++)
					{
						set(it->first, row, false);
					}
				}
			}
		}
	}

}

void RunnerTilemap::addDeletedTile(int x, int y)
{
	deletedTiles[y][x] = RESTORE_TIME_DELETED_TILES;
}

void RunnerTilemap::updateDeletedTiles(float deltaTime)
{
	std::map<int, std::map<int, float> >::iterator it1 = deletedTiles.begin();
	while(it1 != deletedTiles.end())
	{
		std::map<int, float>::iterator it2 = it1->second.begin();
		while (it2 != it1->second.end())
		{
			it2->second -= deltaTime;
			if (it2->second < 0)
			{
				set(it2->first, it1->first%sizes.y, true);
				std::map<int, float>::iterator toDelete = it2++;
				it1->second.erase(toDelete);
			}
			else it2++;
		}

		if (it1->second.empty())
		{
			std::map<int, std::map<int, float> >::iterator toDelete = it1++;
			deletedTiles.erase(toDelete);
		}
		else it1++;
	}
}

void RunnerTilemap::drawDeletedTiles(int y)
{
	//DELETED TILES
	std::map<int, std::map<int, float> >::iterator line = deletedTiles.find(y);
	if (line != deletedTiles.end())
	{
		std::map<int, float>::iterator it = line->second.begin();
		for (;it != line->second.end(); it++)
		{
			if (it->second <= 1.0f)
			{
				int i = it->first;
				int j = y;
				math::bbox2f quad(
					math::vec2f(i,j)*unitsPerTile,
					math::vec2f(i+1,j+1)*unitsPerTile);

				glColor(rgba(0.400f, 0.275f, 0.195f, 1.0f - it->second));
				draw(quad);
			}
		}
	}
}

void RunnerTilemap::set(int i, int j, bool col)
{
	tiles[j*sizes.x + i] = col;
}

bool RunnerTilemap::get(int i, int j)
{
	return tiles[j*sizes.x + i];
}
