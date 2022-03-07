#include "Engine.h"

Engine engine;

int main(int argc, char* argv[])
{
	engine.load();
	while (!engine.quit)
	{
		engine.update();
		engine.render();
	}
	return 0;
}
