/*
    MIT License

    Copyright (c) 2025 Zakhar Shakhanov

    Permission is hereby granted, free of charge, to any person obtaining a copy  
    of this software and associated documentation files (the "Software"), to deal  
    in the Software without restriction, including without limitation the rights  
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell  
    copies of the Software, and to permit persons to whom the Software is  
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all  
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,  
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE  
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER  
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  
    SOFTWARE.
*/

// Библиотеки
#include <iostream>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

// Собственные библиотеки
#include "./lib/render.hpp"
#include "./lib/parser.hpp"

// Добавления в область видемости
using std::cout;
using std::cerr;
using std::endl;

// Главная функция
int main(int argc, char *argv[]) {
    if (argc<2)
    {
        cerr << "\033[33mUsage: " << argv[0] << " <file_path>\033[0m" << endl;
        return 1;
    }
       
    // Переменные и тд
    std::string filePath{argv[1]};
    std::string line; // Строка для парсинга
    short Error{0}; // Переменная для ошибок
    Image img;// Данные изображения

    // Открытие файла
    std::ifstream file{filePath};

    // Проверка на открытие файла
    if (!file.is_open()) {
        cerr << "\033[1;31mError opening file.\033[0m" << endl;
        return 1;
    }

    // Парсинг параметров из файла
    while (getline(file, line))
    {
        // Пропуск пустых строк.
        if (line[0]=='\n'||line.empty()) continue;

        // Вызов парсера на текущей строке
        Error = parserParam(line, img);
        if (Error == 1) break;
        if (Error != 0) return 1;
    }    

    // Рендер

    Error = render(file, img, filePath.data(), Error);

    if (Error < 0)  return 1;
    
    // Закрытие файла
    file.close();

    return 0;
}
/*
 ID: HM0100; Author: Zer Team; Date: 2025/01/01
*/