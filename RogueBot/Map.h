#pragma once
#include <libtcod.h>

struct Tile
{
	bool explored;

	Tile() :explored(false) {}
};

class Map
{
public:
	int width, height;

	Map(int width, int height);
	~Map();
	void init(bool withEntites);
	bool isWall(int x, int y) const;
	bool isInFov(int x, int y) const;
	bool isExplored(int x, int y) const;
	bool canWalk(int x, int y) const;
	void computeFov();
	void render() const;

protected:
	Tile* tiles;
	TCODMap* map;
	long seed;
	TCODRandom* rng;
	friend class BspListener;

	void dig(int x1, int y1, int x2, int y2);
	void createRoom(bool first, int x1, int y1, int x2, int y2, bool withEntities);
	void addMonster(int x, int y);
};