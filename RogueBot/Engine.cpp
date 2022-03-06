#include "Engine.h"
#include <SDL.h>

Engine::Engine()
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

void Engine::update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		context->convert_event_coordinates(event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		default: break;
		}
	}
}

void Engine::render()
{
	TCOD_console_clear(console.get());
	tcod::print(console, { 0, 0 }, "Hello World", TCOD_yellow, std::nullopt);
	context->present(console);
}
