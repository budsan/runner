#ifndef RUNNERTILEMAP_H
#define RUNNERTILEMAP_H

#include <vector>
#include <random>

#include "math/vec2.h"
#include "math/bbox.h"

#include "tilemap.h"

class RunnerTilemap : public Tilemap
{
public:
	RunnerTilemap(float unitsPerTile);

    void init(int seed);
	void update(float deltaTime);
	void draw(const math::bbox2f &screen);

	void setColl(int x, int y, bool col); //set tile collisionable
	bool  isColl(int x, int y); //is collisionable

private:
    int m_seed;
    std::vector<int> m_heights;
    std::mt19937 m_random;

    void generateUntil(int x);
};

#endif // RUNNERTILEMAP_H
