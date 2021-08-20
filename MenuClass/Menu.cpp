#include "Menu.h"

//Static Initialization
char Screen::up = 'W';
char Screen::down = 'S';
bool Screen::noKeyPressed = true;


//Screen Statics
char Screen::getKeyPress() {
	if (GetKeyState(up) & 0x8000) {
		if (noKeyPressed) {
			noKeyPressed = false;
			return up;
		}
		noKeyPressed = false;
	}
	else if (GetKeyState(down) & 0x8000) {
		if (noKeyPressed) {
			noKeyPressed = false;
			return down;
		}
		noKeyPressed = false;
	}
	else if (GetKeyState(13) & 0x8000) { //13 is the enter key
		if (noKeyPressed) {
			noKeyPressed = false;
			return select;
		}
		noKeyPressed = false;
	}
	else noKeyPressed = true;
	return empty;
}

void Screen::setUp(char upKey) {
	up = upKey;
}

void Screen::setDown(char downKey) {
	down = downKey;
}


//Menu Class
Menu::Menu(std::string text) : headerText{ text } {

}

void Menu::start() {
	cursorLoc = 0;
	bool exit = false;

	preExecute();

	while (!exit) {
		if (updateScreen) drawScreen();

		updateScreen = true;
		switch (getKeyPress()) {
		case 'W':
			if (cursorLoc > 0) --cursorLoc;
			else cursorLoc = entryCount - 1;
			break;

		case 'S':
			if (cursorLoc < entryCount - 1) ++cursorLoc;
			else cursorLoc = 0;
			break;

		case '\n':
			std::cin.ignore(1000, '\n');
			if (subScreens[cursorLoc] != nullptr)
				subScreens[cursorLoc]->start();
			else if (cursorLoc == exitLoc) exit = true;
			break;

		default:
			updateScreen = false;
			break;
		}
	}
	std::system("CLS");

	postExecute();
}

void Menu::drawScreen() {
	std::system("CLS");

	std::cout << headerText;
	drawExecute();
	std::cout << std::endl << std::endl;

	for (int i = 0; i < entryCount; ++i) {
		if (i == cursorLoc) std::cout << " > ";
		std::cout << menuEntries[i].entryText;
		if (menuEntries[i].isVar) outputVal(menuEntries[i].entryVal);
		std::cout << std::endl;
	}

	updateScreen = false;
}

void Menu::addExit(std::string text) {
	menuEntries.push_back(text);
	subScreens.push_back(nullptr);
	exitLoc = entryCount;
	++entryCount;
}

void Menu::addStaticEntry(std::string text) {
	Entry temp = Entry(text);
	menuEntries.push_back(temp);
	subScreens.push_back(nullptr);
	++entryCount;
}
void Menu::addStaticEntry(std::string text, Screen &entryAction) {
	menuEntries.push_back(Entry(text));
	subScreens.push_back(&entryAction);
	++entryCount;
}

void Menu::addVariableEntry(std::string text, int& variable) {
	menuEntries.push_back(Entry(text, &variable));
	subScreens.push_back(nullptr);
	++entryCount;
}
void Menu::addVariableEntry(std::string text, int& variable, Screen &entryAction) {
	menuEntries.push_back(Entry(text, &variable));
	subScreens.push_back(&entryAction);
	++entryCount;
}

void Menu::addVariableEntry(std::string text, double& variable) {
	menuEntries.push_back(Entry(text, &variable));
	subScreens.push_back(nullptr);
	++entryCount;
}
void Menu::addVariableEntry(std::string text, double& variable, Screen& entryAction) {
	menuEntries.push_back(Entry(text, &variable));
	subScreens.push_back(&entryAction);
	++entryCount;
}

void Menu::addVariableEntry(std::string text, std::string& variable) {
	menuEntries.push_back(Entry(text, &variable));
	subScreens.push_back(nullptr);
	++entryCount;
}
void Menu::addVariableEntry(std::string text, std::string& variable, Screen& entryAction) {
	menuEntries.push_back(Entry(text, &variable));
	subScreens.push_back(&entryAction);
	++entryCount;
}

void Menu::startAction(void(*func)()) {
	preExecute = func;
}

void Menu::drawAction(void(*func)()) {
	drawExecute = func;
}

void Menu::exitAction(void(*func)()) {
	postExecute = func;
}

//Menu Class Private
void Menu::outputVal(std::variant<int*, double*, std::string*> inVariant) {
	std::visit([](auto val) { std::cout << *val; }, inVariant);
}


//Modifier Class
Modifier::Modifier(int &inValue, std::string header, std::string text)
	:value{ &inValue }, headerText { header }, modifierText{ text } {

}
Modifier::Modifier(double &inValue, std::string header, std::string text)
	: value{ &inValue }, headerText{ header }, modifierText{ text } {

}
Modifier::Modifier(std::string &inValue, std::string header, std::string text)
	: value{ &inValue }, headerText{ header }, modifierText{ text } {

}

void Modifier::start() {
	std::string dataTypes[3] = { "an integer", "a double", "a string" };
	drawScreen();
	while (!(std::cin >> value)) {
		std::cout << "Invalid input type; enter ";
		std::cout << dataTypes[value.index()];
		std::cout << " value: ";
		std::cin.clear();
		std::cin.ignore(1000, '\n');
	}
	std::cin.ignore(1000, '\n');
}

void Modifier::drawScreen() {
	std::system("CLS");

	std::cout << headerText;
	std::cout << std::endl << std::endl;
	std::cout << "Current value: ";
	std::visit([](auto val) { std::cout << *val; }, value);
	std::cout << std::endl;
	std::cout << modifierText;
}

std::istream& operator >> (std::istream& is, std::variant<int*, double*, std::string*> variant) {
	std::visit([](auto val) { std::cin >> *val; }, variant);
	return is;
}


//Text Class
Text::Text(std::string header, std::string body)
	: headerText{ header }, bodyText { body } {

}

void Text::start() {
	drawScreen();

	std::cin.ignore(1000, '\n');
	std::cin.clear();
}

void Text::drawScreen() {
	std::system("CLS");

	std::cout << headerText;
	std::cout << std::endl << std::endl;
	std::cout << bodyText << std::endl;
	std::cout << "Press Enter to return . . . \n";
}

void Text::setText(std::string body) {
	bodyText = body;
}


//Custom Class
Custom::Custom(std::string header, void(*lambda)())
	: headerText{ header }, lambdaFunc{ lambda } {

}

void Custom::start() {
	std::system("CLS");
	std::cout << headerText << std::endl << std::endl;
	lambdaFunc();
}

void Custom::setAction(void(*func)()) {
	lambdaFunc = func;
}