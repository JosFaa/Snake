#pragma once

#include <vector>
#include "types.hpp"

void chooseMap(
    int &rows, 
    int &columns
);

void printBoard(
    const std::vector<Position>& foods, 
    const std::vector<Position>& snake, 
    int rows, 
    int columns
);