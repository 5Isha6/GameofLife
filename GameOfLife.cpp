#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>

// Move OS defines up here to be used in different places
#if defined(_WIN32) || defined(WIN32) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN
// WINDOWS COLORS
#define RED_COLOR SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED)
#define WARNING_COLOR SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN)
#define GREEN_COLOR SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN)

#define BLUE_COLOR SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE)

#define RESET_COLOR SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15)

#elif defined(linux) || defined(__CYGWIN__)
#define OS_LINUX

#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define BLUE_COLOR "\033[34m"
#define RESET_COLOR "\033[0m"

#elif (defined(__APPLE__) || defined(__OSX__) || defined(__MACOS__)) && defined(__MACH__) // To ensure that we are running on a mondern version of macOS
#define OS_MAC

#define RED_COLOR "\033[31m"
#define GREEN_COLOR "\033[32m"
#define BLUE_COLOR "\033[34m"
#define RESET_COLOR "\033[0m"

#endif

#if defined(OS_WIN)
#include <windows.h> // Use for windows
#endif

using namespace std;

const int gridLen = 25;
void showGrid(bool gridStateOne[gridLen + 1][gridLen + 1]);
void computeState(bool gridStateOne[gridLen + 1][gridLen + 1]);
void clearScreen(void);

int main()
{

    // system( "color A" );//LGT green
    cout << GREEN_COLOR;
    clearScreen();
    bool gridStateOne[gridLen + 1][gridLen + 1] = {};
    int x, y, n;
    string nc;
    string start;
    string filename;
    cout << "                         THE GAME OF life - Implementation in C++" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "Also known simply as life, " << endl;
    cout << "is a cellular automaton devised by the British mathematician John Horton Conway in 1970." << endl;
    cout << endl;
    cout << "Rules" << endl;
    cout << "The universe of the Game of life is an infinite two-dimensional orthogonal grid of square cells," << endl;
    cout << "each of which is in one of two possible states, life or dead. Every" << endl;
    cout << "cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent." << endl;
    cout << "At each step in time, the following transitions occur:" << endl;
    cout << "1. Any live cell with fewer than two live neighbours dies, as if caused by under-population." << endl;
    cout << "2. Any live cell with two or three live neighbours lives on to the next generation." << endl;
    cout << "3. Any live cell with more than three live neighbours dies, as if by over-population." << endl;
    cout << "4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction." << endl;
    cout << endl;
    cout << "O - living cell" << endl;
    cout << ". - dead cell" << endl;
    cout << endl;
    cout << "Enter the number of cells, or 'r' to read cells from file: ";
    cin >> nc;
    cout << endl;

    if (nc == "r")
    {
        while (true)
        {

            cout << "Enter name of file to read from: " << endl;
            cin >> filename;

            ifstream readfile(filename);
            if (readfile.is_open())
            {
                string fileline, xx, yy;

                while (getline(readfile, fileline))
                {
                    stringstream ss(fileline);

                    getline(ss, xx, ' ');
                    getline(ss, yy, ' ');

                    x = stoi(xx);
                    y = stoi(yy);

                    gridStateOne[x][y] = true;
                }
                break;
            }
            else
            {
                cout << "No such file, try again." << endl;
            }
        }
    }
    else
    {

        for (int i = 0; i < stoi(nc); i++)
        {
            cout << stoi(nc) << "Enter the coordinates of cell " << i + 1 << " : ";
            cin >> x >> y;
            gridStateOne[x][y] = true;
            showGrid(gridStateOne);
        }
    }
    cout << "Grid setup is done. Start the game ? (y/n)" << endl;
    showGrid(gridStateOne);
    cin >> start;
    if (start == "y" || start == "Y")
    {
        while (true)
        {
            showGrid(gridStateOne);
            computeState(gridStateOne);
            usleep(200000);
            clearScreen();
        }
    }
    else
    {
        cout << RESET_COLOR;
        clearScreen();
        return 0;
    }
}

void compareGrid(bool gridStateOne[gridLen + 1][gridLen + 1], bool gridStateTwo[gridLen + 1][gridLen + 1])
{
    for (int a = 0; a < gridLen; a++)
    {
        for (int b = 0; b < gridLen; b++)
        {
            gridStateTwo[a][b] = gridStateOne[a][b];
        }
    }
}

void computeState(bool gridStateOne[gridLen + 1][gridLen + 1])
{
    bool gridStateTwo[gridLen + 1][gridLen + 1] = {};
    compareGrid(gridStateOne, gridStateTwo);

    for (int a = 1; a < gridLen; a++)
    {
        for (int b = 1; b < gridLen; b++)
        {
            int isAlive = 0;
            for (int c = -1; c < 2; c++)
            {
                for (int d = -1; d < 2; d++)
                {
                    if (!(c == 0 && d == 0))
                    {
                        if (gridStateTwo[a + c][b + d])
                        {
                            ++isAlive;
                        }
                    }
                }
            }
            if (isAlive < 2)
            {
                gridStateOne[a][b] = false;
            }
            else if (isAlive == 3)
            {
                gridStateOne[a][b] = true;
            }
            else if (isAlive > 3)
            {
                gridStateOne[a][b] = false;
            }
        }
    }
}

void clearScreen(void)
{
// Tested and working on Ubuntu and Cygwin
#if defined(OS_WIN)
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};

    if (hStdOut == INVALID_HANDLE_VALUE)
        return;

    /* Get the number of cells in the current buffer */
    GetConsoleScreenBufferInfo(hStdOut, &csbi);
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', cellCount, homeCoords, &count);

    /* Fill the entire buffer with the current colors and attributes */
    FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count);

    SetConsoleCursorPosition(hStdOut, homeCoords);

#elif defined(OS_LINUX) || defined(OS_MAC)
    cout << "\033[2J;"
         << "\033[1;1H"; // Clears screen and moves cursor to home pos on POSIX systems
#endif
}

void showGrid(bool gridStateOne[gridLen + 1][gridLen + 1])
{
    for (int a = 1; a < gridLen; a++)
    {
        for (int b = 1; b < gridLen; b++)
        {
            if (gridStateOne[a][b] == true)
            {
                cout << " O ";
            }
            else
            {
                cout << " . ";
            }
            if (b == gridLen - 1)
            {
                cout << endl;
            }
        }
    }
}