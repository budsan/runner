#include "runnertilemap.h"

#include <cstdlib>
#include <stdlib.h>
#include <random>

#include "graphics/primitives.h"
#include "graphics/color.h"

#define VERTEX_ARRAY_SIZE 512
static float vertcoords[VERTEX_ARRAY_SIZE * 8];
static unsigned short indices[VERTEX_ARRAY_SIZE * 6];

inline void fillVertexArray(unsigned short offset, const math::bbox2f &quad)
{
	vertcoords[offset*8 + 0] = quad.min.x;
	vertcoords[offset*8 + 1] = quad.min.y;
	vertcoords[offset*8 + 2] = quad.max.x;
	vertcoords[offset*8 + 3] = quad.min.y;
	vertcoords[offset*8 + 4] = quad.max.x;
	vertcoords[offset*8 + 5] = quad.max.y;
	vertcoords[offset*8 + 6] = quad.min.x;
	vertcoords[offset*8 + 7] = quad.max.y;

	indices[offset*6 + 0] = (unsigned short) (offset*4 + 3);
	indices[offset*6 + 1] = (unsigned short) (offset*4 + 0);
	indices[offset*6 + 2] = (unsigned short) (offset*4 + 1);
	indices[offset*6 + 3] = (unsigned short) (offset*4 + 1);
	indices[offset*6 + 4] = (unsigned short) (offset*4 + 2);
	indices[offset*6 + 5] = (unsigned short) (offset*4 + 3);
}

inline void drawVertexArray(unsigned short count)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer  (2, GL_FLOAT, 0, vertcoords);
	glDrawElements(GL_TRIANGLES, count*6, GL_UNSIGNED_SHORT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
}



#define RESTORE_TIME_DELETED_TILES 5.0

RunnerTilemap::RunnerTilemap(double unitsPerTile, int maxHeight, bool ceils)
	: Tilemap(unitsPerTile), m_maxHeight(maxHeight), m_color(0), m_ceils(ceils)
{

}

void RunnerTilemap::init(int seed)
{
	this->m_seed = seed;
	m_random = std::default_random_engine(seed);
	m_chunks.resize(60);
}

void RunnerTilemap::setColl(int x, int y, bool col)
{
	return; //DO NOTHING
}

bool RunnerTilemap::isColl(int x, int y)
{
	x = ((x >= 0) * -1) & x;
	chunk& cur = m_chunks[x];
	return (y < 0) || (cur.height > y) || ((int)cur.ceil > 4 && (int)(cur.ceil + cur.height) < y );
}

void RunnerTilemap::update(double deltaTime)
{

}

void RunnerTilemap::draw(const math::bbox2f &screen)
{
	math::vec2i start = tilePos(screen.min);
	math::vec2i end   = tilePos(screen.max) + math::vec2i(1,1);

	if ((int)m_chunks.size() <= end.x) generateUntil(end.x);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_BLEND);

	unsigned short c = 0;
	glColor(m_color);
	if (start.y < 0)
	{
		math::bbox2f quad(
			math::vec2f((float)start.x,(float) start.y)*m_unitsPerTile,
			math::vec2f((float)1+end.x,(float)       0)*m_unitsPerTile);

		fillVertexArray(0, quad);
		drawVertexArray(1);
		start.y = 0;
	}
	if (start.x < 0) start.x = 0;

	for (int i = start.x; i < end.x; ++i)
	{
		chunk& cur = m_chunks[i];
		bool lastColl = false;
		int  lastColly = start.y;

		for (int j = start.y; j < end.y; ++j)
		{
			bool currColl = (j < 0) || (cur.height > j) || ((int)cur.ceil > 4 && (int)(cur.ceil + cur.height) < j);

			if (!lastColl && currColl) lastColly = j;
			else if (lastColl && !currColl)
			{
				math::bbox2f quad(
					math::vec2f((float) i+0,(float) lastColly)*m_unitsPerTile,
					math::vec2f((float) i+1,(float)         j)*m_unitsPerTile);

				fillVertexArray(c, quad);
				if (++c == VERTEX_ARRAY_SIZE) {
					drawVertexArray(c);
					c = 0;
				}
			}

			lastColl = currColl;
		}

		if (lastColl)
		{
			math::bbox2f quad(
				math::vec2f((float) i+0,(float) lastColly)*m_unitsPerTile,
				math::vec2f((float) i+1,(float)     end.y)*m_unitsPerTile);

			fillVertexArray(c, quad);
			if (++c == VERTEX_ARRAY_SIZE) {
				drawVertexArray(c);
				c = 0;
			}
		}
	}

	if (c) drawVertexArray(c);
}

void RunnerTilemap::generateUntil(int x)
{
	int i = m_chunks.size();
	while (i <= x)
	{
		chunk& last = m_chunks.back();
		bool ceiled = last.ceil >= 4;
		chunk  curr(last);

		if (m_seed == 0)
		{
			m_chunks.push_back(curr);
			i++;
			continue;
		}

		while (curr.height == last.height)
		{
			short slope = (2 + m_random() % 4) * (m_random() & 1 ? 1 : -1);
			if (m_ceils)
			{
				if (ceiled || slope < 0)
				{
					curr.ceil = 0;
					if (ceiled) slope = -1 * abs(slope);
				}
				else
				{
					if (m_random() % 4 == 0) curr.ceil = 4 + m_random() % 4;
					else curr.ceil = 0;
				}
			}
			else
			{
				curr.ceil = 0;
			}

			curr.height = last.height + slope;
			if (curr.height <           0)
			{
				curr.height = last.height;
				continue;
			}
			if (curr.height > m_maxHeight)
			{
				curr.height = last.height;
				continue;
			}
		}

		int wide  =  4 + m_random()%4;
		while(wide--)
		{
			m_chunks.push_back(curr);
			i++;
		}
	}
}
