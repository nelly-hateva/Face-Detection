#pragma once
class Pixel
{
public:
	int X;
	int Y;

	Pixel(int x, int y) { X = x; Y = y; }
	void setX(int x) { X = x; }
	int getX() { return X; }
	void setY(int y) { Y = y; }
	int getY() { return Y; }
};

