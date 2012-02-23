#ifndef RUNNERTILEMAP_H
#define RUNNERTILEMAP_H

#include <vector>
#include <map>
#include "math/vec2.h"
#include "math/bbox.h"

#include "tilemap.h"

class RunnerTilemap : public Tilemap
{
public:
	RunnerTilemap(float unitsPerTile);

	void Init(int seed, int wide);
	void update(float deltaTime);
	void Draw(const math::bbox2f &screen);

	void setColl(int x, int y, bool col); //set tile collisionable
	bool  isColl(int x, int y); //is collisionable

private:
	math::vec2i sizes;
	int seed;
	std::vector<bool> tiles;
	std::vector<int> rowsid;

	//MAP < CORD Y, MAP < CORD X, TIMELEFT > >;
	std::map<int, std::map<int, float> > deletedTiles;

	void addDeletedTile(int x, int y);
	void updateDeletedTiles(float deltaTime);
	void drawDeletedTiles(int y);

	void testRowY(int y);
	void set(int x, int y, bool col); //set tile collisionable
	bool get(int i, int j);
};

#endif // RUNNERTILEMAP_H
