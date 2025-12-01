#include "ClassDirectory.hpp"

#include <stdexcept>

#include "CSVReader.hpp"

ClassDirectory::ClassDirectory(const std::string& filename, char delimiter) {
    fromCSV(filename, delimiter);
};

bool ClassDirectory::fromCSV(const std::string& filename, char delimiter) {
    // Read file
    CSVReader reader(filename, delimiter);
    std::vector<std::vector<std::string>> data;
    try {
        data = reader.readAll();
    } catch (const std::runtime_error& e) {
        return false;
    }

    // Parse data from columns
    for (const auto& row : data) {
        std::string classCode = row[0];
        std::string locationId = row[1];
        std::string startTime = row[2];
        std::string endTime = row[3];

        classCodes[classCode] = std::make_tuple(locationId, startTime, endTime);
    }
    return true;
}
