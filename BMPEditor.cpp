#include "BMPEditor.h"
#include <iostream>
#include <fstream>
#include "Headers.h"
#include <cmath>

BMPEditor::BMPEditor() {};

void BMPEditor::open(std::string filename)
{
    std::ifstream image{ filename, std::ios::in | std::ios::binary };
    if (image.is_open())
    {
        image.read((char*)&m_fileHeader, sizeof(m_fileHeader));
        image.read((char*)&m_infoHeader, sizeof(m_infoHeader));
        image.read((char*)&m_colorHeader, sizeof(m_colorHeader));

        m_data.resize(m_infoHeader.size_image);

        image.seekg(m_fileHeader.offset, image.beg);
        image.read((char*)m_data.data(), m_data.size());

        image.close();
    }
}
void BMPEditor::fill(RGBAColor color)
{
    for (int i = 0; i < m_data.size(); i += 4)
    {
        m_data[i] = color.blue;
        m_data[i + 1] = color.green;
        m_data[i + 2] = color.red;
        m_data[i + 3] = color.alpha;
    }
}

void BMPEditor::save(std::string filename)
{
    std::ofstream outImage{ filename, std::ios_base::binary };
    if (outImage)
    {
        outImage.write((char*)&m_fileHeader, sizeof(m_fileHeader));
        outImage.write((char*)&m_infoHeader, sizeof(m_infoHeader));
        outImage.write((char*)&m_colorHeader, sizeof(m_colorHeader));
        outImage.write((char*)m_data.data(), m_data.size());
    }
}

void BMPEditor::drawRectangle(int x1, int y1, int x2, int y2, RGBAColor color)
{
    int indexDepart{ int(m_data.size()) - (4 * (y1 * int(m_infoHeader.width) + x1)) };

    for (int i = y1; i < (y2); i++)
    {
        for (int j = x1; j < x2; j++)
        {
            m_data[getIndex(j, i)] = color.blue;
            m_data[getIndex(j, i) + 1] = color.green;
            m_data[getIndex(j, i) + 2] = color.red;
            m_data[getIndex(j, i) + 3] = color.alpha;
        }
    }
}

int BMPEditor::getIndex(int x, int y)
{
    return (4 * ((m_infoHeader.height - y) * m_infoHeader.width + x));
}

void BMPEditor::drawCircle(int x, int y, int radius, RGBAColor color)
{
    int x1 = x - radius;
    int x2 = x + radius;
    int y1 = y - radius;
    int y2 = y + radius;

    int indexDepart{ int(m_data.size()) - (4 * (y1 * int(m_infoHeader.width) + x1)) };

    for (int i = y1; i < (y2); i++)
    {
        for (int j = x1; j < x2; j++)
        {
            if (pow(j - x, 2) + pow(i - y, 2) - pow(radius, 2) < 0)
            {
                m_data[getIndex(j, i)] = color.blue;
                m_data[getIndex(j, i) + 1] = color.green;
                m_data[getIndex(j, i) + 2] = color.red;
                m_data[getIndex(j, i) + 3] = color.alpha;
            }
        }
    }
}