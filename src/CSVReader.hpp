#pragma once

#include <string>
#include <vector>

class CSVReader {
   private:
    std::string filename;
    char delimiter;

   public:
    CSVReader(const std::string& filename, char delimiter = ',')
        : filename(filename), delimiter(delimiter) {}

    std::vector<std::vector<std::string>> readAll();
};
