#pragma once

#include <libtcod.h>

class Entity
{
public:
	int x, y;
	int chr;
	TCOD_ColorRGB color;

	Entity(int x, int y, int chr, TCOD_ColorRGB color);

	void render();
};