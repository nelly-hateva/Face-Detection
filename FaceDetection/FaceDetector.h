#include "vector"
#include "RetCoord.h"

using namespace std;
using namespace System::Drawing;
using namespace System::Collections::Generic;

#pragma once

public ref class FaceDetector
{
  public:

	static Color FaceColor(Color pixelColor);
	static Bitmap^ NormalizeImageBrightness(Bitmap^ image);
	static Bitmap^ GrayScale(Bitmap^ bitmap);
	static Bitmap^ BlackAndWhite(Bitmap^ image);
	static vector< vector<int> > ConectedComponents(vector< vector<int> > matrix);
	static vector< vector<int> > Lrtb(vector< vector<int> > matrix);
	static List< RetCoord^ > ^ CountComponent(vector< vector<int> > matr);

};
