#pragma once
#include <string>
#include <iostream>
#include <windows.h>
#include <vector>
#include <variant>

class Screen {
public:
	virtual void start() = 0;
	static char getKeyPress();

	static void setUp(char upKey);
	static void setDown(char downKey);

private:
	static const char empty = '\0';
	static const char select = '\n';
	static char up;
	static char down;
	static bool noKeyPressed;
};


class Menu : public Screen {
public:
	Menu(std::string text = std::string());

	void start();
	void drawScreen();

	void addExit(std::string text);
	void addStaticEntry(std::string text);
	void addStaticEntry(std::string text, Screen &entryAction);
	void addVariableEntry(std::string text, int &variable);
	void addVariableEntry(std::string text, int &variable, Screen &entryAction);
	void addVariableEntry(std::string text, double &variable);
	void addVariableEntry(std::string text, double &variable, Screen& entryAction);
	void addVariableEntry(std::string text, std::string &variable);
	void addVariableEntry(std::string text, std::string &variable, Screen& entryAction);

	void startAction(void(*func)());
	void drawAction(void(*func)());
	void exitAction(void(*func)());

private:
	struct Entry {
		std::string entryText;
		std::variant<int*, double*, std::string*> entryVal;
		bool isVar;

		Entry(std::string text) : entryText{ text }, isVar{ false } { }
		Entry(std::string text, int *val) : entryText{ text }, entryVal{ val }, isVar{ true } { }
		Entry(std::string text, double *val) : entryText{ text }, entryVal{ val }, isVar{ true } { }
		Entry(std::string text, std::string *val) : entryText{ text }, entryVal{ val }, isVar{ true } { }
	};

	std::vector<Entry> menuEntries;
	std::vector<Screen*> subScreens;
	std::string headerText;
	void(*preExecute)() = []() { return; };
	void(*drawExecute)() = []() { return; };
	void(*postExecute)() = []() { return; };
	int entryCount = 0;
	int cursorLoc = 0;
	int exitLoc = -1;
	bool updateScreen = true;

	void outputVal(std::variant<int*, double*, std::string*> inVariant);
};


class Modifier : public Screen {
public:
	Modifier(int &inValue, std::string header, std::string text = "Enter new value: ");
	Modifier(double &inValue, std::string header, std::string text = "Enter new value: ");
	Modifier(std::string &inValue, std::string header, std::string text = "Enter new value: ");

	void start();
	void drawScreen();

private:
	std::variant<int*, double*, std::string*> value;
	std::string headerText;
	std::string modifierText;
};

std::istream& operator >> (std::istream& is, std::variant<int*, double*, std::string*> variant);


class Text : public Screen {
public:
	Text(std::string header, std::string body = std::string());

	void start();
	void drawScreen();

	void setText(std::string body);

private:
	std::string headerText;
	std::string bodyText;
};


class Custom : public Screen {
public:
	Custom(std::string header, void(*lambda)() = [] { return; });

	void setAction(void(*func)());

	void start();

private:
	std::string headerText;
	void(*lambdaFunc)();
};