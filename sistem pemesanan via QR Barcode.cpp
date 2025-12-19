#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>

using namespace std;

struct MenuItem {
    string name;
    int price;
};

struct OrderItem {
    MenuItem item;
    int quantity;
};

void tampilkanMenu(const vector<MenuItem>& menu) {
    cout << "Menu RM Padang:\n";
    for (size_t i = 0; i < menu.size(); ++i) {
        cout << i + 1 << ". " << menu[i].name << " - Rp " << menu[i].price << '\n';
    }
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int bacaInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) {
            clearInput();
            return x;
        } else {
            cout << "Input harus angka. Coba lagi.\n";
            clearInput();
        }
    }
}

string bacaLine(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

int main() {
    // isi menu tanpa brace-initializer
    vector<MenuItem> menu;
    MenuItem m;
    m.name = "Rendang"; m.price = 25000; menu.push_back(m);
    m.name = "Soto Padang"; m.price = 20000; menu.push_back(m);
    m.name = "Dendeng Balado"; m.price = 22000; menu.push_back(m);
    m.name = "Sayur Singkong"; m.price = 15000; menu.push_back(m);
    m.name = "Teh Manis"; m.price = 5000; menu.push_back(m);

    string pilihanMakan;
    string meja = "";
    string alamat = "";
    vector<OrderItem> orderList;
    string catatan = "";
    string metodeBayar;
    int pilihanMenu = -1;
    int jumlah = 0;
    char tambahCatatan = 'N';

    cout << "Mulai: Customer scan QR di meja\n";
    tampilkanMenu(menu);

    while (true) {
        cout << "\nPilih 'M' untuk Makan di sini atau 'B' untuk Bungkus: ";
        if (!(cin >> pilihanMakan)) {
            clearInput();
            continue;
        }
        char c = toupper(pilihanMakan[0]);
        if (c == 'M') {
            clearInput();
            cout << "Pilih meja: ";
            getline(cin, meja);
            cout << "Meja " << meja << " telah dikonfirmasi.\n";
            break;
        } else if (c == 'B') {
            clearInput();
            alamat = bacaLine("Input alamat atau nama pemesan: ");
            cout << "Alamat atau nama pemesan: " << alamat << '\n';
            break;
        } else {
            cout << "Pilihan salah, coba lagi.\n";
            clearInput();
        }
    }

    while (true) {
        tampilkanMenu(menu);
        pilihanMenu = bacaInt("Pilih nomor menu yang ingin dipesan (0 untuk selesai): ");
        if (pilihanMenu == 0) break;
        if (pilihanMenu < 0 || pilihanMenu > (int)menu.size()) {
            cout << "Pilihan menu salah.\n";
            continue;
        }
        jumlah = bacaInt("Masukkan jumlah: ");
        if (jumlah <= 0) {
            cout << "Jumlah harus lebih dari 0.\n";
            continue;
        }

        // tanpa range-for, pakai loop index untuk cek keberadaan
        bool found = false;
        for (size_t i = 0; i < orderList.size(); ++i) {
            if (orderList[i].item.name == menu[pilihanMenu - 1].name) {
                orderList[i].quantity += jumlah;
                found = true;
                break;
            }
        }
        if (!found) {
            OrderItem oi;
            oi.item = menu[pilihanMenu - 1];
            oi.quantity = jumlah;
            orderList.push_back(oi);
        }
        cout << "Ditambahkan: " << menu[pilihanMenu - 1].name << " x " << jumlah << '\n';
    }

    if (orderList.empty()) {
        cout << "Anda tidak memesan apapun. Program selesai.\n";
        return 0;
    }

    cout << "\nRingkasan Pesanan:\n";
    int total = 0;
    for (size_t i = 0; i < orderList.size(); ++i) {
        int hargaItem = orderList[i].item.price * orderList[i].quantity;
        cout << orderList[i].item.name << " x " << orderList[i].quantity << " = Rp " << hargaItem << '\n';
        total += hargaItem;
    }
    cout << "Total: Rp " << total << '\n';

    cout << "Ingin tambah catatan? (Y/N): ";
    if (!(cin >> tambahCatatan)) {
        clearInput();
        tambahCatatan = 'N';
    }
    clearInput();
    if (toupper(tambahCatatan) == 'Y') {
        catatan = bacaLine("Input catatan: ");
        cout << "Catatan: " << catatan << '\n';
    } else {
        cout << "Lewati catatan\n";
    }

    while (true) {
        cout << "\nPilih metode pembayaran:\n1. QRIS/Transfer\n2. Bayar tunai di tempat\nPilihan: ";
        if (!(cin >> metodeBayar)) {
            clearInput();
            continue;
        }
        if (metodeBayar == "1" || metodeBayar == "2") {
            clearInput();
            break;
        } else {
            cout << "Pilihan metode pembayaran salah.\n";
            clearInput();
        }
    }

    if (metodeBayar == "1") {
        cout << "Proses bayar via gateway (QRIS/Transfer)...\nPembayaran berhasil.\n";
    } else {
        cout << "Bayar tunai di tempat.\n";
    }

    cout << "\n--- Bukti Pembayaran & Struk ---\n";
    cout << "RM Padang\n";
    cout << "Meja: " << (meja.empty() ? "-" : meja)
         << ", Alamat/Nama: " << (alamat.empty() ? "-" : alamat) << '\n';
    for (size_t i = 0; i < orderList.size(); ++i) {
        cout << orderList[i].item.name << " x " << orderList[i].quantity
             << " - Rp " << orderList[i].item.price * orderList[i].quantity << '\n';
    }
    cout << "Total: Rp " << total << '\n';
    if (!catatan.empty()) cout << "Catatan: " << catatan << '\n';
    cout << "Metode pembayaran: " << (metodeBayar == "1" ? "QRIS/Transfer" : "Tunai") << '\n';

    cout << "\nKirim order ke Dapur...\n";
    cout << "Status: Sedang diproses (Persiapan)\n";
    cout << "Memasak...\n";
    cout << "Packing / Penyajian...\n";
    cout << "Status: Siap (Ready)\n";
    cout << "Order selesai. Update database / feedback.\n";

    return 0;
}
