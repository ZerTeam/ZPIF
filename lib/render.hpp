#ifndef _RENDER_HPP_
#define _RENDER_HPP_

// Библиотеки
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "class.hpp"
#include "parser.hpp"

// Функция рендера
short render(std::istream &file, Image &img, char *windowName, short &Error)
{
    // Объекты
    std::string str;
    u_char factor {1};

    if (img.width<200&&img.height<200)
        factor = 3; // Коэффициент уменьшения
    else if (img.width<20&&img.height<20)
        factor = 50; // Коэффициент уменьшения

    cout << "\033[33mStart rendering\033[0m" << endl;

    sf::Image icon;
    if (!icon.loadFromFile("logo_zpif.png")) { 
        std::cerr << "Error 4: Failed to load icon!" << std::endl;
        return -4;
    }

    // Переменные для хранения состояния окна
    bool isFullscreen = false; // Текущее состояние (false = оконный режим)
    sf::VideoMode windowedMode(img.width*factor, img.height*factor); // Размеры окна в оконном режиме
    sf::VideoMode fullscreenMode = sf::VideoMode::getDesktopMode(); // Размеры для полноэкранного режима
    
    // Создания окно
    sf::RenderWindow window(windowedMode, windowName, sf::Style::Close);

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Создания изображение и текстуры
    sf::Image image;
    image.create(img.width, img.height, sf::Color::Cyan); // Изображение

    sf::Texture texture;
    texture.loadFromImage(image); // Загрузка изображение в текстуру

    // Создания спрайта для отображения текстуры
    sf::Sprite sprite(texture);

    // Основной цикл программы
    while (window.isOpen()) {
        sf::Event event;

        // Обработка событий
        while (window.pollEvent(event)) {
            // Закрытие окна
            if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                cout << "\033[33mExit\033[0m" << endl;
                window.close();
            }
            // Открытие на весь экран
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F11) {
                // Закрываем текущее окно
                window.close();

                // Переключаем режим
                isFullscreen = !isFullscreen;

                // Создаем новое окно с нужным стилем
                if (isFullscreen) {
                    window.create(fullscreenMode, "Fullscreen Mode", sf::Style::Fullscreen);
                } else {
                    window.create(windowedMode, "Windowed Mode", sf::Style::Close);
                }
            }
            // Обработка изменения размера окна
            if (event.type == sf::Event::Resized) {
                // Новые размеры окна
                float newWidth = static_cast<float>(event.size.width);
                float newHeight = static_cast<float>(event.size.height);

                // Масштабируем спрайт, чтобы он соответствовал размеру окна
                sprite.setScale(newWidth / texture.getSize().x, newHeight / texture.getSize().y);
            }
            window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        }

        // Парсинг и заполнения пикселя
        while (getline(file, str)) {
            str = trim(str);

            // Пропускаем пустые строки и комментарии
            if (str.empty() || str[0] == ';' || str[0] == '\n')    continue;

            // Парсинг пикселя
            Error = parserPixel(str, img);

            if (Error < 0)  return -1;
            if (Error == 1) break; // Завершение рендера

            // Проверка на равенства
            if (img.point > img.width * img.height) break;
            
            // Устанавливаем цвет пикселя
            sf::Color color(img.rgb[0], img.rgb[1], img.rgb[2]);
            
            // Обработка сжатия
            if (img.quantity>0 && containsString(img.compression, "rle"))
            {
                while(img.quantity>0 && (img.point < img.width * img.height))
                {
                    image.setPixel(((img.point-1) % img.width), ((img.point-1) / img.width), color);
                    img.quantity--;
                    img.point++;
                }
                
            }
            // Заполнения пикселя
            image.setPixel(((img.point-1) % img.width), ((img.point-1) / img.width), color);
            // Проверка на равенства
            if(img.point + img.quantity > img.width * img.height) cout << "\033[1;33mWARNING: Number of pixels exceeds available\033[0m" << endl;
        }

        // Обновления текстуру из изменённого изображения
        texture.update(image);

        // Отрисовка
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}

#endif