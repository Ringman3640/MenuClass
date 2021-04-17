#include "Menu.h"
#include <windows.h>

int main() {
	int savedSpeed = 5, savedHeight = 10;
	Menu startScreen("MY TEST");
	Menu options("MY TEST > Options");
	Modifier speed(&savedSpeed, "MY TEST > Options > Change Speed");
	Modifier height(&savedHeight, "MY TEST > Options > Change Height");
	Text credits("MY TEST > Credits");
	Custom customTest("Custom Test", []() {
		std::cout << "Entered Test, press Enter to return . . . \n";
		std::cin.ignore(1000, '\n');
		std::cin.clear();
	});

	startScreen.setUp('W');
	startScreen.setDown('S');

	startScreen.addExit("Start");
	startScreen.addStaticEntry("Options", options);
	startScreen.addVariableEntry("Current Speed: ", savedSpeed, speed);
	startScreen.addStaticEntry("Credits", credits);
	startScreen.addStaticEntry("Custom Screen Test", customTest);

	startScreen.exitAction([]() {
		std::cout << "Testing Exit action for 5 seconds . . .";
		Sleep(5000);
		});

	options.addStaticEntry("Op 1");
	options.addStaticEntry("Op 2");
	options.addStaticEntry("Op 3");
	options.addStaticEntry("Op 4");
	options.addVariableEntry("Current Speed: ", savedSpeed, speed);
	options.addVariableEntry("Current Height: ", savedHeight, height);
	options.addExit("Return");

	credits.setText("This is a test of a credits screen. Blah blah blah blah..... \n");

	startScreen.start();
}