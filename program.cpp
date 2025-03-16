#include<iostream>
#include<stdlib.h>
#include<windows.h>
#include<synchapi.h>
#include<vector>
#include<ctime>

#define WINDOW_XSIZE 152
#define LAND_YPOS 40
#define CONST_PLR_XPOS 20

#define OBSTACLE_PROBABILITY 5
#define PLAYER_ICON "@"
#define OBSTACLE "#"
#define PLAYER_JUMPPOWER 6
int score = 0;

void TPCursor(int X, int Y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // 커서 숨기기
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;  // 커서 숨기기
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // 커서 위치 변경
    COORD coord;
    coord.X = X;  // X 좌표
    coord.Y = Y;   // Y 좌표
    SetConsoleCursorPosition(hConsole, coord);
}

class Player {
    private:
    int Velocity = 0;

    public:
    int Y = LAND_YPOS;

    void Jump() {
        if (Velocity <= 0 && Y >= LAND_YPOS) {
            Velocity = PLAYER_JUMPPOWER;
        }
    }

    void Physics() {
        if (Velocity > 0) {
            Velocity-=1;
            Y-=1;
        } else if (Y < LAND_YPOS) {
            Y+=1;
        }
    }
    void Avatar() {
        TPCursor(CONST_PLR_XPOS,Y);
        printf(PLAYER_ICON);
    }
};

class Obstacle {
    private:
    int X=WINDOW_XSIZE;
    int height;

    void RemoveSelf(std::vector<Obstacle>& obstacles) {
        for (int i = 0; i < obstacles.size(); i++) {
            if (obstacles[i].ID == (*this).ID) {
                obstacles.erase(obstacles.begin()+i);
            }
        }
    }
    public:
    float ID;

    Obstacle(int yh) {
        height = yh;
        ID = score/10;
    }

    void Hitbox(Player player) {
        if (X == CONST_PLR_XPOS && player.Y >= LAND_YPOS - height) {
            TPCursor(0,0);
            printf("사망! 점수 : %d\n", score);
            system("pause");
            exit(0);
        }
    }

    void Sight(std::vector<Obstacle>& obstacles) {
        X-=1;
        if (X < 0) {
            RemoveSelf(obstacles);
        } else {
            Avatar();
        }
    }

    void Avatar() {
        for (int i = 0; i < height; i++) {
            TPCursor(X,LAND_YPOS - i);
            printf(OBSTACLE);
        }
        // printf("%f",ID);
    }
};

std::vector<Obstacle> obstacles = {};

int main() {
    system("chcp 65001");
    system("cls");
    Player player;

    int ObstacleCool=0;

    float SpeedMS = 50;

    srand(static_cast<unsigned int>(std::time(nullptr)));

    while (true) {
        score++;
        ObstacleCool++;
        system("cls");
        player.Physics();
        player.Avatar();

        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            player.Jump();
        }

        if (ObstacleCool >= 3*PLAYER_JUMPPOWER) {
            int isspawn = rand() % OBSTACLE_PROBABILITY;

            if (isspawn == 0) {
                ObstacleCool=0;
                Obstacle obstacle(rand() % 4 + 1);
                obstacles.push_back(obstacle);
            }
        }

        for (int i = 0; i < obstacles.size(); i++) {
            obstacles[i].Sight(obstacles);
            obstacles[i].Hitbox(player);
        }

        if (score < 3600) {
            SpeedMS -= 0.00833333333;
        } else if (score >= 3600) {
            SpeedMS = 20;
        }

        TPCursor(0,0);
        printf("점수 : %d",score);

        Sleep(SpeedMS);
    }

    return 0;
}