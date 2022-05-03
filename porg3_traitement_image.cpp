#include <iostream>
#include <fstream>
#include <vector>

#include "Headers.h"
#include "BMPEditor.h"

using namespace std;

int main()
{
	BMPEditor image1{};
	image1.open("message.bmp");
	std::cout << image1.readMessage();
}