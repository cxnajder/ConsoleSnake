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