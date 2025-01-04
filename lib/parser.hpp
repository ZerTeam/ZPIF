#ifndef _PARSER_H_
#define _PARSER_H_

// Библиотеки
#include <string>
#include <algorithm>
#include <iostream>
#include <cctype>
#include "class.hpp"
#include "render.hpp"

// npos
#define NPOS std::string::npos

// Добавления в область видемости
using std::cout;
using std::cerr;
using std::endl;

// Класс для пораметра
class Parameter
{
    // Имя и значения
    std::string name;
    std::string value;

public:
    // Конструкторы для присвоения имени и значения
    Parameter() : name(""), value("")
    {}
    Parameter(std::string n, std::string v) : name(n), value(v)
    {}
    // Получения имени
    std::string getName() const { return name; }
    // Получения значения
    std::string getValue() const { return value; }
};

// Функция для проверки существования заданого символа в строке
bool containsChar(const std::string& str, char ch) {
    return str.find(ch) != NPOS;
}

bool containsString(const std::string& str, const std::string& text) {
    return str.find(text) != NPOS;
}

// Функция для удаления пробелов
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == NPOS) {
        return ""; // Строка состоит только из пробелов
    }
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

// Функция для перевода текса в строке к нижнему регистру
std::string toLowerCase(const std::string& input) {
    std::string result;
    for (char c : input) {
        result += std::tolower(c);
    }
    return result;
}

// Функция для получения такста из заданных символов 
std::string extractContent(const std::string& str, char startChar, char endChar) {
    std::string result;
    size_t start = 0;

    while ((start = str.find(startChar, start)) != NPOS) {
        size_t end = str.find(endChar, start+1);
        if (end != NPOS) {
            if (!result.empty()) {
                break;
            }
            result += str.substr(start + 1, end - start - 1);
        } else {
            break;
        }
    }

    return result;
}

// Функция парсинга
short parserParam(std::string &str, Image &img)
{
    str = toLowerCase(str);
    if (containsChar(str, '{') && containsChar(str, '}') && containsChar(str, '(') && containsChar(str, ')'))
    {
        // Создания объекта и присвоения значений
        Parameter param{extractContent(str, '{', '}'), extractContent(str, '(', ')')};
        // Получения данных
        if (containsString(param.getName(), "f"))
            img.format=param.getValue();
        if (containsString(param.getName(), "m"))
            img.mode=param.getValue();
        if (containsString(param.getName(), "c"))
            img.compression=param.getValue();
        // Получения размеров
        if (containsString(param.getName(), "w"))
            img.width=std::strtoul((param.getValue().data()), nullptr, 10);
        if (containsString(param.getName(), "h"))
            img.height=std::strtoul((param.getValue().data()), nullptr, 10);
    }
    
    else if (containsString(str, "@s@"))
    {
        if (img.format==""||img.mode==""||img.compression==""||img.width==0||img.height==0)
            {
            cerr << "\033[1;31mError 2: The file is damaged or the format is not supported.\033[0m" << std::endl;
            return -2;
        }
            
        img.randerStart = true;
        return 1;
    }

    else{
        cerr << "\033[1;31mError 1: The file is damaged or the format is not supported.\033[0m" << std::endl;
        return -1;
    }
    return 0;
}

short parserPixel(std::string &str, Image &img)
{
    str = toLowerCase(str);

    // Обработка строки с RGB
    if (containsChar(str, '[') && containsChar(str, ']') && img.randerStart) {
        std::string content = extractContent(str, '[', ']');

        if (containsChar(str, '(') && containsChar(str, ')') && containsString(img.compression, "0"))
        {
            cerr << "\033[1;31mError 2: The file is damaged or the format is not supported.\033[0m" << endl;
            return -2;
        }

        if (containsChar(str, '(') && containsChar(str, ')') && containsString(img.compression, "rle"))
        {
            std::string number = extractContent(str, '(', ')');
            sscanf(number.c_str(), "%d", &img.quantity);
            img.quantity--;
        }

        if (sscanf(content.c_str(), "%hu %hu %hu", &img.rgb[0], &img.rgb[1], &img.rgb[2]) == 3) {
            img.point++; // Инкремент только при успешном парсинге
            return 0;    // Продолжаем
        } else {
            cerr << "\033[1;31mError 2: The file is damaged or the format is not supported.\033[0m" << endl;
            return -2;
        }
    }

    // Обработка команды @END@
    if (containsString(str, "@e@")) {
        std::cout << "\033[32mRendering completed\033[0m" << std::endl;
        img.randerStart = false;
        // Проверка на равенства
        if(img.point + img.quantity < img.width * img.height) cout << "\033[1;33mWARNING: Number of pixels is less than required\033[0m" << endl;
        return 1; // Завершаем
    }

    return 0;
}

#endif