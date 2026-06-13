#include <iostream>
#include <random>
#include <vector>

#define FOOD_COUNT 2

using std::cout, std::endl, std::cin;
using std::vector;

int row = 3;
int col = 3;

std::random_device rd; //random seed generator 
std::mt19937 gen(rd()); // random number gen
std::uniform_int_distribution<> rowDist(0, row-1); 
std::uniform_int_distribution<> colDist(0, col-1);

struct Position {
    int x;
    int y;
};

void printBoard(const vector<Position>& foods, const vector<Position>& snake) {
    bool foodHere = false;
    bool headHere = false;
    bool bodyHere = false;

    for (int y=0; y<row; y++) {
        for (int x=0; x<col; x++) {
            for (int i=0; i<foods.size(); i++) {
                if (foods[i].x == x and foods[i].y == y) {
                    foodHere = true;
                }
            }

            for (int i=0; i<snake.size(); i++) {
                if (snake[i].x == x and snake[i].y == y) {
                    if (i == 0) {
                        headHere = true;
                    } else bodyHere = true;
                }       
            }

            if (headHere) {cout << "@";}
            else if (bodyHere) {cout << "o";}
            else if (foodHere) {cout << "*";}
            else {cout << ".";}

            foodHere = false;
            headHere = false;
            bodyHere = false;
        }
        cout << endl;
    }
}

void moveSnake(char input, vector<Position>& snake) {
    int dx = 0;
    int dy = 0;
    
    switch (input) {
        case 'w':
            dy += -1;
            break;
        case 'a': 
            dx += -1;
            break;
        case 's': 
            dy += 1;
            break;
        case 'd': 
            dx += 1;
            break;
        default:
            return;
    }

    for (int i = snake.size() - 1; i > 0; --i) {
        snake[i] = snake[i - 1];
    }

    snake[0].x += dx;
    snake[0].y += dy;
}

Position spawnFood(const vector<Position>& foods, const vector<Position>& snake) {
    cout << "calling spawnfood()" << endl;
    
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
        for (int i=0; i<snake.size(); i++) {
            if (randomCol == snake[i].x and randomRow == snake[i].y) {
                valid = false; 
            }
        }
    }   

    return {randomCol, randomRow};
}

int checkFoodCollision(const vector<Position>& foods, const vector<Position>& snake) {
    for (int i=0; i<foods.size(); i++) {
        if (foods[i].x == snake[0].x and foods[i].y == snake[0].y) {
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

void endGame(bool& gameLoop, const vector<Position>& snake) {
    bool outsideArea = snake[0].y < 0 or snake[0].y >= row or snake[0].x < 0 or snake[0].x >= col;
    bool bodyCollision = false; 

    for (int i = 1; i<snake.size(); i++) {
        if (snake[0].x == snake[i].x and 
        snake[0].y == snake[i].y) {
            bodyCollision = true;
            break;
        }

    }
    if (outsideArea or bodyCollision) {
            gameLoop = false;
            cout << "Game Over!" << endl;
        }
}

int main() {
    vector<Position> snake;
    
    snake.push_back({col/2, row/2});
    snake.push_back({col/2 - 1, row/2});
    snake.push_back({col/2 - 2, row/2});


    vector<Position> foods;

    bool gameLoop = true;
    char input;
    int foodCollision = -1;
    int totalCells = row * col;

    for (int i =0; i<FOOD_COUNT; i++) {
        foods.push_back(spawnFood(foods, snake));
    }

    while (gameLoop) {
        printBoard(foods, snake);

        Position oldTail = snake.back();
        cin >> input;
        moveSnake(input, snake);

        foodCollision = checkFoodCollision(foods, snake);
        if (foodCollision != -1) {
            snake.push_back(oldTail);
            removeFood(foods, foodCollision);
            if (snake.size() >= totalCells) {
                cout << "You win!" << endl;
                return 0;
            }
                int occupiedCells = snake.size() + foods.size();
                if (occupiedCells < totalCells) {
                    foods.push_back(spawnFood(foods, snake));
                }
        }
        
        endGame(gameLoop, snake);
    }
    return 0;
}