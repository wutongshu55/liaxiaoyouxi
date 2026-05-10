#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 40
#define HEIGHT 20
#define SNAKE_MAX_LENGTH 100

// 方向常量
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

// 蛇的结构体
struct Snake {
    int x[SNAKE_MAX_LENGTH];
    int y[SNAKE_MAX_LENGTH];
    int length;
    int direction;
};

struct Food {
    int x;
    int y;
};

// 全局变量
struct Snake snake;
struct Food food;
int score = 0;
int gameover = 0;
HANDLE hConsole;

// 函数声明
void initGame();
void drawGame();
void generateFood();
void moveSnake();
void changeDirection(int direction);
int checkCollision();
void input();
void setColor(int color);
void showGameOver();
void hideCursor();

// 设置控制台颜色
void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

// 初始化游戏
void initGame() {
    int i;
    snake.length = 3;
    snake.direction = RIGHT;

    for (i = 0; i < snake.length; i++) {
        snake.x[i] = WIDTH / 2 - i;
        snake.y[i] = HEIGHT / 2;
    }

    generateFood();
    score = 0;
    gameover = 0;
}

// 绘制游戏界面
void drawGame() {
    system("cls");
    int i, j, k;
    int found;

    // 绘制上边界
    setColor(15); // 白色
    for (i = 0; i < WIDTH + 2; i++) {
        printf("#");
    }
    printf("\n");

    // 绘制游戏区域
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            // 左边界
            if (j == 0) {
                setColor(15);
                printf("#");
            }

            found = 0;

            // 绘制蛇头
            if (i == snake.y[0] && j == snake.x[0]) {
                setColor(10); // 绿色
                printf("O");
                found = 1;
            }
            // 绘制蛇身
            else {
                for (k = 1; k < snake.length; k++) {
                    if (i == snake.y[k] && j == snake.x[k]) {
                        setColor(9); // 蓝色
                        printf("o");
                        found = 1;
                        break;
                    }
                }
            }

            // 绘制食物
            if (!found && i == food.y && j == food.x) {
                setColor(12); // 红色
                printf("*");
                found = 1;
            }

            // 绘制空地
            if (!found) {
                setColor(0); // 黑色
                printf(" ");
            }

            // 右边界
            if (j == WIDTH - 1) {
                setColor(15);
                printf("#");
            }
        }
        printf("\n");
    }

    // 绘制下边界
    setColor(15);
    for (i = 0; i < WIDTH + 2; i++) {
        printf("#");
    }
    printf("\n");

    // 显示分数和控制说明
    setColor(14); // 黄色
    printf("Score: %d\n", score);
    printf("Controls: WASD to move, R to restart, Q to quit\n");
}

// 生成食物
void generateFood() {
    int valid = 0;
    int i;

    while (!valid) {
        valid = 1;
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;

        for (i = 0; i < snake.length; i++) {
            if (food.x == snake.x[i] && food.y == snake.y[i]) {
                valid = 0;
                break;
            }
        }
    }
}

// 移动蛇
void moveSnake() {
    int i;

    for (i = snake.length - 1; i > 0; i--) {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }

    switch (snake.direction) {
    case UP: snake.y[0]--; break;
    case DOWN: snake.y[0]++; break;
    case LEFT: snake.x[0]--; break;
    case RIGHT: snake.x[0]++; break;
    }

    if (snake.x[0] == food.x && snake.y[0] == food.y) {
        snake.length++;
        score += 10;
        generateFood();
    }
}

// 改变方向
void changeDirection(int direction) {
    if ((direction == UP && snake.direction != DOWN) ||
        (direction == DOWN && snake.direction != UP) ||
        (direction == LEFT && snake.direction != RIGHT) ||
        (direction == RIGHT && snake.direction != LEFT)) {
        snake.direction = direction;
    }
}

// 检查碰撞
int checkCollision() {
    int i;

    if (snake.x[0] < 0 || snake.x[0] >= WIDTH ||
        snake.y[0] < 0 || snake.y[0] >= HEIGHT) {
        return 1;
    }

    for (i = 1; i < snake.length; i++) {
        if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i]) {
            return 1;
        }
    }

    return 0;
}

// 处理输入
void input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'w': case 'W': changeDirection(UP); break;
        case 's': case 'S': changeDirection(DOWN); break;
        case 'a': case 'A': changeDirection(LEFT); break;
        case 'd': case 'D': changeDirection(RIGHT); break;
        case 'r': case 'R': initGame(); break;
        case 'q': case 'Q': gameover = 1; break;
        }
    }
}

// 显示游戏结束
void showGameOver() {
    system("cls");
    setColor(12); // 红色
    printf("================================\n");
    printf("           GAME OVER           \n");
    printf("================================\n");
    setColor(14); // 黄色
    printf("      Final Score: %d\n", score);
    printf("================================\n");
    setColor(15); // 白色
    printf("Press R to restart or Q to quit\n");

    while (1) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'r' || key == 'R') {
                initGame();
                break;
            }
            else if (key == 'q' || key == 'Q') {
                gameover = 1;
                break;
            }
        }
        Sleep(100);
    }
}

// 隐藏光标
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// 主函数
int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    srand((unsigned)time(NULL));

    hideCursor();

    printf("Welcome to Snake Game!\n");
    printf("Press any key to start...\n");
    _getch();

    initGame();

    while (!gameover) {
        drawGame();
        input();
        moveSnake();

        if (checkCollision()) {
            showGameOver();
        }

        Sleep(200);
    }

    setColor(15);
    printf("Thanks for playing!\n");
    return 0;
}