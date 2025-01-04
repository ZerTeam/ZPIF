#include <iostream>
#include <fstream>
#include <string>

#define NPOS std::string::npos

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

    while ((start = str.find(startChar)) != NPOS) {
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

void compress_rle(std::ifstream &input_file, std::ofstream &output_file) {
    std::string line;
    std::string buffer;
    std::string data;
    int count = 1;
    bool header_written = false;

    while (std::getline(input_file, line)) {
        line = toLowerCase(line);

        if (!header_written) {

            if (line.find('(') != NPOS && line.find(')') != NPOS)
            {
                data = extractContent(line, '(', ')');
            }
            
            if (line.find("{compression}") != NPOS || line.find("{c}") != NPOS) 
                output_file << "{c}(rle)\n";
             else if (line.find("{format}") != NPOS || line.find("{f}") != NPOS) 
                output_file << "{f}(" << data << ")\n";
             else if (line.find("{mode}") != NPOS || line.find("{m}") != NPOS) 
                output_file << "{m}(" << data << ")\n";
             else if (line.find("{width}") != NPOS || line.find("{w}") != NPOS)
                output_file << "{w}(" << data << ")\n";
             else if (line.find("{height}") != NPOS || line.find("{h}") != NPOS)
                output_file << "{h}(" << data << ")\n";
             else if (line.find("{date}") != NPOS || line.find("{d}") != NPOS)
                output_file << "{d}(" << data << ")\n";
             else if (line.find("{time}") != NPOS || line.find("{t}") != NPOS)
                output_file << "{t}(" << data << ")\n";
             else if (line.find("{gmt}") != NPOS || line.find("{g}") != NPOS)
                output_file << "{g}(" << data << ")\n";
             else if (line.find("{date_unix}") != NPOS || line.find("{u}") != NPOS) 
                output_file << "{u}(" << data << ")\n";
            if (line.find("@s@") != NPOS) {
                header_written = true;
                output_file << line << "\n";
            }
            continue;
        }

        if (line.find("@e@") != NPOS) {
            if (count > 1) {
                output_file << '(' << count << ')' << buffer << '\n';
            } else {
                output_file << buffer << '\n';
            }
            output_file << line;
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
} 

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n";
        return 1;
    }

    std::ifstream input_file(argv[1]);
    std::ofstream output_file(argv[2], std::ios::binary);
    if (!input_file||!output_file) {
        std::cerr << "Error opening input file\n";
        return 1;
    }

    compress_rle(input_file, output_file);

    input_file.close();
    output_file.close();

    return 0;
}