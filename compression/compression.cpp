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
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define NPOS std::string::npos

// Преобразование числа в big-endian
inline void convertNumberInBE(u_short value, std::vector<u_int8_t> &buffer)
{
    buffer[0] = value >> 8;
    buffer[1] = value & 0xFF;
}

// Функция для перевода текса в строке к нижнему регистру
std::string toLowerCase(const std::string &input)
{
    std::string result;
    for (char c : input)
    {
        result += std::tolower(c);
    }
    return result;
}

// Функция для получения текста из заданных символов
std::string extractContent(const std::string &str, char startChar, char endChar)
{
    std::string result;
    size_t start = 0;

    while ((start = str.find(startChar)) != NPOS)
    {
        size_t end = str.find(endChar, start + 1);
        if (end != NPOS)
        {
            if (!result.empty())
            {
                break;
            }
            result += str.substr(start + 1, end - start - 1);
        }
        else
        {
            break;
        }
    }

    return result;
}

void compress_rle(std::ifstream &inputFile, std::ofstream &outputFile)
{
    std::string line;
    std::string buffer;
    std::string data;
    std::vector<u_int8_t> bufferInp(6), // Буфер для чтения
                          bufferOut(6); // Буфер для записи
    unsigned short count = 1;

    outputFile << '\xDD' << "ZPIF\n";

    while (std::getline(inputFile, line))
    {
        line = toLowerCase(line);

        if (line.find('(') != NPOS && line.find(')') != NPOS)
            data = extractContent(line, '(', ')');

        if (line.find("{width}") != NPOS || line.find("{w}") != NPOS)
            outputFile << "{w}(" << data << ")\n";
        else if (line.find("{height}") != NPOS || line.find("{h}") != NPOS)
            outputFile << "{h}(" << data << ")\n";
        else if (line.find("{date}") != NPOS || line.find("{d}") != NPOS)
            outputFile << "{d}(" << data << ")\n";
        else if (line.find("{time}") != NPOS || line.find("{t}") != NPOS)
            outputFile << "{t}(" << data << ")\n";
        else if (line.find("{gmt}") != NPOS || line.find("{g}") != NPOS)
            outputFile << "{g}(" << data << ")\n";
        else if (line.find("{date_unix}") != NPOS || line.find("{u}") != NPOS)
            outputFile << "{u}(" << data << ")\n";
        else if (line.c_str() == std::string("\x00\x00\xFF\xFF\xFF\xFF"))
        {
            outputFile.write("\x00\x00\xFF\xFF\xFF\xFF\n", 7);
            break;
        }
    }

    // Чтение первого блока
    if (inputFile.read(reinterpret_cast<char *>(bufferOut.data()), bufferOut.size()))
    {
        // Чтения остальных блоков
        while (inputFile.read(reinterpret_cast<char *>(bufferInp.data()), bufferInp.size()))
        {
            // Завершаем сжатие при встрече 6 нулевых байтов (конца файла)
            if (bufferInp == std::vector<u_int8_t>(6, 0x00))
                break;

            // Проверка данных из буфера out и буфера inp (одинаковые ли они) и не превышает ли count двух байтов
            if (bufferInp == bufferOut && count < 65535)
                count++;
            else
            {
                // Проверка нужна ли запись count в блок буфера
                if (count > 1)
                {
                    convertNumberInBE(count, bufferOut);
                }
                outputFile.write(reinterpret_cast<char *>(bufferOut.data()), bufferOut.size());
                bufferOut = bufferInp;
                count = 1;
            }
        }
        if (count > 1)
        {
            convertNumberInBE(count, bufferOut);
        }
        outputFile.write(reinterpret_cast<char *>(bufferOut.data()), bufferOut.size());
    }
    outputFile.write("\x00\x00\x00\x00\x00\x00", 6);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n";
        // return 1;
    }

    std::ifstream input_file("/home/admin/Рабочий стол/ZPIF/render/test.zpif", std::ios::binary);
    std::ofstream output_file("./tt.zpif", std::ios::binary);
    if (!input_file || !output_file)
    {
        std::cerr << "Error opening input file\n";
        return 1;
    }

    compress_rle(input_file, output_file);

    input_file.close();
    output_file.close();

    return 0;
}