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
            if (isInFrame(j, i))
            {
                m_data[getIndex(j, i)] = color.blue;
                m_data[getIndex(j, i) + 1] = color.green;
                m_data[getIndex(j, i) + 2] = color.red;
                m_data[getIndex(j, i) + 3] = color.alpha;
            }
        }
    }
}

int BMPEditor::getIndex(int x, int y)
{
    return (4 * ((m_infoHeader.height - y - 1) * m_infoHeader.width + x));
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
                if (isInFrame(j, i))
                {
                    m_data[getIndex(j, i)] = color.blue;
                    m_data[getIndex(j, i) + 1] = color.green;
                    m_data[getIndex(j, i) + 2] = color.red;
                    m_data[getIndex(j, i) + 3] = color.alpha;
                }
            }
        }
    }
}

bool BMPEditor::isInFrame(int x, int y)
{
    return x >= 0 && x < m_infoHeader.width && y >= 0 && y < m_infoHeader.height;
}

void BMPEditor::invert()
{
    for (int i = 0; i < m_data.size(); i += 4)
    {
        m_data[i] = ~m_data[i];
        m_data[i+1] = ~m_data[i+1];
        m_data[i+2] = ~m_data[i+2];
    }
}

void BMPEditor::blur(int strength)
{
    for (int x = 0; x < m_infoHeader.width; x++)
    {
        for (int y = 0; y < m_infoHeader.height; y++)
        {
            RGBAColor color = getBlurColor(x,y, strength);
            setColor(x, y, color);
        }
    }
}

RGBAColor BMPEditor::getBlurColor(int x, int y, int strength)
{
    int totalR{ 0 };
    int totalG{ 0 };
    int totalB{ 0 };
    int totalA{ 0 };
    int numPixel{ 0 };

    for (int i = x-strength; i <= x+strength; i++)
    {
        for (int j = y - strength; j <= y + strength; j++)
        {
            if (isInFrame(i, j) )
            {
                int index{ getIndex(i,j) };
                totalB += m_data[index];
                totalG += m_data[index+1];
                totalR += m_data[index+2];
                totalA += m_data[index+3];
                numPixel++;
            }
        }
    }

    return RGBAColor{ uint8_t(totalR / numPixel), uint8_t(totalG / numPixel),uint8_t(totalB / numPixel),uint8_t(totalA / numPixel) };
}

void BMPEditor::setColor(int x, int y, RGBAColor color)
{
    int index{ getIndex(x,y) };
    m_data[index] = color.blue;
    m_data[index+1] = color.green;
    m_data[index+2] = color.red;
    m_data[index+3] = color.alpha;
}

void BMPEditor::hideMessage(std::string message)
{
    for (int i = 0; i < message.length(); i++)
    {
        uint8_t currentChar{ uint8_t(message[i]) };
        for (int j = 0; j < 8; j++)
        {
            uint8_t lastBit = (currentChar >> abs(j - 7)) & 0b00000001;
            uint8_t maskedByte = (m_data[i * 8 + j] & 0b11111110);
            uint8_t newByte = ( lastBit | maskedByte);
            m_data[i * 8 + j] = newByte;
        }
    }

    //Byte de fin (\0 ou 0b00000000)
    for (int j = 0; j < 8; j++)
    {
        uint8_t currentChar = 0b00000000;
        uint8_t lastBit = (currentChar >> abs(j - 7)) & 0b00000001;
        uint8_t maskedByte = (m_data[message.length() * 8 + j] & 0b11111110);
        uint8_t newByte = (lastBit | maskedByte);
        m_data[message.length() * 8 + j] = newByte;
    }
}

std::string BMPEditor::readMessage()
{
    std::string message = "";
    //Initialisation 255 pour pouvoir passer le while
    char currentChar = 0b11111111;
    int charIndex{ 0 };
    while (currentChar != 0b00000000 && charIndex < m_data.size() - 8)
    {
        currentChar = 0b00000000;
        for (int j = 0; j < 8; j++)
        {
            char temp1{ (m_data[charIndex + j] & 0b00000001) };
            char temp{ (temp1 << abs(j - 7)) };
            currentChar = currentChar | temp;
        }
        if (currentChar != 0b00000000)
        {
            message.push_back(currentChar);
        }
        charIndex+=8;
    }
    //Pas de message
    if (charIndex >= m_data.size() - 8)
    {
        return "";
    }
    return message;
}

