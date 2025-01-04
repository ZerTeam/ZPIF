#ifndef _CLASSES_
#define _CLASSES_

// Библиотеки
#include <string>

typedef unsigned short u_short;

// Класс изображения
class Image
{
public:
    std::string format{""}; // Фармат изображения
    std::string mode{"rgb"}; // Режим отображения цвета например rgb
    std::string compression{""}; // Сжатие
    u_short width{0}; // Ширина изображения
    u_short height{0}; // Высота изображения
    u_int quantity; // Количество под ряд (для сжатых изображений)

    uint8_t rgb[3]{0}; // Цвет пикселя

    u_int point{0}; // Позиция пикселя заполнения с лево на право сверху вниз

    bool randerStart{false}; // Разрешения на рендер
};
#endif