#pragma once

#include <cstdint>

// La commande prama pack permet de condenser les structures
// Elle ordonne on compilateur de ne pas ajouter de bits lors de la compilation
#pragma pack(push, 1)
struct BMPFileHeader
{
    uint16_t file_type{ 0x4D42 }; // Toujours 0x4D42 qui représente BM
    uint32_t file_size{ 0 }; // Taille du fichier en octets (bytes)
    uint16_t reserved1{ 0 }; 
    uint16_t reserved2{ 0 }; 
    uint32_t offset{ 0 }; // L'endroit où les pixel commencent à partir du début du fichier (en octets)
};

struct BMPInfoHeader {
    uint32_t size{ 0 }; // Taille de cet en-tête en octets
    int32_t width{ 0 }; // Largeur de l'image en pixels
    int32_t height{ 0 }; // Hauteur de l'image en pixels
    uint16_t planes{ 1 }; //
    uint16_t bit_count{ 0 }; // nb de bits/pixel (32,24,etc)
    uint32_t compression{ 0 };
    uint32_t size_image{ 0 };
    int32_t x_pixels_per_meter{ 0 };
    int32_t y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };
    uint32_t colors_important{ 0 };
};

struct BMPColorHeader // Masques pour les couleur et la transparence
{
    uint32_t red_mask{ 0x00ff0000 };
    uint32_t green_mask{ 0x0000ff00 };
    uint32_t blue_mask{ 0x000000ff };
    uint32_t alpha_mask{ 0xff000000 };
    uint32_t color_space{ 0x73524742 };
    uint32_t unused[16]{ 0 };
};

struct RGBAColor
{
    uint8_t red{ 0 };
    uint8_t green{ 0 };
    uint8_t blue{ 0 };
    uint8_t alpha{ 0 };
};

#pragma pack(pop)