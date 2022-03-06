#include "Entity.h"
#include "Engine.h"

Entity::Entity(int x, int y, int chr, TCOD_ColorRGB color):x(x),y(y),chr(chr),color(color)
{
}

void Entity::render()
{
	TCOD_console_set_char(engine.console.get(), x, y, chr);
	TCOD_console_set_char_foreground(engine.console.get(), x, y, color);
}
