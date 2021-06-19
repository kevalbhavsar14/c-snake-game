/*
Date   : 19-6-2021
Aim    : Snake game in terminal
Source :
*/
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>

#define MAP_WIDTH 30
#define MAP_HEIGHT 20
#define DELAY 100 // in Milliseconds

typedef struct Position
{
    int x, y;
} Position;

DWORD WINAPI input(LPVOID inputVar);
void clear(char map[][MAP_WIDTH]);
void displayMap(char map[][MAP_WIDTH]);

void main()
{
    // Game Variables
    char map[MAP_HEIGHT][MAP_WIDTH] = {' '};
    Position snake[(MAP_HEIGHT * MAP_WIDTH)];
    Position food;
    Position inputDir;
    int snakeLength = 1;
    int score = 0;
    int exitGame = 0;

    // Input Thread Setup
    DWORD ThreadId;
    HANDLE inputThreadHandle;
    inputThreadHandle = CreateThread(NULL, 0, input, &inputDir, 0, &ThreadId);

    // Game Variables Setup
    snake[0].x = 10;
    snake[0].y = 10;

    food.x = 5;
    food.y = 5;

    inputDir.x = 0;
    inputDir.y = 0;

    // Cursor positions to move
    COORD start = {0, 0};
    COORD death_text = {MAP_WIDTH / 2 - 4,
                        MAP_HEIGHT / 2 + 2};
    COORD end = {0, MAP_HEIGHT + 3};

    system("cls");
    // Game Loop
    while (!exitGame)
    {
        // Move Cursor
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), start);
        clear(map);

        // Update Snake
        if (food.x == snake[0].x && food.y == snake[0].y)
        {
            score++;
            snakeLength++;
            food.x = rand() % MAP_WIDTH;
            food.y = rand() % MAP_HEIGHT;
        }

        for (int i = snakeLength - 1; i > 0; i--)
        {
            snake[i] = snake[i - 1];
        }

        snake[0].x += inputDir.x;
        snake[0].y += inputDir.y;

        // Collision
        if (snake[0].x < 0 || snake[0].x >= MAP_WIDTH || snake[0].y < 0 || snake[0].y >= MAP_HEIGHT)
        {
            exitGame = 1;
        }
        for (int i = 1; i < snakeLength; i++)
        {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            {
                exitGame = 1;
                break;
            }
        }

        // Display
        map[food.y][food.x] = 'O';
        for (int i = 0; i < snakeLength; i++)
        {
            int x = snake[i].x;
            int y = snake[i].y;

            map[y][x] = '#';
        }
        printf("Score : %d\n", score);
        displayMap(map);
        Sleep(DELAY);
    }
    // Death Message
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), death_text);
    printf("You Died!");
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), end);

    CloseHandle(inputThreadHandle);
}

DWORD WINAPI input(LPVOID inputVar)
{
    Position *inputDir = (Position *)inputVar;
    char key;
    while (1)
    {
        key = getch();
        switch (key)
        {
        case 'w':
        case 'W':
            inputDir->x = 0;
            inputDir->y = -1;
            break;

        case 'a':
        case 'A':
            inputDir->x = -1;
            inputDir->y = 0;
            break;

        case 's':
        case 'S':
            inputDir->x = 0;
            inputDir->y = 1;
            break;

        case 'd':
        case 'D':
            inputDir->x = 1;
            inputDir->y = 0;
            break;
        }
    }
}

void clear(char map[][MAP_WIDTH])
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            map[i][j] = ' ';
        }
    }
}

void displayMap(char map[][MAP_WIDTH])
{
    for (int i = 0; i < MAP_WIDTH + 2; i++)
    {
        printf("%c", 219);
    }
    printf("\n");
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        printf("%c", 219);
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            printf("%c", map[i][j]);
        }
        printf("%c\n", 219);
    }
    for (int i = 0; i < MAP_WIDTH + 2; i++)
    {
        printf("%c", 219);
    }
    printf("\n");
}
