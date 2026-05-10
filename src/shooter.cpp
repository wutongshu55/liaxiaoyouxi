#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define max(a,b) ((a)>(b)?(a):(b))

#define WIDTH 40
#define HEIGHT 20
#define MAX_BULLETS 30
#define MAX_ENEMIES 20
#define MAX_STARS 50

// 玩家
struct Player {
    int x;
    int y;
    int lives;
};

// 子弹
struct Bullet {
    int x;
    int y;
    int active;
};

// 敌人
struct Enemy {
    int x;
    int y;
    int active;
    int type;      // 0=普通, 1=精英
    int speed;
};

// 星星（背景）
struct Star {
    int x;
    int y;
    int brightness;
};

// 全局变量
struct Player player;
struct Bullet bullets[MAX_BULLETS];
struct Enemy enemies[MAX_ENEMIES];
struct Star stars[MAX_STARS];
int score = 0;
int level = 1;
int wave = 0;
int gameover = 0;
int frameCount = 0;
int spawnTimer = 0;
int comboCount = 0;
int comboTimer = 0;
HANDLE hConsole;

// 函数声明
void initGame();
void drawGame();
void spawnEnemy();
void spawnBullet();
void updateBullets();
void updateEnemies();
void checkCollisions();
void input();
void hideCursor();
void gotoxy(int x, int y);
void setColor(int color);
void showTitle();
void showGameOver();
void drawUI();
void initStars();
void updateStars();

// 设置光标位置
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
}

// 设置颜色
void setColor(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

// 隐藏光标
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

// 初始化星星背景
void initStars() {
    int i;
    for (i = 0; i < MAX_STARS; i++) {
        stars[i].x = rand() % WIDTH;
        stars[i].y = rand() % HEIGHT;
        stars[i].brightness = rand() % 3 + 1; // 1-3 亮度等级
    }
}

// 更新星星（闪烁效果）
void updateStars() {
    int i;
    for (i = 0; i < MAX_STARS; i++) {
        stars[i].brightness = rand() % 3 + 1;
    }
}

// 初始化游戏
void initGame() {
    int i;

    // 初始化玩家
    player.x = WIDTH / 2;
    player.y = HEIGHT - 2;
    player.lives = 3;

    // 初始化子弹
    for (i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }

    // 初始化敌人
    for (i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = 0;
    }

    initStars();
    score = 0;
    level = 1;
    wave = 0;
    gameover = 0;
    frameCount = 0;
    spawnTimer = 0;
    comboCount = 0;
    comboTimer = 0;
}

// 生成敌人
void spawnEnemy() {
    int i;
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].x = rand() % (WIDTH - 2) + 1;
            enemies[i].y = 0;
            enemies[i].active = 1;
            enemies[i].type = (rand() % 100 < 20 && level > 1) ? 1 : 0; // 20%概率生成精英
            enemies[i].speed = level + (rand() % 2); // 速度随等级提升
            return;
        }
    }
}

// 生成子弹
void spawnBullet() {
    int i;
    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = player.x;
            bullets[i].y = player.y - 1;
            bullets[i].active = 1;
            return;
        }
    }
}

// 更新子弹
void updateBullets() {
    int i;
    for (i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].y--;
            if (bullets[i].y < 0) {
                bullets[i].active = 0;
            }
        }
    }
}

// 更新敌人
void updateEnemies() {
    int i;
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            enemies[i].y += enemies[i].speed;
            // 敌人到达底部
            if (enemies[i].y >= HEIGHT) {
                enemies[i].active = 0;
                player.lives--;
                if (player.lives <= 0) {
                    gameover = 1;
                }
            }
        }
    }
}

// 碰撞检测
void checkCollisions() {
    int i, j;

    for (i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;

        for (j = 0; j < MAX_ENEMIES; j++) {
            if (!enemies[j].active) continue;

            // 子弹命中敌人
            if (bullets[i].x == enemies[j].x && bullets[i].y == enemies[j].y) {
                bullets[i].active = 0;
                enemies[j].active = 0;

                // 连击系统
                comboCount++;
                comboTimer = 30; // 连击窗口

                // 计分：精英敌人分数更高
                if (enemies[j].type == 1) {
                    score += 30 * (1 + comboCount / 5);
                } else {
                    score += 10 * (1 + comboCount / 5);
                }
                break;
            }
        }
    }

    // 敌人碰到玩家
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;
        if (enemies[i].x == player.x && enemies[i].y == player.y) {
            enemies[i].active = 0;
            player.lives--;
            if (player.lives <= 0) {
                gameover = 1;
            }
        }
    }
}

// 绘制游戏
void drawGame() {
    int i, j;

    // 绘制边界和背景
    gotoxy(0, 0);
    setColor(11); // 青色
    for (i = 0; i < WIDTH + 2; i++) {
        printf("#");
    }

    for (i = 0; i < HEIGHT; i++) {
        gotoxy(0, i + 1);
        setColor(11);
        printf("#");
        gotoxy(WIDTH + 1, i + 1);
        printf("#");
    }

    gotoxy(0, HEIGHT + 1);
    for (i = 0; i < WIDTH + 2; i++) {
        printf("#");
    }

    // 绘制星星
    for (i = 0; i < MAX_STARS; i++) {
        gotoxy(stars[i].x + 1, stars[i].y + 1);
        switch (stars[i].brightness) {
            case 1: setColor(8); break;  // 暗灰
            case 2: setColor(7); break;  // 亮灰
            case 3: setColor(15); break; // 白色
        }
        printf(".");
    }

    // 绘制敌人
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            gotoxy(enemies[i].x + 1, enemies[i].y + 1);
            if (enemies[i].type == 1) {
                // 精英敌人
                setColor(13); // 紫色
                printf("MM");
                gotoxy(enemies[i].x + 1, enemies[i].y + 2);
                printf("MM");
            } else {
                setColor(12); // 红色
                printf("M");
            }
        }
    }

    // 绘制子弹
    for (i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            gotoxy(bullets[i].x + 1, bullets[i].y + 1);
            setColor(14); // 黄色
            printf("!");
        }
    }

    // 绘制玩家（+1 偏移与边界对齐）
    gotoxy(player.x + 1, player.y + 1);
    setColor(10); // 绿色
    printf("A");
    gotoxy(player.x, player.y + 1);
    printf("<");
    gotoxy(player.x + 2, player.y + 1);
    printf(">");
    gotoxy(player.x + 1, player.y + 2);
    printf("W");
    gotoxy(player.x, player.y + 2);
    printf("=");
    gotoxy(player.x + 2, player.y + 2);
    printf("=");

    // 绘制UI信息
    drawUI();
}

// 绘制UI
void drawUI() {
    int y = HEIGHT + 3;

    gotoxy(0, y);
    setColor(14);
    printf("==========================================\n");

    gotoxy(0, y + 1);
    setColor(15);
    printf(" < 玩家 ");
    setColor(10);
    printf("A");
    setColor(15);
    printf(" >   ");
    setColor(12);
    printf(" M ");
    setColor(15);
    printf(" 敌人   ");
    setColor(14);
    printf(" ! ");
    setColor(15);
    printf(" 子弹");

    gotoxy(0, y + 2);
    setColor(14);
    printf("==========================================\n");

    gotoxy(0, y + 3);
    setColor(11);
    printf(" 分数: ");
    setColor(15);
    printf("%-6d", score);
    setColor(11);
    printf("  等级: ");
    setColor(15);
    printf("%-4d", level);
    setColor(11);
    printf("  生命: ");
    setColor(12);
    for (int i = 0; i < player.lives; i++) {
        printf("<3 ");
    }
    setColor(8);
    for (int i = player.lives; i < 3; i++) {
        printf("   ");
    }

    // 连击显示
    if (comboCount > 0) {
        gotoxy(0, y + 4);
        setColor(13);
        printf(" 连击: %d  ", comboCount);
        if (comboCount >= 10) {
            setColor(14);
            printf("HOT!");
        } else if (comboCount >= 5) {
            setColor(13);
            printf("NICE!");
        }
    }

    gotoxy(0, y + 5);
    setColor(8);
    printf(" A/D 移动 | SPACE 射击 | Q 退出\n");
}

// 输入处理
void input() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
        case 'a': case 'A':
            if (player.x > 2) player.x--;
            break;
        case 'd': case 'D':
            if (player.x < WIDTH - 2) player.x++;
            break;
        case ' ':
            spawnBullet();
            break;
        case 'q': case 'Q':
            gameover = 1;
            break;
        }
    }
}

// 标题画面
void showTitle() {
    system("cls");
    setColor(11);
    printf("\n\n");
    printf("   ########################################\n");
    printf("   #                                      #\n");
    printf("   #       ");
    setColor(14);
    printf("<< 太 空 射 击 >>");
    setColor(11);
    printf("      #\n");
    printf("   #                                      #\n");
    printf("   ########################################\n\n");
    setColor(15);
    printf("   > ");
    setColor(10);
    printf("A");
    setColor(15);
    printf(" = 你\n");
    setColor(12);
    printf("   > M = 敌人\n");
    setColor(13);
    printf("   > MM = 精英敌人（更多分数）\n");
    setColor(14);
    printf("   > ! = 子弹\n\n");
    setColor(7);
    printf("   控制方式：\n");
    printf("   A/D 或 左右方向键 - 移动\n");
    printf("   空格键 - 射击\n");
    printf("   Q - 退出游戏\n\n");
    setColor(14);
    printf("   按任意键开始...\n");
    _getch();
}

// 游戏结束
void showGameOver() {
    system("cls");
    setColor(12);
    printf("\n\n");
    printf("   ########################################\n");
    printf("   #                                      #\n");
    printf("   #          G A M E  O V E R           #\n");
    printf("   #                                      #\n");
    printf("   ########################################\n\n");
    setColor(14);
    printf("   最终得分: %d\n", score);
    printf("   达到等级: %d\n", level);
    printf("   最高连击: %d\n\n", comboCount);

    // 评价
    setColor(15);
    printf("   评价: ");
    if (score >= 1000) {
        setColor(13);
        printf("太空王牌飞行员！\n");
    } else if (score >= 500) {
        setColor(14);
        printf("优秀射手！\n");
    } else if (score >= 200) {
        setColor(10);
        printf("合格的守卫者\n");
    } else {
        setColor(7);
        printf("继续练习！\n");
    }

    setColor(11);
    printf("\n   按 R 重新开始 或 Q 退出\n");

    while (1) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'r' || key == 'R') {
                initGame();
                break;
            } else if (key == 'q' || key == 'Q') {
                gameover = 1;
                break;
            }
        }
        Sleep(100);
    }
}

// 主函数
int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    srand((unsigned)time(NULL));
    hideCursor();

    showTitle();
    initGame();

    // 游戏主循环
    while (1) {
        if (gameover) {
            showGameOver();
            if (gameover) break;
        }

        input();

        // 更新逻辑（每帧执行）
        updateBullets();
        updateEnemies();
        updateStars();
        checkCollisions();

        // 敌人生成逻辑
        spawnTimer++;
        int spawnRate = max(5, 20 - level * 2);
        if (spawnTimer >= spawnRate) {
            spawnTimer = 0;
            spawnEnemy();
            // 高等级时偶尔双倍刷敌
            if (level >= 3 && rand() % 100 < 30) {
                spawnEnemy();
            }
        }

        // 连击计时器
        if (comboTimer > 0) {
            comboTimer--;
            if (comboTimer == 0) {
                comboCount = 0;
            }
        }

        // 等级提升（每100分升一级）
        int newLevel = score / 100 + 1;
        if (newLevel > level) {
            level = newLevel;
        }

        frameCount++;

        // 绘制
        drawGame();

        // 延迟控制帧率
        Sleep(80);
    }

    system("cls");
    setColor(15);
    printf("感谢游玩！\n");
    return 0;
}
