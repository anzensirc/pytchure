#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

struct Struk {
    int id;
    std::string tanggal;
    std::string namaBarang;
    int jumlah;
    double harga;
};

std::vector<Struk> bacaData() {
    std::ifstream file("struk.txt");
    std::vector<Struk> data;
    Struk s;
    while (file >> s.id >> s.tanggal >> s.namaBarang >> s.jumlah >> s.harga) {
        data.push_back(s);
    }
    return data;
}

void simpanData(const std::vector<Struk>& data) {
    std::ofstream file("struk.txt");
    for (const auto& s : data) {
        file << s.id << " " << s.tanggal << " " << s.namaBarang << " "
             << s.jumlah << " " << s.harga << "\n";
    }
}

void tambahStruk() {
    Struk s;
    std::cout << "ID: "; std::cin >> s.id;
    std::cout << "Tanggal (YYYY-MM-DD): "; std::cin >> s.tanggal;
    std::cout << "Nama Barang: "; std::cin >> s.namaBarang;
    std::cout << "Jumlah: "; std::cin >> s.jumlah;
    std::cout << "Harga: "; std::cin >> s.harga;

    std::vector<Struk> data = bacaData();
    data.push_back(s);
    simpanData(data);
    std::cout << "Struk berhasil ditambahkan!\n";
}

void tampilkanStruk() {
    std::vector<Struk> data = bacaData();
    std::cout << std::left << std::setw(5) << "ID" << std::setw(12) << "Tanggal"
              << std::setw(15) << "Barang" << std::setw(8) << "Jumlah"
              << std::setw(10) << "Harga" << "\n";
    for (const auto& s : data) {
        std::cout << std::left << std::setw(5) << s.id
                  << std::setw(12) << s.tanggal
                  << std::setw(15) << s.namaBarang
                  << std::setw(8) << s.jumlah
                  << std::setw(10) << s.harga << "\n";
    }
}

void ubahStruk() {
    int id;
    std::cout << "Masukkan ID struk yang ingin diubah: ";
    std::cin >> id;

    std::vector<Struk> data = bacaData();
    bool ditemukan = false;
    for (auto& s : data) {
        if (s.id == id) {
            std::cout << "Nama Barang Baru: "; std::cin >> s.namaBarang;
            std::cout << "Jumlah Baru: "; std::cin >> s.jumlah;
            std::cout << "Harga Baru: "; std::cin >> s.harga;
            ditemukan = true;
            break;
        }
    }

    if (ditemukan) {
        simpanData(data);
        std::cout << "Data berhasil diubah.\n";
    } else {
        std::cout << "ID tidak ditemukan.\n";
    }
}

void hapusStruk() {
    int id;
    std::cout << "Masukkan ID struk yang ingin dihapus: ";
    std::cin >> id;

    std::vector<Struk> data = bacaData();
    std::vector<Struk> hasil;

    bool dihapus = false;
    for (const auto& s : data) {
        if (s.id != id) {
            hasil.push_back(s);
        } else {
            dihapus = true;
        }
    }

    if (dihapus) {
        simpanData(hasil);
        std::cout << "Struk berhasil dihapus.\n";
    } else {
        std::cout << "ID tidak ditemukan.\n";
    }
}

int main() {
    int pilihan;
    do {
        std::cout << "\n=== Menu Aplikasi Struk ===\n";
        std::cout << "1. Tambah Struk\n";
        std::cout << "2. Tampilkan Semua Struk\n";
        std::cout << "3. Ubah Struk\n";
        std::cout << "4. Hapus Struk\n";
        std::cout << "5. Keluar\n";
        std::cout << "Pilih menu: ";
        std::cin >> pilihan;

        switch (pilihan) {
            case 1: tambahStruk(); break;
            case 2: tampilkanStruk(); break;
            case 3: ubahStruk(); break;
            case 4: hapusStruk(); break;
            case 5: std::cout << "Keluar...\n"; break;
            default: std::cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 5);

    return 0;
}
