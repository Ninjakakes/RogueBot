#include "Engine.h"
#include <SDL.h>

Engine::Engine() :fovRadius(10)
{
	console = tcod::Console{ 80,50 };

	auto params = TCOD_ContextParams{};
	params.tcod_version = TCOD_COMPILEDVERSION;
	params.console = console.get();
	params.window_title = "RogueBot";
	params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
	params.vsync = true;
	auto tileset = tcod::load_tilesheet("LN_EGA8x8.png", { 16,16 }, tcod::CHARMAP_CP437);
	params.tileset = tileset.get();

	context = tcod::new_context(params);
}

Engine::~Engine()
{
	entities.clearAndDelete();
}

void Engine::init()
{
	player = new Entity(0, 0, '@', TCOD_grey);
	entities.push(player);
	map = new Map(80, 45);
	map->init(true);
	map->computeFov();
}

void Engine::load()
{
	init();
}

void Engine::save()
{
}

void Engine::update()
{
	int dx = 0, dy = 0;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		context->convert_event_coordinates(event);
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_UP:
				dy = -1;
				break;
			case SDL_SCANCODE_DOWN:
				dy = 1;
				break;
			case SDL_SCANCODE_LEFT:
				dx = -1;
				break;
			case SDL_SCANCODE_RIGHT:
				dx = 1;
				break;
			default:
				break;
			}
			break;
		case SDL_QUIT:
			quit = true;
			break;
		default: break;
		}
		if (dx != 0 || dy != 0)
		{
			if (map->canWalk(player->x + dx, player->y + dy))
			{
				player->x += dx;
				player->y += dy;
				map->computeFov();
			}
		}
	}
}

void Engine::render()
{
	TCOD_console_clear(console.get());
	map->render();
	for (Entity* entity : entities)
	{
		if (map->isInFov(entity->x, entity->y))
		{
			entity->render();
		}
	}
	player->render();
	context->present(console);
}
