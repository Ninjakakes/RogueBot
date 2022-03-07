#include "Map.h"
#include "Engine.h"

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;
static const int MAX_ROOM_MONSTERS = 3;

class BspListener : public ITCODBspCallback
{
private:
	Map& map; // a map to dig
	int roomNum; // room number
	int lastX=0, lastY=0; // center of the last room
public:
	BspListener(Map& map) : map(map), roomNum(0) {}
	bool visitNode(TCODBsp* node, void* userData)
	{
		bool withEntites = (bool)userData;
		if (node->isLeaf())
		{
			int x, y, w, h;
			// dig a room
			w = map.rng->getInt(ROOM_MIN_SIZE, node->w - 2);
			h = map.rng->getInt(ROOM_MIN_SIZE, node->h - 2);
			x = map.rng->getInt(node->x + 1, node->x + node->w - w - 1);
			y = map.rng->getInt(node->y + 1, node->y + node->h - h - 1);
			map.createRoom(roomNum == 0, x, y, x + w - 1, y + h - 1, withEntites);
			if (roomNum != 0)
			{
				// dig a corridor from last room
				map.dig(lastX, lastY, x + w / 2, lastY);
				map.dig(x + w / 2, lastY, x + w / 2, y + h / 2);
			}
			lastX = x + w / 2;
			lastY = y + h / 2;
			roomNum++;
		}
		return true;
	}
};

Map::Map(int width, int height):width(width),height(height)
{
	seed = TCODRandom::getInstance()->getInt(0, LONG_MAX);
	printf("%d", seed);
}

Map::~Map()
{
	delete rng;
	delete[] tiles;
	delete map;
}

void Map::init(bool withEntites)
{
	rng = new TCODRandom(seed);
	tiles = new Tile[width * height];
	map = new TCODMap(width, height);
	TCODBsp bsp(0, 0, width, height);
	bsp.splitRecursive(NULL, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
	BspListener listener(*this);
	bsp.traverseInvertedLevelOrder(&listener, (void*)withEntites);
}

bool Map::isWall(int x, int y) const
{
	return !map->isWalkable(x, y);
}

bool Map::isInFov(int x, int y) const
{
	if (map->isInFov(x, y))
	{
		tiles[x + y * width].explored = true;
		return true;
	}
	return false;
}

bool Map::isExplored(int x, int y) const
{
	return tiles[x + y * width].explored;
}

bool Map::canWalk(int x, int y) const
{
	if (isWall(x, y))
	{
		return false;
	}
	return true;
}

void Map::computeFov()
{
	map->computeFov(engine.player->x, engine.player->y, engine.fovRadius);
}

void Map::render() const
{
	static const tcod::ColorRGB darkWall(0, 0, 100);
	static const tcod::ColorRGB darkGround(50, 50, 150);
	static const tcod::ColorRGB lightWall(130, 110, 50);
	static const tcod::ColorRGB lightGround(200, 180, 50);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (isInFov(x, y))
			{
				TCOD_console_set_char_background(engine.console.get(), x, y, isWall(x, y) ? lightWall : lightGround, TCOD_BKGND_SET);
			}
			else if (isExplored(x, y))
			{
				TCOD_console_set_char_background(engine.console.get(), x, y, isWall(x, y) ? darkWall : darkGround, TCOD_BKGND_SET);
			}

		}
	}
}

void Map::dig(int x1, int y1, int x2, int y2)
{
	if (x2 < x1)
	{
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
	}

	if (y2 < y1)
	{
		int tmp = y2;
		y2 = y1;
		y1 = tmp;
	}

	for (int tileX = x1; tileX <= x2; tileX++)
	{
		for (int tileY = y1; tileY <= y2; tileY++)
		{
			map->setProperties(tileX, tileY, true, true);
		}
	}
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2, bool withEntities)
{
	dig(x1, y1, x2, y2);
	if (!withEntities) return;
	if (first)
	{
		// put the player in the first room
		engine.player->x = (x1 + x2) / 2;
		engine.player->y = (y1 + y2) / 2;
	}
	else
	{
		int nbMonsters = rng->getInt(0, MAX_ROOM_MONSTERS);
		while (nbMonsters > 0)
		{
			int x = rng->getInt(x1, x2);
			int y = rng->getInt(y1, y2);
			if (canWalk(x, y))
			{
				addMonster(x, y);
			}
			nbMonsters--;
		}
	}
}

void Map::addMonster(int x, int y)
{
	engine.entities.push(new Entity(x, y, '@', TCOD_light_yellow));
}
