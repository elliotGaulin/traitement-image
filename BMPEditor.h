#pragma once
#include "Headers.h"
#include <vector>
#include <string>
class BMPEditor
{
private:
	std::vector<uint8_t> m_data{};
	BMPFileHeader m_fileHeader{};
	BMPInfoHeader m_infoHeader{};
	BMPColorHeader m_colorHeader{};

	int getIndex(int x,int y);
public:
	BMPEditor();
	void open(std::string);
	void fill(RGBAColor color);
	void save(std::string);
	void drawRectangle(int x1, int y1, int x2, int y2, RGBAColor color);
	void drawCircle(int x, int y, int radius, RGBAColor color);
};
