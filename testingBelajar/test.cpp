#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Point {
    int x, y;
};

enum Direction { UP, DOWN, LEFT, RIGHT };

class SnakeGame {
private:
    int width, height;
    vector<Point> snake;
    Point food;
    Direction dir;
    bool gameOver;

public:
    SnakeGame(int w, int h) : width(w), height(h), dir(RIGHT), gameOver(false) {
        // Inisialisasi snake
        snake.push_back({w / 2, h / 2});
        spawnFood();
    }

    void spawnFood() {
        food.x = rand() % width;
        food.y = rand() % height;
    }

    void input() {
        timeout(100);
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (dir != DOWN) dir = UP;
                break;
            case KEY_DOWN:
                if (dir != UP) dir = DOWN;
                break;
            case KEY_LEFT:
                if (dir != RIGHT) dir = LEFT;
                break;
            case KEY_RIGHT:
                if (dir != LEFT) dir = RIGHT;
                break;
            case 'q':
                gameOver = true;
                break;
        }
    }

    void logic() {
        Point newHead = snake[0];

        switch (dir) {
            case UP: newHead.y--; break;
            case DOWN: newHead.y++; break;
            case LEFT: newHead.x--; break;
            case RIGHT: newHead.x++; break;
        }

        // Tabrakan dengan tembok
        if (newHead.x < 0 || newHead.x >= width || newHead.y < 0 || newHead.y >= height)
            gameOver = true;

        // Tabrakan dengan tubuh sendiri
        for (int i = 1; i < snake.size(); i++) {
            if (snake[i].x == newHead.x && snake[i].y == newHead.y) {
                gameOver = true;
                break;
            }
        }

        if (gameOver) return;

        snake.insert(snake.begin(), newHead);

        // Makan makanan
        if (newHead.x == food.x && newHead.y == food.y) {
            spawnFood();
        } else {
            snake.pop_back(); // Tidak makan, hapus ekor
        }
    }

    void draw() {
        clear();
        box(stdscr, 0, 0);

        // Gambar makanan
        mvprintw(food.y, food.x, "O");

        // Gambar ular
        for (int i = 0; i < snake.size(); i++) {
            mvprintw(snake[i].y, snake[i].x, i == 0 ? "@" : "o");
        }

        mvprintw(0, 2, " Snake Game - Press 'q' to quit ");
        refresh();
    }

    void run() {
        while (!gameOver) {
            input();
            logic();
            draw();
            usleep(100000); // Delay
        }

        clear();
        mvprintw(height / 2, width / 2 - 5, "Game Over!");
        refresh();
        sleep(2);
    }
};

int main() {
    srand(time(0));
    initscr();
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);

    SnakeGame game(40, 20);
    game.run();

    endwin();
    return 0;
}
