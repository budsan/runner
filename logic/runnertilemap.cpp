#include "runnertilemap.h"

#include <cstdlib>
#include <stdlib.h>
#include "boost/random.hpp"

#include "graphics/primitives.h"
#include "graphics/color.h"


#define RESTORE_TIME_DELETED_TILES 5.0f

RunnerTilemap::RunnerTilemap(float unitsPerTile, int maxHeight)
	: Tilemap(unitsPerTile), m_maxHeight(maxHeight), m_color(0)
{

}

void RunnerTilemap::init(int seed)
{
	this->m_seed = seed;
	m_random = boost::mt19937(seed);
    m_chunks.resize(60);
}

void RunnerTilemap::setColl(int x, int y, bool col)
{
	return; //DO NOTHING
}

bool RunnerTilemap::isColl(int x, int y)
{
	if ( y < 0 ) return true;
	if (m_seed == 0) return false;

	x = x < 0 ?  0 : x;
	if ((int)m_chunks.size() <= x) generateUntil(x);

	chunk& cur = m_chunks[x];
	if ((int)cur.height > y) return true;
	else {
		if ((int)cur.ceil > 4 && (int)(cur.ceil + cur.height) < y )
			return true;
	}

	return false;
}

void RunnerTilemap::update(float deltaTime)
{

}

void RunnerTilemap::draw(const math::bbox2f &screen)
{
	math::vec2i start = tilePos(screen.min);
	math::vec2i end   = tilePos(screen.max) + math::vec2i(1,1);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_BLEND);

	#define VERTEX_ARRAY_SIZE 512
	static float vertcoords[VERTEX_ARRAY_SIZE * 8];
	static unsigned short indices[VERTEX_ARRAY_SIZE * 6];
	unsigned short c = 0;

	glColor(m_color);
	for (int j = start.y; j < end.y; ++j)
	{
		for (int i = start.x; i < end.x; ++i)
		{
			if (isColl(i,j))
			{
				math::bbox2f quad(
					math::vec2f(i+0,j+0)*unitsPerTile,
					math::vec2f(i+1,j+1)*unitsPerTile);

				vertcoords[c*8 + 0] = quad.min.x;
				vertcoords[c*8 + 1] = quad.min.y;
				vertcoords[c*8 + 2] = quad.max.x;
				vertcoords[c*8 + 3] = quad.min.y;
				vertcoords[c*8 + 4] = quad.max.x;
				vertcoords[c*8 + 5] = quad.max.y;
				vertcoords[c*8 + 6] = quad.min.x;
				vertcoords[c*8 + 7] = quad.max.y;

				indices[c*6 + 0] = c*4 + 3;
				indices[c*6 + 1] = c*4 + 0;
				indices[c*6 + 2] = c*4 + 1;
				indices[c*6 + 3] = c*4 + 1;
				indices[c*6 + 4] = c*4 + 2;
				indices[c*6 + 5] = c*4 + 3;

				if (++c == VERTEX_ARRAY_SIZE)
				{
					glGetError();

					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer  (2, GL_FLOAT, 0, vertcoords);
					glDrawElements(GL_TRIANGLES, c*6, GL_UNSIGNED_SHORT, indices);
					glDisableClientState(GL_VERTEX_ARRAY);

					GLint err = glGetError();
					if (err != 0) std::cerr << "GL error " << err << std::endl;

					c = 0;
				}
			}
		}
	}

	if (c)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer  (2, GL_FLOAT, 0, vertcoords);
		glDrawElements(GL_TRIANGLES, c*6, GL_UNSIGNED_SHORT, indices);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void RunnerTilemap::generateUntil(int x)
{
	int i = m_chunks.size();
	while (i <= x)
	{
		chunk& last = m_chunks.back();
		bool ceiled = last.ceil >= 4;
		chunk  curr(last);

		while (curr.height == last.height)
		{
			short slope = (2 + m_random()%4) * (m_random()&1 ? 1 : -1);
			if (ceiled || slope < 0)
			{
				curr.ceil = 0;
				if (ceiled) slope = -1 * (short)abs(slope);
			}
			else
			{
				if (m_random()%4 == 0) curr.ceil = 4 + m_random()%4;
				else curr.ceil = 0;
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
