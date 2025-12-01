#include "StudentDirectory.hpp"

#include <algorithm>

std::vector<Student>::iterator StudentDirectory::findStudent(
    const std::string id) {
    auto it =
        std::find_if(students.begin(), students.end(),
                     [&](const Student& student) { return student.id == id; });
    return it;
}

void StudentDirectory::insert(const std::string& name, const std::string& id,
                              const std::string& residenceLocationId,
                              const std::vector<std::string>& classCodes) {
    Student newStudent;
    newStudent.name = name;
    newStudent.id = id;
    newStudent.residenceLocationId = residenceLocationId;
    for (const auto& classCode : classCodes) {
        newStudent.classCodes.insert(classCode);
    }
    students.push_back(newStudent);
}

void StudentDirectory::removeStudent(const std::string id) {
    auto it = findStudent(id);
    students.erase(it);
}

void StudentDirectory::dropClass(const std::string& id,
                                 const std::string& classCode) {
    auto it = findStudent(id);
    auto classIt = it->classCodes.find(classCode);

    // Remove class code
    it->classCodes.erase(classIt);

    // If no more class codes, remove student
    if (it->classCodes.size() == 0) students.erase(it);
}

void StudentDirectory::replaceClass(const std::string& id,
                                    const std::string& oldClassCode,
                                    const std::string& newClassCode) {
    auto it = findStudent(id);
    auto oldClassIt = it->classCodes.find(oldClassCode);

    // Remove old class code and add new class code
    it->classCodes.erase(oldClassIt);
    it->classCodes.insert(newClassCode);
}

int StudentDirectory::removeClass(const std::string& classCode) {
    int removedCount = 0;
    // Remove class from all students
    for (auto it = students.begin(); it != students.end();) {
        auto classIt = it->classCodes.find(classCode);
        if (classIt != it->classCodes.end()) {
            it->classCodes.erase(classIt);
            removedCount++;
            // If no more class codes, remove student
            if (it->classCodes.size() == 0) {
                it = students.erase(it);
                continue;
            }
        }
        ++it;
    }
    return removedCount;
}
