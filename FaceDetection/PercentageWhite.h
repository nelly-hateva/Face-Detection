#pragma once
ref class PercentageWhite
{
	int nameComponent;
	int whiteCounter;

public:
	PercentageWhite(int newNameComponent, int newWhiteCounter) {nameComponent = newNameComponent; whiteCounter = newWhiteCounter;}
	void setNameComponent(int newNameComponent) {nameComponent = newNameComponent; }
	void setWhiteCounter(int newWhiteCounter) {whiteCounter = newWhiteCounter; }
	int getNameComponent() {return nameComponent;}
	int getWhiteCounter() {return whiteCounter;}
};

