#pragma once

#include <libtcod.h>
#include "Entity.h"
#include "Map.h"

class Engine
{
public:
	bool quit;
	tcod::Console console;
	TCODList<Entity*> entities;
	Entity* player;
	Map* map;
	int fovRadius;

	Engine();
	~Engine();

	void init();
	void load();
	void save();
	void update();
	void render();

private:
	tcod::ContextPtr context;
};

extern Engine engine;