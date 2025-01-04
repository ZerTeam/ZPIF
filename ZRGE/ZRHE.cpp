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
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include "../lib/class.hpp"
#include <limits>

#define NPOS std::string::npos

// Добавления в область видемости
using std::cout;
using std::cerr;
using std::endl;

// Сжатие
char compress_rle(std::string &input, std::string &output, Image &img) {
    // Объявления
    std::string line;
    std::string buffer;
    int count = 1;
    bool header_written = false;
    // Открытие файлов
    std::ifstream input_file(input.c_str());
    std::ofstream output_file(output.c_str());

    output_file << "{f}" << '(' << img.format << ")\n";
    output_file << "{c}" << '(' << img.compression << ")\n";
    output_file << "{m}" << '(' << img.mode << ")\n";
    output_file << "{w}" << '(' << img.width << ")\n";
    output_file << "{h}" << '(' << img.height << ")\n";
    output_file << "@s@" << '\n';

    // Проверка на открытие файлов
    if (!input_file.is_open()||!input_file.is_open()) {
        cerr << "\033[31mError opening or create file.\033[0m" << endl;
        return -1;
    }

    while (getline(input_file, line)) {

        if (!header_written) {
            if (line.find("@s@") != NPOS)   header_written = true;
            continue;
        }

        if (line.find("@e@") != NPOS) {
            if (count > 1) {
                output_file << '(' << count << ')' << buffer << '\n';
            } else {
                output_file << buffer << '\n';
            }
            break;
        }

        if (line.find(buffer) != NPOS && !buffer.empty()) {
            count++;
        } else {
            if (count > 1) {
                output_file << '(' << count << ')' << buffer << '\n';
            } else if (!buffer.empty()) {
                output_file << buffer << '\n';
            }
            buffer = line;
            count = 1;
        }
    }
    
    output_file << "@e@";

    // Закрытие
    input_file.close();
    output_file.close();
    return 0;
}

// Сохранения изображения
char savingImage(std::string &filename, sf::Image &image, Image &img)
{
    std::ofstream file(filename.c_str());
    if (!file.is_open()) {
        cerr << "\033[31mError opening/create temp file.\033[0m" << endl;
        return -1;
    }
    std::string buffer;

    if (img.compression == "0")
    {
        file << "{f}" << '(' << img.format << ")\n";
        file << "{c}" << '(' << img.compression << ")\n";
        file << "{m}" << '(' << img.mode << ")\n";
        file << "{w}" << '(' << img.width << ")\n";
        file << "{h}" << '(' << img.height << ")\n";
    }
    file << "@s@" << '\n';
    
    for (u_int i = 0; i < img.width*img.height; i++)
    {
        sf::Color pixelColor = image.getPixel((i % img.width), (i / img.width));
        file << '[' << (u_short)pixelColor.r << " " << (u_short)pixelColor.g << " " << (u_short)pixelColor.b << "]\n";
        img.point++;
    }
    
    file << "@e@" << '\n';
    
    file.close();
    return 0;
}

char saveImage(sf::Image &canvas, Image &img, std::string &filename, std::string &filename_temp, char &Error)
{
    // Очистка
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // Запрос имени файла
    std::cout << "Enter the name of the output file: ";
    getline(std::cin, filename);
    if (img.compression.find("rle") != NPOS)
    {
        cout << "Start create temp file..." << std::endl;
        Error = savingImage(filename_temp, canvas, img);
                    
        if (Error < 0)
            return -1;

        cout << "Create, save and compress image..." << endl;
    
        Error = compress_rle(filename_temp, filename, img);
                    
        if (Error < 0)
            return -1;


        cout << "Remove temp file..." << std::endl;
        remove(filename_temp.c_str());
    }
    else
    {
        Error = savingImage(filename, canvas, img);
        if (Error < 0)
            return -1;
    }
    
    cout << "\033[32mImage saved successfully.\033[0m" << endl;
    return 0;
}

int main() {
    Image img;
    std::string filename_temp = ".tempZRGE_file.tmp";
    std::string filename = "";
    u_int canvasWidth {1};
    u_int canvasHeight {1};
    u_short factor{1};
    char Error {0};

        // Вывод заголовка и запрос данных размеров
        cout << "\033[1;33mWelcome to ZeR Graphics Editor!\033[0m" << endl;
        cout << "\033[1mEnter canvas width: \033[0m";
        std::cin >> canvasWidth;
        cout << "\033[1mEnter canvas height: \033[0m";
        std::cin >> canvasHeight;
        cout << "\033[1mEnter canvas factor: \033[0m";
        std::cin >> factor;

        // Параметры изображения
        img.format = "ZPIFv1";
        img.width = canvasWidth;
        img.height = canvasHeight;
        cout << "\033[1mUse RLE compression? [Y/n] \033[0m";
        std::cin >> img.compression;
        if (img.compression.find('y') != NPOS || img.compression.find('Y') != NPOS)
            img.compression = "rle";
        else
            img.compression = "0";


    // Создания окна
    sf::RenderWindow window(sf::VideoMode(canvasWidth*factor, canvasHeight*factor), "ZeR Graphics Editor", sf::Style::Close);
    sf::Image canvas;
    canvas.create(canvasWidth, canvasHeight, sf::Color::White);

    // Загрузка иконки
    sf::Image icon;
    if (!icon.loadFromFile("icon.png")) 
        std::cerr << "\033[1;33mWARNING: Failed to load icon!\033[0m" << std::endl;
    else
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    //  Создания текстуры
    sf::Texture texture;
    texture.loadFromImage(canvas);
    sf::Sprite sprite(texture);
    // Растягивание текстуры
    sprite.setScale(canvasWidth*factor / texture.getSize().x,canvasHeight*factor / texture.getSize().y);

    sf::Color brushColor = sf::Color::Black;
    bool isErasing = false;
    bool isSave = true;
    int brushSize = 5;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Закрытие
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                if (isSave == false) 
                {
                    std::string answer;
                    
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    
                    cout << "Save image? [Y/n] ";
                    std::cin >> answer;
                    if (answer.find('y')!= NPOS || answer.find('Y')!= NPOS)
                    {
                        Error = saveImage(canvas, img, filename, filename_temp, Error);
                        if (Error < 0)
                            return 1;
                    }
                    cout << "\033[1;33mExit\033[0m" << endl;
                    window.close();
                    return 0;
                }
                    else
                    {
                        cout << "\033[1;33mExit\033[0m" << endl;
                        window.close();
                        return 0;
                    }
            }
            // Мышь
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x/factor;
                    int y = event.mouseButton.y/factor;
                    if (x >= 0 && x < canvasWidth && y >= 0 && y < canvasHeight) {
                        sf::Color colorToUse = isErasing ? sf::Color::White : brushColor;
                        for (int i = -brushSize; i <= brushSize; ++i) {
                            for (int j = -brushSize; j <= brushSize; ++j) {
                                int drawX = x + i;
                                int drawY = y + j;
                                if (drawX >= 0 && drawX < canvasWidth && drawY >= 0 && drawY < canvasHeight) {
                                    canvas.setPixel(drawX, drawY, colorToUse);
                                }
                            }
                        }
                        texture.update(canvas);
                    }
                }
                isSave = false;
            }
            // Клавиатура
            else if (event.type == sf::Event::KeyPressed) {
                // Ластик, стёрка, сброс и задать цвет
                if (event.key.code == sf::Keyboard::E) {
                    isErasing = false;
                } else if (event.key.code == sf::Keyboard::Q) {
                    isErasing = true;
                } else if (event.key.code == sf::Keyboard::R) {
                    canvas.create(canvasWidth, canvasHeight, sf::Color::White);
                    brushColor = sf::Color::Black;
                    brushSize = 5;
                    texture.update(canvas);
                } else if (event.key.code == sf::Keyboard::C) {
                    u_short rgb[3]{0};

                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    std::cout << "Enter RGB values (0-255): ";
                    std::cin >> rgb[0] >> rgb[1] >> rgb[2];
                    if (rgb[0] > 255 || rgb[1] > 255 || rgb[2] > 255) 
                    {
                        std::cerr << "\033[31mRGB values must be between 0 and 255.\033[0m" << endl;
                        continue;
                    }
                    cout << rgb[0] << " " << rgb[1] << " " << rgb[2] << endl;
                    brushColor = sf::Color(rgb[0], rgb[1], rgb[2]);
                }
                // Увелечения и уменьшения размера кисти
                else if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) {
                    brushSize = std::min(brushSize + 2, 50);
                }
                else if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Hyphen) {
                    brushSize = std::max(brushSize - 2, 1);
                }            
                // Сохранение
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    Error = saveImage(canvas, img, filename, filename_temp, Error);
                    if (Error < 0)
                        return 1;
                    isSave = true;
                }
            }

        }
        
        // Мышь
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int x = mousePos.x/factor;
            int y = mousePos.y/factor;
            if (x >= 0 && x < canvasWidth && y >= 0 && y < canvasHeight) {
                sf::Color colorToUse = isErasing ? sf::Color::White : brushColor;
                for (int i = -brushSize; i <= brushSize; ++i) {
                    for (int j = -brushSize; j <= brushSize; ++j) {
                        int drawX = x + i;
                        int drawY = y + j;
                        if (drawX >= 0 && drawX < canvasWidth && drawY >= 0 && drawY < canvasHeight) {
                            canvas.setPixel(drawX, drawY, colorToUse);
                        }
                    }
                }
                texture.update(canvas);
            }
            isSave = false;
        }

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.display();
    }

    return 0;
}
