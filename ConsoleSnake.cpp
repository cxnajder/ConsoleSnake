﻿#include <cstdlib>
#include <iostream>
#include <vector>

#if defined(_WIN64) || defined(_WIN32)
#include <conio.h>
#include <windows.h>
#elif defined(__linux__)
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Function to get a character without waiting for Enter
char _getch() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// Function to check if a key has been pressed
int _kbhit() {
    struct termios oldt, newt;
    int oldf;
    fd_set readfds;
    struct timeval tv;

    // Save terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Set non-blocking mode
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    // Initialize the file descriptor set
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    // Initialize the timeout data structure
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    // Check if there's input available
    int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    return result > 0;
}

// Function to check if a key has been pressed
int _kbhit_2() {
    struct termios oldt, newt;
    char ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif


class SnkaeGame {
public:
    enum GameMode {IMMORTAL = 0, EASY, MEDIUM, HARD };

    SnkaeGame(GameMode mode)
        : gameMode(mode)
    {
        Setup();
        while (!gameOver)
        {
            Input();
            Logic();
            Draw();
            SleepForXMs(_REFRESH_RATE_MS);
        }
    }

private:
    GameMode gameMode;
    const int _MAP_WIDTH = 15;
    const int _MAP_HEIGHT = 15;
    int _REFRESH_RATE_MS = 100;
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
    Segment frog;
    enum MovementDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
    MovementDirection snakeDirection;
    int score;
    bool snakeHasEaten;

    void Setup() {
        gameOver = false;
        snake.head.x = _MAP_WIDTH / 2;
        snake.head.y = _MAP_HEIGHT / 2;
        snakeDirection = STOP;
        frog.x = std::rand() % _MAP_WIDTH;
        frog.y = std::rand() % _MAP_HEIGHT;
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
                else if (j == frog.x && i == frog.y)
                    std::cout << "F";
                else if (snake.tail.empty())
                        std::cout << " ";
                else
                {
                    bool printed = false;
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
            switch (_getch())
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
        if (snake.head.x == frog.x && snake.head.y == frog.y)
        {
            ++score;
            snakeHasEaten = true;

            frog.x = std::rand() % _MAP_WIDTH;
            frog.y = std::rand() % _MAP_HEIGHT;

            if (gameMode == HARD)
            {
                if (_REFRESH_RATE_MS > 10)
                    _REFRESH_RATE_MS -= 10;
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
    SnkaeGame game = { SnkaeGame::GameMode::EASY };
}
