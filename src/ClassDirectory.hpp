#include <string>
#include <unordered_map>

struct ClassDirectory {
    // Maps class code to (location ID, start time, end time)
    std::unordered_map<std::string,
                       std::tuple<std::string, std::string, std::string>>
        classCodes;

    ClassDirectory(const std::string& filename, char delimiter);
    ~ClassDirectory() = default;

    std::unordered_map<std::string, std::tuple<std::string, std::string,
                                               std::string>>::iterator
    findClassCode(const std::string& locationId) {
        return classCodes.find(locationId);
    }
    // Returns bool to indicate success or failure
    bool fromCSV(const std::string& filename, char delimiter = ',');
};
