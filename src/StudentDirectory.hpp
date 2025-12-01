#pragma once

#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>

struct Student {
    std::string id;  // 8-digit string
    std::string name;
    std::string residenceLocationId;
    std::unordered_set<std::string> classCodes;

    static bool validateId(const std::string& id) {
        if (id.length() != 8) return false;
        return std::all_of(id.begin(), id.end(),
                           [](unsigned char c) { return std::isdigit(c); });
    }
    static bool validateName(const std::string& name) {
        // Must be A-Z, a-z, spaces only and not empty
        if (name.empty()) return false;

        return std::all_of(name.begin(), name.end(), [](unsigned char c) {
            return std::isalpha(c) || std::isspace(c);
        });
    }
};

struct StudentDirectory {
    std::vector<Student> students;

    StudentDirectory() = default;
    ~StudentDirectory() = default;

    // Validation is done in Compass, so these functions do not need to return
    std::vector<Student>::iterator findStudent(const std::string id);
    void insert(const std::string& name, const std::string& id,
                const std::string& residenceLocationId,
                const std::vector<std::string>& classCodes);
    void removeStudent(const std::string id);
    void dropClass(const std::string& id, const std::string& classCode);
    void replaceClass(const std::string& id, const std::string& oldClassCode,
                      const std::string& newClassCode);
    int removeClass(const std::string& classCode);
};
