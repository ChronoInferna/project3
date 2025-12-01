#pragma once

#include "ClassDirectory.hpp"
#include "Graph.hpp"
#include "StudentDirectory.hpp"

class Compass {
   private:
    Graph graph;
    StudentDirectory studentDirectory;
    ClassDirectory classDirectory;

   public:
    Compass(std::string edgeCSVFile, char edgeDelimiter,
            std::string classCSVFile, char classDelimiter);
    ~Compass() = default;

    std::string parseInput(const std::string& input);

    // All required functions - void return types are for functions where the
    // input is guaranteed to be valid
    bool insert(const std::string& name, const std::string& id,
                const std::string& residenceLocationId, const std::string& n,
                const std::vector<std::string>& classCodes);
    bool remove(const std::string& id);
    bool dropClass(const std::string& id, const std::string& classCode);
    bool replaceClass(const std::string& id, const std::string& oldClassCode,
                      const std::string& newClassCode);
    int removeClass(const std::string& classCode);
    void toggleEdgesClosure(const std::string& n,
                            const std::vector<std::string>& locationIds);
    std::string checkEdgeStatus(const std::string& locationId1,
                                const std::string& locationId2);
    bool isConnected(const std::string& locationId1,
                     const std::string& locationId2);
    std::string printShortestEdges(const std::string& id);
    std::string printStudentZone(const std::string& id);
    // TODO extra credit
    // void verifySchedule(const std::string& id);
};
