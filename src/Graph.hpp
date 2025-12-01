#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct Graph {
    // Adjacency list representation: node -> list of (neighbor, weight, on/off)
    std::unordered_map<std::string,
                       std::vector<std::tuple<std::string, std::string, bool>>>
        adjList;
    // Maps name to node ID
    std::unordered_map<std::string, std::string> names;

    Graph() = default;
    Graph(const std::string& filename, char delimiter);
    ~Graph() = default;

    bool fromCSV(const std::string& filename, char delimiter = ',');
    void addNode(const std::string& node, const std::string& name);
    void addEdge(const std::string& from, const std::string& to,
                 const std::string& time);
    bool hasNode(const std::string& node) const;
    bool hasEdge(const std::string& from, const std::string& to) const;
    void toggleEdge(const std::string& from, const std::string& to);
    bool isConnected(const std::string& locationId1,
                     const std::string& locationId2);
    std::string shortestPath(
        const std::string& start,
        const std::vector<std::pair<std::string, std::string>>& classCodes);
    std::string studentZone(
        const std::string& start,
        const std::vector<std::pair<std::string, std::string>>& classCodes);
};
