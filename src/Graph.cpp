#include "Graph.hpp"

#include <algorithm>
#include <limits>
#include <queue>
#include <stdexcept>
#include <unordered_set>

#include "CSVReader.hpp"

Graph::Graph(const std::string& filename, char delimiter) {
    fromCSV(filename, delimiter);
};

bool Graph::fromCSV(const std::string& filename, char delimiter) {
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
        std::string from = row[0];
        std::string to = row[1];
        std::string fromName = row[2];
        std::string toName = row[3];
        std::string time = row[4];

        addNode(from, fromName);
        addNode(to, toName);
        addEdge(from, to, time);
        addEdge(to, from, time);  // Undirected graph
    }
    return true;
}

void Graph::addNode(const std::string& node, const std::string& name) {
    if (adjList.find(node) != adjList.end()) return;
    adjList[node] = std::vector<std::tuple<std::string, std::string, bool>>{};
    names[name] = node;
    return;
}

void Graph::addEdge(const std::string& from, const std::string& to,
                    const std::string& time) {
    if (adjList.find(from) == adjList.end() ||
        adjList.find(to) == adjList.end())
        return;

    adjList[from].emplace_back(to, time, true);  // Edge is initially "on"
    return;
}

bool Graph::hasNode(const std::string& node) const {
    return adjList.find(node) != adjList.end();
}

bool Graph::hasEdge(const std::string& from, const std::string& to) const {
    if (adjList.find(from) == adjList.end()) return false;

    for (const auto& neighbor : adjList.at(from)) {
        if (std::get<0>(neighbor) == to) {
            return true;
        }
    }
    return false;
}

void Graph::toggleEdge(const std::string& from, const std::string& to) {
    for (auto& neighbor : adjList[from]) {
        if (std::get<0>(neighbor) == to) {
            std::get<2>(neighbor) = !std::get<2>(neighbor);
            break;
        }
    }
    for (auto& neighbor : adjList[to]) {
        if (std::get<0>(neighbor) == from) {
            std::get<2>(neighbor) = !std::get<2>(neighbor);
            break;
        }
    }
}

bool Graph::isConnected(const std::string& locationId1,
                        const std::string& locationId2) {
    // BFS to check connectivity
    std::unordered_set<std::string> visited;
    std::queue<std::string> q;
    q.push(locationId1);
    visited.insert(locationId1);

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        if (current == locationId2) return true;

        for (const auto& neighbor : adjList[current]) {
            std::string neighborId = std::get<0>(neighbor);
            bool isOpen = std::get<2>(neighbor);
            if (visited.find(neighborId) == visited.end() && isOpen) {
                visited.insert(neighborId);
                q.push(neighborId);
            }
        }
    }
    return false;
}

std::string Graph::shortestPath(
    const std::string& start,
    const std::vector<std::pair<std::string, std::string>>& classes) {
    // Dijkstra's algorithm to find shortest path to all class locations
    std::unordered_map<std::string, int> distances;

    for (const auto& node : adjList)
        distances[node.first] = std::numeric_limits<int>::max();
    distances[start] = 0;

    using PQElement = std::pair<int, std::string>;  // (distance, node)
    std::priority_queue<PQElement, std::vector<PQElement>,
                        std::greater<PQElement>>
        pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [currentDistance, currentNode] = pq.top();
        pq.pop();

        if (currentDistance > distances[currentNode]) continue;

        for (const auto& neighbor : adjList[currentNode]) {
            std::string neighborId = std::get<0>(neighbor);
            int weight = std::stoi(std::get<1>(neighbor));
            bool isOpen = std::get<2>(neighbor);
            if (isOpen) {
                int newDistance = currentDistance + weight;
                if (newDistance < distances[neighborId]) {
                    distances[neighborId] = newDistance;
                    pq.push({newDistance, neighborId});
                }
            }
        }
    }

    // Sort classes by class code lexicographically
    std::vector<std::pair<std::string, std::string>> sortedClasses = classes;
    std::sort(sortedClasses.begin(), sortedClasses.end(),
              [](const auto& a, const auto& b) { return a.first < b.first; });
    // Build result string for each class location
    std::string result;
    for (const auto& [classCode, locationId] : sortedClasses) {
        if (distances.find(locationId) == distances.end() ||
            distances[locationId] == std::numeric_limits<int>::max()) {
            distances[locationId] = -1;
        }
        result += classCode +
                  " | Total Time: " + std::to_string(distances[locationId]) +
                  "\n";
    }

    return result;
}

std::string Graph::studentZone(
    const std::string& start,
    const std::vector<std::pair<std::string, std::string>>& classes) {
    // We are guaranteed that all classCodes are reachable from start

    // First, we find the shortest path from start to all nodes using Dijkstra's
    std::unordered_map<std::string, int> distances;
    std::unordered_map<std::string, std::string> previous;

    for (const auto& node : adjList)
        distances[node.first] = std::numeric_limits<int>::max();
    distances[start] = 0;

    using PQElement = std::pair<int, std::string>;  // (distance, node)
    std::priority_queue<PQElement, std::vector<PQElement>,
                        std::greater<PQElement>>
        pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [currentDistance, currentNode] = pq.top();
        pq.pop();

        if (currentDistance > distances[currentNode]) continue;

        for (const auto& neighbor : adjList[currentNode]) {
            std::string neighborId = std::get<0>(neighbor);
            int weight = std::stoi(std::get<1>(neighbor));
            bool isOpen = std::get<2>(neighbor);
            if (isOpen) {
                int newDistance = currentDistance + weight;
                if (newDistance < distances[neighborId]) {
                    distances[neighborId] = newDistance;
                    previous[neighborId] = currentNode;
                    pq.push({newDistance, neighborId});
                }
            }
        }
    }

    // Build set of nodes in the student zone
    std::unordered_set<std::string> zoneNodes;
    zoneNodes.insert(start);
    for (const auto& [classCode, locationId] : classes) {
        // Backtrack from classCode to start to find all nodes in path
        std::string currentNode = locationId;
        while (currentNode != start) {
            if (!previous.count(currentNode)) break;  // No path found
            zoneNodes.insert(currentNode);
            // Find the previous node in the path
            currentNode = previous.at(currentNode);
        }
    }

    // Create subgraph of zone nodes
    Graph subgraph;
    for (const auto& node : zoneNodes) subgraph.addNode(node, names[node]);
    // If both nodes are in zoneNodes and original edge is open, add edge to
    // subgraph
    for (const auto& node : zoneNodes) {
        for (const auto& neighbor : adjList[node]) {
            std::string neighborId = std::get<0>(neighbor);
            std::string time = std::get<1>(neighbor);
            bool isOpen = std::get<2>(neighbor);
            if (zoneNodes.find(neighborId) != zoneNodes.end() && isOpen) {
                subgraph.addEdge(node, neighborId, time);
            }
        }
    }

    // Find the MST of the subgraph using Prim's algorithm, keeping track of the
    // cost of the MST
    std::unordered_set<std::string> mstSet;
    int resultCost = 0;
    std::string currentNode = start;
    mstSet.insert(currentNode);
    while (mstSet.size() < zoneNodes.size()) {
        int minWeight = std::numeric_limits<int>::max();
        std::string nextNode;
        for (const auto& node : mstSet) {
            for (const auto& neighbor : subgraph.adjList[node]) {
                std::string neighborId = std::get<0>(neighbor);
                int weight = std::stoi(std::get<1>(neighbor));
                if (!mstSet.count(neighborId) && weight < minWeight) {
                    minWeight = weight;
                    nextNode = neighborId;
                }
            }
        }
        if (minWeight == std::numeric_limits<int>::max())
            break;  // defensive against impossible cases
        resultCost += minWeight;
        mstSet.insert(nextNode);
    }

    return std::to_string(resultCost);
}
