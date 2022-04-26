#include <iostream>
#include <fstream>
#include <vector>

#include "Headers.h"
#include "BMPEditor.h"

using namespace std;

int main()
{
	BMPEditor image1{};

	RGBAColor white{ 255,255,255,255 };
	RGBAColor red{ 255,0,0,255 };
	RGBAColor blue{ 0,0,255,255 };
	image1.open("shapes.bmp");
	image1.fill(white);
	image1.drawRectangle(100,100, 200, 200, red);
	image1.drawCircle(300, 300, 25, blue);
	image1.save("caliss.bmp");
}