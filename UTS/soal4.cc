#include <iostream>
using namespace std;

int main() {
    int tahun_lahir, tahun_sekarang, usia;
    tahun_sekarang = 2024;

    cout << "Masukkan tahun kelahiran kalian: ";
    cin >> tahun_lahir;

    usia = tahun_sekarang - tahun_lahir;

    cout << "Berarti Usia Kalian sekarang adalah " << usia << " Tahun" << endl;

    return 0;
}