#include <iostream>
#include <random>
#include <vector>
#include <ncurses.h>
#include <thread>
#include <chrono>

#define FOOD_COUNT 2

using std::cout, std::endl, std::cin;
using std::vector;

int row = 20;
int col = 20;

std::random_device rd; //random seed generator 
std::mt19937 gen(rd()); // random number gen

struct Position {
    int x;
    int y;
};

void chooseMap() {
    vector<std::string>options = {
        "Small",
        "Medium",
        "Large"
    };

    bool validChoice = false;

    int choice = 0;
    cout << "Select grid size:" << endl;
    for (size_t i = 0; i<options.size(); i++) {
        cout << i + 1 << ". " << options[i] << endl;
    }
    
    while (!validChoice) {
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Please enter a number." << endl;
            continue;
        }
        if (choice >= 1 && choice <=3) {
            switch (choice) {
                case 1: 
                    row = 10;
                    col = 10;
                    validChoice = true;
                    break;
                case 2: 
                    row = 15;
                    col = 15;
                    validChoice = true;
                    break;
                case 3: 
                    row = 20;
                    col = 20;
                    validChoice = true;
                    break;
            }
        } else {cout << "Number must be between 1 and 3" << endl;}
    }
}
void printBoard(const vector<Position>& foods, const vector<Position>& snake) {
    bool foodHere = false;
    bool headHere = false;
    bool bodyHere = false;

    erase();

    for (int y=0; y<row; y++) {
        for (int x=0; x<col; x++) {
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

bool reverseDirection(char current, char requested) {
    bool reversing = (
        (current == 'w' && requested == 's') ||
        (current == 's' && requested == 'w') || 
        (current == 'd' && requested == 'a') || 
        (current == 'a' && requested == 'd')
    );
    return reversing;
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

    for (size_t i = snake.size(); i > 1; --i) {
        snake[i - 1] = snake[i - 2];
    }

    snake[0].x += dx;
    snake[0].y += dy;
}

Position spawnFood(const vector<Position>& foods, const vector<Position>& snake) {

    std::uniform_int_distribution<> rowDist(0, row-1); 
    std::uniform_int_distribution<> colDist(0, col-1);
    
    bool valid = false; 
    int randomCol; int randomRow;

    while (!valid) {
        randomCol = colDist(gen);
        randomRow = rowDist(gen);
        valid = true; 

        for (size_t i=0; i<foods.size(); i++) {
            if (randomCol == foods[i].x and randomRow == foods[i].y) {
                valid = false; 
                break;
            }
        }
        for (size_t i=0; i<snake.size(); i++) {
            if (randomCol == snake[i].x and randomRow == snake[i].y) {
                valid = false; 
            }
        }
    }   

    return {randomCol, randomRow};
}

int checkFoodCollision(const vector<Position>& foods, const vector<Position>& snake) {
    for (size_t i=0; i<foods.size(); i++) {
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

    for (size_t i = 1; i<snake.size(); i++) {
        if (snake[0].x == snake[i].x and 
        snake[0].y == snake[i].y) {
            bodyCollision = true;
            break;
        }

    }
    if (outsideArea or bodyCollision) {
            gameLoop = false;
        }
}

int main() {
    chooseMap();

    initscr();      //inits ncurses
    noecho();       //don't show typed keys 
    curs_set(0);    //no cursor 
    nodelay(stdscr, true);

    vector<Position> snake;
    
    // Snake starter body
    snake.push_back({col/2, row/2});
    snake.push_back({col/2 - 1, row/2});
    snake.push_back({col/2 - 2, row/2});

    vector<Position> foods;

    bool gameLoop = true;
    int foodCollision = -1;
    size_t totalCells = static_cast<size_t>(row) * static_cast<size_t>(col);

    for (int i =0; i<FOOD_COUNT; i++) {
        foods.push_back(spawnFood(foods, snake));
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
                    cout << "You win!" << endl;
                    return 0;
                }
                    size_t occupiedCells = snake.size() + foods.size();
                    if (occupiedCells < totalCells) {
                        foods.push_back(spawnFood(foods, snake));
                    }
            }
            printBoard(foods, snake);
            lastUpdate = now;
        }
        std::this_thread::sleep_for(
            std::chrono::milliseconds(10)
        );
        endGame(gameLoop, snake);
    }
    endwin();
    cout << "Game Over!" << endl;
    return 0;
}