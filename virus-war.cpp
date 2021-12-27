#include <iostream>
#include "GameController.h"

int main()
{
	GameController* gc = new GameController();
	gc->run();

	std::cin.get();

	return 0;
}
