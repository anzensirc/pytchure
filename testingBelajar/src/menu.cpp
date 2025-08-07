#include <iostream>
#include <fstream>
#include <vector>
#include "menu.h"

using namespace std;

vector<char> skins;
int currentSkinIndex = 0;

void loadSkins() {
    skins.clear();
    ifstream file("../assets/skins.txt");
    char ch;
    while (file >> ch) {
        skins.push_back(ch);
    }
    file.close();
}

int showMenu() {
    system("cls");
    cout << "==== GAME SNAKE ====\n";
    cout << "1. Mainkan Game\n";
    cout << "2. Ubah Skin\n";
    cout << "3. Lihat High Score\n";
    cout << "4. Keluar\n";
    cout << "Pilih: ";
    int choice;
    cin >> choice;
    return choice;
}

void changeSkin() {
    loadSkins();
    if (skins.empty()) {
        cout << "Tidak ada skin tersedia.\n";
        system("pause");
        return;
    }

    cout << "=== PILIH SKIN ===\n";
    for (size_t i = 0; i < skins.size(); ++i) {
        cout << i + 1 << ". " << skins[i] << endl;
    }
    cout << "Masukkan nomor skin: ";
    int index;
    cin >> index;

    if (index >= 1 && index <= skins.size()) {
        currentSkinIndex = index - 1;
        cout << "Skin berhasil diubah ke '" << skins[currentSkinIndex] << "'\n";
    } else {
        cout << "Pilihan tidak valid.\n";
    }

    system("pause");
}

char getCurrentSkin() {
    loadSkins();
    if (skins.empty()) return 'O'; // default skin
    return skins[currentSkinIndex];
}
