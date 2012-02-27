#ifndef RUNNERTILEMAP_H
#define RUNNERTILEMAP_H

#include <vector>
#include <random>

#include "math/vec2.h"
#include "math/bbox.h"
#include "graphics/color.h"

#include "tilemap.h"

class RunnerTilemap : public Tilemap
{
public:
	RunnerTilemap(float unitsPerTile, int maxHeight);

    void init(int seed);
	void update(float deltaTime);
	void draw(const math::bbox2f &screen);

	void setColl(int x, int y, bool col); //set tile collisionable
	bool  isColl(int x, int y); //is collisionable

	void setColor(const rgba &color) {m_color = color;}

private:
    int m_seed;
    int m_maxHeight;
    std::mt19937 m_random;

    struct chunk {
	    chunk() : height(0), ceil(0) {}
	    chunk(unsigned int height, unsigned short ceil)
		    : height(height), ceil(ceil) {}
	    chunk(const chunk& c)
		    : height(c.height), ceil(c.ceil) {}

	    unsigned int height;
	    unsigned short ceil;
    };

    std::vector<chunk> m_chunks;
    rgba m_color;

    void generateUntil(int x);
};

#endif // RUNNERTILEMAP_H
