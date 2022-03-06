#pragma once

#include <libtcod.h>
#include "Entity.h"

class Engine
{
public:
	bool quit;
	tcod::Console console;
	TCODList<Entity*> entities;
	Entity* player;

	Engine();
	~Engine();

	void update();
	void render();

private:
	tcod::ContextPtr context;
};

extern Engine engine;