#include <iostream>
#include "menu.h"
#include "game.h"
using namespace std;

int main() {
    while (true) {
        int choice = showMenu();

        switch (choice) {
            case 1:
                startGame(getCurrentSkin());
                break;
            case 2:
                changeSkin();
                break;
            case 3:
                showHighScore();
                break;
            case 4:
                cout << "Terima kasih telah bermain!\n";
                return 0;
            default:
                cout << "Pilihan tidak valid.\n";
                break;
        }
    }
    return 0;
}
