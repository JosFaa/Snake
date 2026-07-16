#include "terminal.hpp"

#include <iostream>
#include <ncurses.h>
#include <string>

void chooseMap(int &rows, int &columns) {
    std::vector<std::string>options = {
        "Small",
        "Medium",
        "Large"
    };

    bool validChoice = false;

    int choice = 0;
    std::cout << "Select grid size:" << std::endl;
    for (size_t i = 0; i<options.size(); i++) {
        std::cout << i + 1 << ". " << options[i] << std::endl;
    }
    
    while (!validChoice) {
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Please enter a number." << std::endl;
            continue;
        }
        if (choice >= 1 && choice <=3) {
            switch (choice) {
                case 1: 
                    rows = 10;
                    columns = 10;
                    validChoice = true;
                    break;
                case 2: 
                    rows = 15;
                    columns = 15;
                    validChoice = true;
                    break;
                case 3: 
                    rows = 20;
                    columns = 20;
                    validChoice = true;
                    break;
            }
        } else {std::cout << "Number must be between 1 and 3" << std::endl;}
    }
}
void printBoard(const std::vector<Position>& foods, const std::vector<Position>& snake, int rows, int columns) {
    bool foodHere = false;
    bool headHere = false;
    bool bodyHere = false;

    erase();

    for (int y=0; y<rows; y++) {
        for (int x=0; x<columns; x++) {
            for (size_t i=0; i<foods.size(); i++) {
                if (foods[i].x == x and foods[i].y == y) {
                    foodHere = true;
                }
            }

            for (size_t i=0; i<snake.size(); i++) {
                if (snake[i].x == x and snake[i].y == y) {
                    if (i == 0) {
                        headHere = true;
                    } else bodyHere = true;
                }       
            }

            if (headHere) {mvaddch(y, x, '@');}
            else if (bodyHere) {mvaddch(y, x, 'o');}
            else if (foodHere) {mvaddch(y, x, '*');}
            else {mvaddch(y, x, '.');}

            foodHere = false;
            headHere = false;
            bodyHere = false;
        }
    }
    refresh();
}