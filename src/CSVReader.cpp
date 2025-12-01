#include "CSVReader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

std::vector<std::vector<std::string>> CSVReader::readAll() {
    std::vector<std::vector<std::string>> data;
    std::ifstream file{filename};
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << filename << std::endl;
        throw std::runtime_error("File open error");
    }

    // Discard first line (header)
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, delimiter)) row.push_back(cell);
        data.push_back(row);
    }

    file.close();
    return data;
}
