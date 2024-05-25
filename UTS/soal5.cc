#include <iostream>
using namespace std;

int main() {
    int angka1, angka2;

    cout << "Masukkan Angka Pertama : ";
    cin >> angka1;

    cout << "Masukkan Angka Kedua : ";
    cin >> angka2;

    cout << "Penjumlahan : " << angka1 + angka2 << endl;

    cout << "Pengurangan : " << angka1 - angka2 << endl;

    cout << "Perkalian : " << angka1 * angka2 << endl;

    if (angka2 != 0) {
        cout << "Pembagian : " << angka1 / angka2 << endl;
    } else {
        cout << "Pembagian : Tidak dapat membagi dengan nol" << endl;
    }

    return 0;
}
