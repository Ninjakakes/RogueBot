#pragma once
#include <libtcod.h>

class Engine
{
public:
	bool quit;
	tcod::Console console;

	Engine();

	void update();
	void render();

private:
	tcod::ContextPtr context;
};