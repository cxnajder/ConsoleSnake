#include <cstdlib>
#include <iostream>
#include <vector>

#if defined(_WIN64) || defined(_WIN32)
#include <conio.h>
#include <windows.h>
#elif defined(__linux__)
#include "ConioForLinux/ConioForLinux.h"
#endif


class SnakeGame {
public:
    enum GameMode {IMMORTAL = 0, EASY, MEDIUM, HARD };

    SnakeGame(GameMode mode)
        : gameMode(mode)
    {
        Setup();
        while (!gameOver)
        {
            Input();
            Logic();
            Draw();
            SleepForXMs(currentRefreshRateMs);
        }
    }

    SnakeGame()
        : SnakeGame(GameMode::MEDIUM)
    {}

private:
    GameMode gameMode;
    const int _MAP_WIDTH = 15;
    const int _MAP_HEIGHT = 15;
    const int _AMOUNT_OF_FROGS_SPAWNING = 4;
    const char _SPRINT_KEY = '.';
    const int START_REFRESH_RATE_MS = 100;

    int currentRefreshRateMs = 100;
    bool gameOver;
    struct Segment
    {
        int x;
        int y;
    };
    struct Snake
    {
        Segment head;
        std::vector<Segment> tail;
    };
    Snake snake;
    std::vector<Segment> frogs;
    enum MovementDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
    MovementDirection snakeDirection;
    int score;
    bool snakeHasEaten;

    void Setup() {
        gameOver = false;
        snake.head.x = _MAP_WIDTH / 2;
        snake.head.y = _MAP_HEIGHT / 2;
        snakeDirection = STOP;
        for(int i = 0; i < _AMOUNT_OF_FROGS_SPAWNING; ++i)
        {
            frogs.push_back( { 0, 0 } ); // Known issue: Frogs can stock on each other.
        }
        for (auto& frog: frogs)
        {
            frog.x = std::rand() % _MAP_WIDTH; 
            frog.y = std::rand() % _MAP_HEIGHT;
        }
        score = 0;
        snakeHasEaten = false;
    }

    void ClearConsole() {
    #if defined(_WIN64) || defined(_WIN32)
        system("cls");
    #elif defined(__linux__)
        system("clear");
    #endif
    }

    void Draw() {

        ClearConsole();

        for (int i = 0; i < _MAP_WIDTH + 2; ++i)
            std::cout << "W";
        std::cout << "\n";

        for (int i = 0; i < _MAP_HEIGHT; ++i)
        {
            std::cout << "W";
            for (int j = 0; j < _MAP_WIDTH; ++j)
            {
                if (j == snake.head.x && i == snake.head.y)
                    std::cout << "S";
                else
                {
                    bool printed = false;

                    for (const auto& frog: frogs)
                        if (j == frog.x && i == frog.y)
                        {
                            std::cout << "F";
                            printed = true;
                            break;
                        }

                    if(!printed)
                        for (const auto& s : snake.tail)
                            if (j == s.x && i == s.y)
                            {
                                std::cout << "s";
                                printed = true;
                                break;
                            }
                    
                    if(!printed)
                        std::cout << " ";
                }
            }
            std::cout << "W";
            std::cout << "\n";
        }


        for (int i = 0; i < _MAP_WIDTH + 2; ++i)
            std::cout << "W";
        std::cout << "\n";

        std::cout << "Score:" << score;
        std::cout << "\n";
    }

    void Input() {
        if (_kbhit())
        {
            const char key = _getch();
            if (key == _SPRINT_KEY)
                currentRefreshRateMs = START_REFRESH_RATE_MS / 2;
            else
                currentRefreshRateMs = START_REFRESH_RATE_MS;

            switch (key)
            {
            case 'a': 
            case 'A':
                if (snakeDirection != RIGHT)
                    snakeDirection = LEFT;
                break;
            case 'w': 
            case 'W':
                if (snakeDirection != DOWN)
                    snakeDirection = UP;
                break;
            case 's':
            case 'S':
                if (snakeDirection != UP)
                    snakeDirection = DOWN;
                break;
            case 'd':
            case 'D':
                if (snakeDirection != LEFT)
                    snakeDirection = RIGHT;
                break;
            case 'x':
            case 'X':
                gameOver = true;
                break;
            }
        }
    }

    void Logic() {

        // Elongate snake after he has eaten in prev loop
        if (snakeHasEaten)
        {
            snake.tail.push_back( { 0, 0 } );
            snakeHasEaten = false;
        }

        // Make the tail fallow the head
        {
            Segment prev1 = snake.head;
            Segment prev2;

            for (int i = 0; i < snake.tail.size(); ++i)
            {
                prev2 = prev1;
                prev1 = snake.tail[i];
                snake.tail[i] = prev2;
            }
        }

        // Change snake's movement
        switch (snakeDirection)
        {
        case LEFT:
            --snake.head.x;
            break;
        case UP:
            --snake.head.y;
            break;
        case DOWN:
            ++snake.head.y;
            break;
        case RIGHT:
            ++snake.head.x;
            break;
        }

        // On eat
        for(auto& frog: frogs)
            if (snake.head.x == frog.x && snake.head.y == frog.y)
            {
                ++score;
                snakeHasEaten = true;

                frog.x = std::rand() % _MAP_WIDTH;
                frog.y = std::rand() % _MAP_HEIGHT;

                if (gameMode == HARD)
                {
                    if (currentRefreshRateMs > 10)
                        currentRefreshRateMs -= 10;
                }
            }

        // Handle wall colisions
        if (gameMode <= EASY)
        {
            // If easy mode teleport throught walls
            if (snake.head.x < 0)
                snake.head.x = _MAP_WIDTH - 1;

            if (snake.head.y < 0)
                snake.head.y = _MAP_HEIGHT - 1;

            if (snake.head.x >= _MAP_WIDTH)
                snake.head.x = 0;

            if (snake.head.y >= _MAP_HEIGHT)
                snake.head.y = 0;
        }
        else
        {
            // Game over on wall colision
            if (snake.head.x < 0 || snake.head.x >= _MAP_WIDTH || snake.head.y < 0 || snake.head.y >= _MAP_HEIGHT)
            {
                gameOver = true;
            }
        }

        if (gameMode != IMMORTAL)
        {
            // Game over on tail colision
            for (const auto& s : snake.tail)
            {
                if (s.x == snake.head.x && s.y == snake.head.y)
                    gameOver = true;
            }
        }

    }

    void SleepForXMs(int ms) {
    #if defined(_WIN64) || defined(_WIN32)
        Sleep(ms);
    #elif defined(__linux__)
        usleep(ms * 1000);
    #endif
    }
};

int main() {
    SnakeGame game;
}
