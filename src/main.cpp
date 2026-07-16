#include <chrono>
#include <iostream>
#include <ncurses.h>
#include <random>
#include <thread>
#include <vector>

#include "game.hpp"
#include "terminal.hpp"
#include "types.hpp"

#define FOOD_COUNT 2


int main() {
    int row = 20;
    int col = 20;

    std::random_device rd; //random seed generator 
    std::mt19937 gen(rd()); // random number gen


    chooseMap(row, col);

    initscr();      //inits ncurses
    noecho();       //don't show typed keys 
    curs_set(0);    //no cursor 
    nodelay(stdscr, true);

    std::vector<Position> snake;
    
    // Snake starter body
    snake.push_back({col/2, row/2});
    snake.push_back({col/2 - 1, row/2});
    snake.push_back({col/2 - 2, row/2});

    std::vector<Position> foods;

    bool gameLoop = true;
    int foodCollision = -1;
    size_t totalCells = static_cast<size_t>(row) * static_cast<size_t>(col);

    for (int i =0; i<FOOD_COUNT; i++) {
        foods.push_back(spawnFood(foods, snake, row, col, gen));
    }

    char currentDirection = 'd';
    char nextDirection = 'd';

    auto lastUpdate = std::chrono::steady_clock::now();

    while (gameLoop) {
        Position oldTail = snake.back();
        int requestedDirection = getch();
        bool validKey = (requestedDirection == 'w' || 
            requestedDirection == 'a' || 
            requestedDirection == 's' || 
            requestedDirection == 'd');

        if (requestedDirection != ERR &&!reverseDirection(currentDirection, requestedDirection)) {
                if (validKey) {nextDirection = requestedDirection;}
            }
        
        auto now = std::chrono::steady_clock::now();

        if (now - lastUpdate >= std::chrono::milliseconds(250)) {
            currentDirection = nextDirection;
            moveSnake(currentDirection, snake);
    
            foodCollision = checkFoodCollision(foods, snake);
            if (foodCollision != -1) {
                snake.push_back(oldTail);
                removeFood(foods, foodCollision);
                if (snake.size() >= totalCells) {
                    endwin();
                    std::cout << "You win!" << std::endl;
                    return 0;
                }
                    size_t occupiedCells = snake.size() + foods.size();
                    if (occupiedCells < totalCells) {
                        foods.push_back(spawnFood(foods, snake, row, col, gen));
                    }
            }
            printBoard(foods, snake, row, col);
            lastUpdate = now;
        }
        std::this_thread::sleep_for(
            std::chrono::milliseconds(10)
        );
        endGame(gameLoop, snake, row, col);
    }
    endwin();
    std::cout << "Game Over!" << std::endl;
    return 0;
}