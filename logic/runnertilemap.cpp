#include "runnertilemap.h"

#include <cstdlib>
#include <random>

#include "graphics/primitives.h"
#include "graphics/color.h"

#define RESTORE_TIME_DELETED_TILES 5.0f

RunnerTilemap::RunnerTilemap(float unitsPerTile) : Tilemap(unitsPerTile)
{
}

void RunnerTilemap::init(int seed)
{
    this->m_seed = seed;
    m_random = std::mt19937(seed);
    m_heights.assign(40, 0);
}

void RunnerTilemap::setColl(int x, int y, bool col)
{
    return; //DO NOTHING
}

bool RunnerTilemap::isColl(int x, int y)
{
	if ( y == -2 && (x%32) == 0) return false;
	if ( y < 0 ) return true;

    x = x < 0 ?  0 : x;
    if (m_heights.size() <= x) generateUntil(x);
    if (m_heights[x] >= y ) return true;

	return false;
}

void RunnerTilemap::update(float deltaTime)
{

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

void RunnerTilemap::generateUntil(int x)
{
    int i = m_heights.size();
    while (i <= x)
    {
        int lastHeight = m_heights.back();
        int newHeight = lastHeight;
        int slope = 0, wide = 0;

        while (newHeight == lastHeight)
        {
            slope = (4 + m_random()%4) * (m_random()&1 ? 1 : -1);
            wide  =  4 + m_random()%4;

            newHeight = lastHeight - slope;
            newHeight = newHeight < 0 ? 0 : newHeight;
            newHeight = newHeight > 13 ? 13 : newHeight;
        }

        while(wide--)
        {
            m_heights.push_back(newHeight);
            i++;
        }
    }
}
