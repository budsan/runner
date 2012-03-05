#include "runnertilemap.h"

#include <list>
#include <cstdlib>
#include <random>

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
	m_random = std::mt19937(seed);
	m_chunks.resize(40);
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
	if (m_chunks.size() <= x) generateUntil(x);

	chunk& cur = m_chunks[x];
	if (cur.height >= y) return true;
	else {
		if (cur.ceil > 4 && (cur.ceil + cur.height) < y )
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

	std::list<math::vec2i> colls;
	for (int j = start.y; j < end.y; ++j)
	{
		for (int i = start.x; i < end.x; ++i)
		{
			if (isColl(i,j)) colls.push_back(math::vec2i(i,j));
		}
	}

	std::vector<math::vec2f> vertcoords;
	std::vector<unsigned int> indices;

	vertcoords.reserve(colls.size()*4);
	indices.reserve(colls.size()*6);

	std::list<math::vec2i>::iterator it = colls.begin();
	for(;it != colls.end(); it++)
	{
		int i = it->x, j = it->y;
		math::bbox2f quad(
			math::vec2f(i,j)    *unitsPerTile,
			math::vec2f(i+1,j+1)*unitsPerTile);

		::draw(quad, vertcoords, indices);
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_BLEND);

	glEnableClientState(GL_VERTEX_ARRAY);
	glColor(m_color);
	glVertexPointer  (2, GL_FLOAT, 0, &vertcoords[0]);
	glDrawElements(GL_TRIANGLES, colls.size()*6, GL_UNSIGNED_INT, &indices[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
