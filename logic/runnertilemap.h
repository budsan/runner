#pragma once

#include <vector>
#include <random>

#include "math/algebra3.h"
#include "math/bbox.h"
#include "graphics/color.h"

#include "tilemap.h"

class RunnerTilemap : public Tilemap
{
public:
	RunnerTilemap(double m_unitsPerTile, int maxHeight, bool ceils);
	virtual ~RunnerTilemap() {}

    void init(int seed);
	void update(double deltaTime);
	void draw(const math::bbox2f &screen);

	void setColl(int x, int y, bool col); //set tile collisionable
	bool  isColl(int x, int y); //is collisionable

	void setColor(const Guy::rgba &color) {m_color = color;}

private:
    int m_seed;
    int m_maxHeight;
    bool m_ceils;
    std::default_random_engine m_random;

    struct chunk {
	    chunk() : height(0), ceil(0) {}
	    chunk(unsigned int _height, unsigned short _ceil)
		    : height(_height), ceil(_ceil) {}
	    chunk(const chunk& c)
		    : height(c.height), ceil(c.ceil) {}

	    int height;
	    short ceil;
    };

    std::vector<chunk> m_chunks;
    Guy::rgba m_color;

    void generateUntil(int x);
};
