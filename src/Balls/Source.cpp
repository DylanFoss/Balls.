#include "Balls.h"
int main(int argc, char* argv[])
{

	std::unique_ptr<Balls> app = std::make_unique<Balls>("Balls.", 800, 800);
	app->Run();

	return 0;
}