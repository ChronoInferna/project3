#include <iostream>

#include "Compass.hpp"

int main() {
    Compass compass("data/edges.csv", ',', "data/classes.csv", ',');

    std::string numCommands;
    int commands;
    std::getline(std::cin, numCommands);
    try {
        commands = std::stoi(numCommands);
    } catch (const std::invalid_argument& e) {
        return 1;
    }

    std::string input;
    for (int i = 0; i < commands; ++i) {
        std::getline(std::cin, input);
        std::string output = compass.parseInput(input);
        std::cout << output << std::endl;
    }
    return 0;
}
