#include "Compass.hpp"

#include <iomanip>
#include <sstream>

Compass::Compass(std::string edgeCSVFile, char edgeDelimiter,
                 std::string classCSVFile, char classDelimiter)
    : graph(edgeCSVFile, edgeDelimiter),
      studentDirectory(),
      classDirectory(classCSVFile, classDelimiter) {}

std::string Compass::parseInput(const std::string& input) {
    std::istringstream iss(input);
    std::string command;
    iss >> command;

    bool success = false;

    if (command == "insert") {
        std::string arg1, arg2, arg3, arg4;
        iss >> std::quoted(arg1) >> arg2 >> arg3 >> arg4;
        if (iss.fail()) return "unsuccessful";

        int N;
        try {
            N = std::stoi(arg4);
        } catch (const std::invalid_argument& e) {
            return "unsuccessful";
        }

        // Student can only have between 1-6 classes
        if (N < 1 || N > 6) return "unsuccessful";

        // Push back N variadic arguments
        std::vector<std::string> varArgs;
        std::string varArg;
        for (int i = 0; i < N; ++i) {
            iss >> varArg;
            if (iss.fail()) return "unsuccessful";
            varArgs.push_back(varArg);
        }

        // Run command
        success = insert(arg1, arg2, arg3, arg4, varArgs);
    } else if (command == "remove") {
        std::string arg1;
        iss >> arg1;
        if (iss.fail()) return "unsuccessful";

        // Run command
        success = remove(arg1);
    } else if (command == "dropClass") {
        std::string arg1, arg2;
        iss >> arg1 >> arg2;
        if (iss.fail()) return "unsuccessful";

        // Run command
        success = dropClass(arg1, arg2);
    } else if (command == "replaceClass") {
        std::string arg1, arg2, arg3;
        iss >> arg1 >> arg2 >> arg3;
        if (iss.fail()) return "unsuccessful";

        // Run command
        success = replaceClass(arg1, arg2, arg3);
    } else if (command == "removeClass") {
        std::string arg1;
        iss >> arg1;
        if (iss.fail()) return "unsuccessful";

        // Run command
        int result = removeClass(arg1);
        return std::to_string(result);
    } else if (command == "toggleEdgesClosure") {
        std::string arg1;
        iss >> arg1;
        if (iss.fail()) return "unsuccessful";

        int N;
        try {
            N = std::stoi(arg1);
        } catch (const std::invalid_argument& e) {
            return "unsuccessful";
        }
        N *= 2;  // Each edge has two location IDs

        // Push back N variadic arguments
        std::vector<std::string> varArgs;
        std::string varArg;
        for (int i = 0; i < N; ++i) {
            iss >> varArg;
            if (iss.fail()) return "unsuccessful";
            varArgs.push_back(varArg);
        }
        // Must be even number of location IDs
        if (varArgs.size() % 2 != 0) return "unsuccessful";

        // Run command
        toggleEdgesClosure(arg1, varArgs);
        success = true;
    } else if (command == "checkEdgeStatus") {
        std::string arg1, arg2;
        iss >> arg1 >> arg2;
        if (iss.fail()) return "unsuccessful";

        // Run command
        return checkEdgeStatus(arg1, arg2);
    } else if (command == "isConnected") {
        std::string arg1, arg2;
        iss >> arg1 >> arg2;
        if (iss.fail()) return "unsuccessful";

        // Run command
        success = isConnected(arg1, arg2);
    } else if (command == "printShortestEdges") {
        std::string arg1;
        iss >> arg1;
        if (iss.fail()) return "unsuccessful";

        // Run command
        return printShortestEdges(arg1);
    } else if (command == "printStudentZone") {
        std::string arg1;
        iss >> arg1;
        if (iss.fail()) return "unsuccessful";

        // Run command
        return printStudentZone(arg1);
    } else {
        return "unsuccessful";  // Unknown command
    }

    // Check for remaining input
    std::string remaining;
    if (iss >> remaining) success = false;

    return success ? "successful" : "unsuccessful";
}

bool Compass::insert(const std::string& name, const std::string& id,
                     const std::string& residenceLocationId,
                     const std::string& n,
                     const std::vector<std::string>& classCodes) {
    // Validate number of class codes
    // Okay cast since we know we don't have too many classes
    if (static_cast<int>(classCodes.size()) != std::stoi(n)) return false;

    // Validate student ID
    if (!Student::validateId(id)) return false;
    // Validate student name - spaces allowed since parser handles quotes
    if (!Student::validateName(name)) return false;
    // Validate student does not already exist
    auto studentIt = studentDirectory.findStudent(id);
    if (studentIt != studentDirectory.students.end()) return false;
    // Validate residence location ID exists
    if (!graph.hasNode(residenceLocationId)) return false;

    // Validate class code exists (we do not need to validate the format since
    // it is given by the csv
    for (const auto& classCode : classCodes) {
        // Validate class code exists
        auto classIt = classDirectory.findClassCode(classCode);
        if (classIt == classDirectory.classCodes.end()) return false;
    }

    studentDirectory.insert(name, id, residenceLocationId,
                            classCodes);  // N is not required since classCodes
                                          // size is already validated
    return true;
}

bool Compass::remove(const std::string& id) {
    // Validate student exists
    auto studentIt = studentDirectory.findStudent(id);
    if (studentIt == studentDirectory.students.end()) return false;

    // Remove student
    studentDirectory.removeStudent(id);
    return true;
}

bool Compass::dropClass(const std::string& id, const std::string& classCode) {
    // Validate class exists
    auto classIt = classDirectory.findClassCode(classCode);
    if (classIt == classDirectory.classCodes.end()) return false;

    // Validate student exists
    auto studentIt = studentDirectory.findStudent(id);
    if (studentIt == studentDirectory.students.end()) return false;

    // Validate class exists for student
    auto studentClassIt = studentIt->classCodes.find(classCode);
    if (studentClassIt == studentIt->classCodes.end()) return false;

    // Drop class for student
    studentDirectory.dropClass(id, classCode);
    return true;
}

bool Compass::replaceClass(const std::string& id,
                           const std::string& oldClassCode,
                           const std::string& newClassCode) {
    // Validate new class exists
    auto newClassIt = classDirectory.findClassCode(newClassCode);
    if (newClassIt == classDirectory.classCodes.end()) return false;

    // Validate student exists
    auto studentIt = studentDirectory.findStudent(id);
    if (studentIt == studentDirectory.students.end()) return false;

    // Validate old class exists for student
    auto oldClassIt = studentIt->classCodes.find(oldClassCode);
    if (oldClassIt == studentIt->classCodes.end()) return false;

    // Replace class for student
    studentDirectory.replaceClass(id, oldClassCode, newClassCode);
    return true;
}

int Compass::removeClass(const std::string& classCode) {
    // Validate class exists
    auto classIt = classDirectory.findClassCode(classCode);
    if (classIt == classDirectory.classCodes.end()) return false;

    int result = studentDirectory.removeClass(classCode);
    return result;
}

void Compass::toggleEdgesClosure(const std::string& n,
                                 const std::vector<std::string>& locationIds) {
    // Validation not required since it is guaranteed to be valid
    std::pair<std::string, std::string> edge;
    for (size_t i = 0; i < locationIds.size() - 1; i += 2)
        graph.toggleEdge(locationIds[i], locationIds[i + 1]);
}

std::string Compass::checkEdgeStatus(const std::string& locationId1,
                                     const std::string& locationId2) {
    // Validate IDs exist
    if (!graph.hasNode(locationId1) || !graph.hasNode(locationId2))
        return "DNE";
    // Validate edge exists
    if (!graph.hasEdge(locationId1, locationId2)) return "DNE";
    // Return edge status
    for (const auto& neighbor : graph.adjList[locationId1])
        if (std::get<0>(neighbor) == locationId2)
            return std::get<2>(neighbor) ? "open" : "closed";

    return "DNE";  // Should not reach here
}

bool Compass::isConnected(const std::string& locationId1,
                          const std::string& locationId2) {
    // Validate IDs exist
    if (!graph.hasNode(locationId1) || !graph.hasNode(locationId2))
        return false;

    return graph.isConnected(locationId1, locationId2);
}

std::string Compass::printShortestEdges(const std::string& id) {
    // Student ID will be valid
    auto it = studentDirectory.findStudent(id);
    auto residenceLocationId = it->residenceLocationId;
    auto classCodes = it->classCodes;
    std::vector<std::string> classCodesVector(classCodes.begin(),
                                              classCodes.end());
    // Now we need to convert to vector of pairs (classCode, locationId)
    std::vector<std::pair<std::string, std::string>> sortedCodePairs;
    for (const auto& classCode : classCodesVector) {
        std::string locationId =
            std::get<0>(classDirectory.classCodes[classCode]);
        sortedCodePairs.emplace_back(classCode, locationId);
    }

    std::string result;
    result = "Name: " + it->name + "\n" +
             graph.shortestPath(residenceLocationId, sortedCodePairs);
    return result;
}

std::string Compass::printStudentZone(const std::string& id) {
    // Student ID will be valid
    // Subgraph will also be given as valid

    auto it = studentDirectory.findStudent(id);
    auto residenceLocationId = it->residenceLocationId;
    auto classCodes = it->classCodes;
    std::vector<std::string> classCodesVector(classCodes.begin(),
                                              classCodes.end());
    // Now we need to convert to vector of pairs (classCode, locationId)
    std::vector<std::pair<std::string, std::string>> sortedCodePairs;
    for (const auto& classCode : classCodesVector) {
        std::string locationId =
            std::get<0>(classDirectory.classCodes[classCode]);
        sortedCodePairs.emplace_back(classCode, locationId);
    }

    std::string result;
    result = "Student Zone Cost For " + it->name + ": " +
             graph.studentZone(residenceLocationId, sortedCodePairs);
    return result;
}
