#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>

using namespace std;

int rows;
int cols;

// 游戏地图元素
enum Tile {
    WALL = 'W',
    EMPTY = '.',
    BOX = 'B'
};

// 游戏地图
vector<vector<Tile>> gameMap;

// 玩家位置
pair<int, int> playerPos;

// 目标位置坐标
vector<pair<int, int>> targetPositions = { {1, 2} };

// 从文件中读取游戏地图
bool loadMap(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << filename << endl;
        return false;
    }

    gameMap.clear();
    string line;

    // 读取第一行以获取行数和列数
    if (getline(file, line)) {
        stringstream ss(line);
        ss >> rows >> cols;
    }
    else {
        return false;
    }

    // 读取每一行
    for (int i = 0; i < rows && getline(file, line); ++i) {
        vector<Tile> row;
        for (char ch : line) {
            // 根据字符设置地图元素
            switch (ch) {
            case 'W': row.push_back(WALL); break;
            case '.': row.push_back(EMPTY); break;
            case 'B': row.push_back(BOX); break;
            case 'P': row.push_back(EMPTY); playerPos = { i, row.size() - 1 }; break;
            default: row.push_back(EMPTY); break;
            }
        }
        gameMap.push_back(row);
    }

    file.close();
    return true;
}

// 显示游戏地图
void displayMap() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (gameMap[i][j] == EMPTY) {
                // 检查是否为玩家位置
                if (make_pair(i, j) == playerPos) {
                    cout << 'P';
                }
                else {
                    // 检查是否为目标位置
                    bool isTarget = false;
                    for (const auto& target : targetPositions) {
                        if (make_pair(i, j) == target) {
                            cout << 'T';
                            isTarget = true;
                            break;
                        }
                    }
                    // 如果不是目标位置，则输出'.'
                    if (!isTarget) {
                        cout << '.';
                    }
                }
            }
            else {
                cout << static_cast<char>(gameMap[i][j]);
            }
            cout << " ";
        }
        cout << endl;
    }
}

// 移动玩家和箱子
bool moveBox(int BoxX, int BoxY, int dx, int dy) {
    // 计算箱子的新位置
    int boxNewX = BoxX + dx;
    int boxNewY = BoxY + dy;

    // 检查箱子的新位置是否有效
    if (boxNewX < 0 || boxNewX >= rows || boxNewY < 0 || boxNewY >= cols) {
        return false;
    }

    // 检查箱子的新位置是否是墙或箱子
    if (gameMap[boxNewX][boxNewY] == WALL || gameMap[boxNewX][boxNewY] == BOX) {
        return false;
    }

    // 移动箱子和玩家
    gameMap[BoxX][BoxY] = EMPTY;
    gameMap[boxNewX][boxNewY] = BOX;
    playerPos.first = BoxX;
    playerPos.second = BoxY;

    return true;
}
bool movePlayer(int dx, int dy) {
    // 计算新的玩家位置
    int newX = playerPos.first + dx;
    int newY = playerPos.second + dy;

    // 检查新位置是否有效
    if (newX < 0 || newX >= rows || newY < 0 || newY >= cols) {
        return false;
    }

    // 检查新位置是否是墙
    if (gameMap[newX][newY] == WALL) {
        return false;
    }

    // 检查新位置是否有箱子
    if (gameMap[newX][newY] == BOX) {
        return moveBox(newX, newY, dx, dy);
    }

    // 移动玩家
    playerPos.first = newX;
    playerPos.second = newY;

    return true;
}

// 检查游戏是否结束
bool isGameOver() {
    for (const auto& target : targetPositions) {
        if (gameMap[target.first][target.second] != BOX) {
            return false;
        }
    }
    return true;
}

// 显示开始界面
void showStartScreen() {
    cout << "Welcome to the Box Pushing Game!" << endl;
    cout << "1. Start Game" << endl;
    cout << "2. Quit" << endl;
    cout << "Enter your choice: ";
}

// 显示选关界面
void showLevelSelectScreen() {
    cout << "Level Select" << endl;
    cout << "1. Level 1" << endl;
    cout << "2. Quit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    int choice;
    bool playing = true;

    while (playing) {
        showStartScreen();
        cin >> choice;

        switch (choice) {
        case 1:
            // 从文件中加载地图，这里假设地图文件名为 "map.txt"
            if (!loadMap("map.txt")) {
                return 1;
            }
            break;
        case 2:
            playing = false;
            continue;
            break;
        default:
            cout << "Invalid choice. Please choose again." << endl;
            continue;
        }

        showLevelSelectScreen();

        char command;
        bool levelPlaying = true;

        while (levelPlaying) {
            displayMap();

            // 检查游戏是否结束
            if (isGameOver()) {
                cout << "Congratulations, you've won!" << endl;
                levelPlaying = false;
                continue;
            }

            cout << "Move (WASD, Q to quit): ";
            cin >> command;

            switch (command) {
            case 'W': case 'w':
                movePlayer(-1, 0);
                break;
            case 'A': case 'a':
                movePlayer(0, -1);
                break;
            case 'S': case 's':
                movePlayer(1, 0);
                break;
            case 'D': case 'd':
                movePlayer(0, 1);
                break;
            case 'Q': case 'q':
                levelPlaying = false;
                break;
            default:
                cout << "Invalid command." << endl;
            }
        }
    }

    return 0;
}