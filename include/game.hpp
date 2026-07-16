#pragma once

#include <vector>
#include <random>
#include "types.hpp"

bool reverseDirection(char current, char requested);

void moveSnake(
    char input, 
    std::vector<Position>& snake
);

Position spawnFood(
    const std::vector<Position>& foods, 
    const std::vector<Position>& snake, 
    int rows, 
    int columns, 
    std::mt19937& generator
);

int checkFoodCollision(
    const std::vector<Position>& foods, 
    const std::vector<Position>& snake
);

void removeFood(
    std::vector<Position>& foods, 
    int index
);

void endGame(
    bool& gameLoop, 
    const std::vector<Position>& snake,
    int rows,
    int columns
);