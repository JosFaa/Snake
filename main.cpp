#include <iostream>
#include <random>
#include <vector>

#define FOOD_COUNT 2

using std::cout, std::endl, std::cin;
using std::vector;

int row = 10;
int col = 20;
int snakeHeadRow = row/2; // 2
int snakeHeadCol = col/2; // 5 

std::random_device rd; //random seed generator 
std::mt19937 gen(rd()); // random number gen
std::uniform_int_distribution<> rowDist(0, row-1); 
std::uniform_int_distribution<> colDist(0, col-1);

struct Position {
    int x;
    int y;
};

void printBoard(const vector<Position>& foods) {
    bool foodHere = false;

    for (int y=0; y<row; y++) {
        for (int x=0; x<col; x++) {
            for (int i=0; i<foods.size(); i++) {
                if (foods[i].x == x and foods[i].y == y) {
                    foodHere = true;
                }
            }

            if (x == snakeHeadCol and y == snakeHeadRow) {cout << "@";}
            else if (foodHere) {cout << "*";}
            else {cout << ".";}

            foodHere = false;
        }
        cout << endl;
    }
}

void moveSnake(char input) {
    if (input == 'w') {
        snakeHeadRow += -1;
    } else if (input == 'a') {
        snakeHeadCol += -1;
    } else if (input == 's') {
        snakeHeadRow += 1;
    } else if (input == 'd') {
        snakeHeadCol += 1;
    }
}

Position spawnFood(const vector<Position>& foods) {
    bool valid = false; 
    int randomCol; int randomRow;

    while (!valid) {
        randomCol = colDist(gen);
        randomRow = rowDist(gen);
        valid = true; 

        for (int i=0; i<foods.size(); i++) {
            if (randomCol == foods[i].x and randomRow == foods[i].y) {
                valid = false; 
                break;
            }
        }
        if (randomCol == snakeHeadCol and randomRow == snakeHeadRow) {
            valid = false; 
        }
    }   

    return {randomCol, randomRow};
}

int checkFoodCollision(const vector<Position>& foods) {
    for (int i=0; i<foods.size(); i++) {
        if (foods[i].x == snakeHeadCol and foods[i].y == snakeHeadRow) {
            return i;
        }
    }
    return -1;
}

void removeFood(vector<Position>& foods, int index) {
    if (index != -1) {
        foods.erase(foods.begin()+index);
    }
}

void endGame(bool& gameLoop) {
    if (snakeHeadRow < 0 or snakeHeadRow >= row or snakeHeadCol < 0 or snakeHeadCol >= col) {
            gameLoop = false;
            cout << "Game Over!" << endl;
        }
}

int main() {
    cout << "x="<< snakeHeadCol << ", " << "y=" << snakeHeadRow << endl;

    bool gameLoop = true;
    char input;
    int foodCollision = -1;

    vector<Position> foods;
    for (int i =0; i<FOOD_COUNT; i++) {
        foods.push_back(spawnFood(foods));
    }

    while (gameLoop) {
        for (int i=0; i<foods.size(); i++) {
            cout << "food" << i << " at (" << foods[i].x << ", " << foods[i].y << ")" << endl;
        }
        printBoard(foods);
        cin >> input;
        moveSnake(input);
        foodCollision = checkFoodCollision(foods);
        if (foodCollision != -1) {
            removeFood(foods, foodCollision);
            foods.push_back(spawnFood(foods));
        }
        
        endGame(gameLoop);
    }
    return 0;
}