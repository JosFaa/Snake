#include <game.hpp>

bool reverseDirection(char current, char requested) {
    bool reversing = (
        (current == 'w' && requested == 's') ||
        (current == 's' && requested == 'w') || 
        (current == 'd' && requested == 'a') || 
        (current == 'a' && requested == 'd')
    );
    return reversing;
}

void moveSnake(char input, std::vector<Position>& snake) {
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

Position spawnFood(const std::vector<Position>& foods, const std::vector<Position>& snake, int rows, int columns, std::mt19937& generator) {

    std::uniform_int_distribution<> rowDist(0, rows - 1); 
    std::uniform_int_distribution<> colDist(0, columns - 1);
    
    bool valid = false; 
    int randomCol; int randomRow;

    while (!valid) {
        randomCol = colDist(generator);
        randomRow = rowDist(generator);
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

int checkFoodCollision(const std::vector<Position>& foods, const std::vector<Position>& snake) {
    for (size_t i=0; i<foods.size(); i++) {
        if (foods[i].x == snake[0].x and foods[i].y == snake[0].y) {
            return i;      
        }
    }
    return -1;
}

void removeFood(std::vector<Position>& foods, int index) {
    if (index != -1) {
        foods.erase(foods.begin()+index);
    }
}

void endGame(bool& gameLoop, const std::vector<Position>& snake, int rows, int columns) {
    bool outsideArea = snake[0].y < 0 or snake[0].y >= rows or snake[0].x < 0 or snake[0].x >= columns;
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