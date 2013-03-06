using namespace System::Drawing;
#pragma once
namespace ColorConversion{
  ref class ColorConverter
  {
  public:
	ColorConverter(void);
	static Color ColorFromRGB(Color pixelColor);
  };
}
