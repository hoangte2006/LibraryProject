#include "Docgia.h"
#include "Structs.h"
#include <iostream>
#include <limits>
#include <cstring>

using namespace std;

// ============================================================
//  HÀM PHỤ TRỢ - NGÀY THÁNG
// ============================================================

Ngay nhapNgay(const char* label) {
    Ngay ng;
    while (true) {
        cout << label;
        cin >> ng.ngay >> ng.thang >> ng.nam;

        bool hopLe = true;
        if (ng.thang < 1 || ng.thang > 12) hopLe = false;
        if (ng.ngay  < 1 || ng.ngay  > 31) hopLe = false;
        if (ng.nam < 2000 || ng.nam > 2100) hopLe = false;

        if (hopLe && (ng.thang == 4 || ng.thang == 6 ||
                      ng.thang == 9 || ng.thang == 11)) {
            if (ng.ngay > 30) hopLe = false;
        }
        if (hopLe && ng.thang == 2) {
            bool nhuan = (ng.nam % 4 == 0 && ng.nam % 100 != 0) ||
                         (ng.nam % 400 == 0);
            if (ng.ngay > (nhuan ? 29 : 28)) hopLe = false;
        }

        if (hopLe) break;
        cout << "  [!] Ngay khong hop le. Nhap lai (dd mm yyyy):\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return ng;
}

// Trả về true nếu a > b
bool ngayLonHon(const Ngay& a, const Ngay& b) {
    if (a.nam   != b.nam)   return a.nam   > b.nam;
    if (a.thang != b.thang) return a.thang > b.thang;
    return a.ngay > b.ngay;
}

// Tính số ngày chênh lệch (xấp xỉ, đủ dùng để kiểm tra quá hạn)
int soNgayChenhLech(const Ngay& tu, const Ngay& den) {
    int t = tu.nam  * 365 + tu.thang  * 30 + tu.ngay;
    int d = den.nam * 365 + den.thang * 30 + den.ngay;
    return d - t;
}

// ============================================================
//  HÀM PHỤ TRỢ - DANH SÁCH MƯỢN TRẢ
// ============================================================

void addTailMuonTra(ListMuonTra& list, MuonTra* newNode) {
    newNode->pNext = nullptr;
    if (list.pTail == nullptr) {
        list.pHead = list.pTail = newNode;
    } else {
        list.pTail->pNext = newNode;
        list.pTail        = newNode;
    }
}

// ============================================================
//  HÀM PHỤ TRỢ - TÌM KIẾM SÁCH
// ============================================================

// Tìm DauSach theo ISBN trong mảng nodes[]
DauSach* timDauSach(ListDauSach& ds, const char* ISBN) {
    for (int i = 0; i < ds.n; i++) {
        if (strcmp(ds.nodes[i]->ISBN, ISBN) == 0)
            return ds.nodes[i];
    }
    return nullptr;
}

// Tìm một cuốn Sach cụ thể còn cho mượn (trangThai == 0)
Sach* timSachCoTheMuon(DauSach* dauSach) {
    Sach* temp = dauSach->dsSach.pHead;
    while (temp != nullptr) {
        if (temp->trangThai == 0) return temp;
        temp = temp->pNext;
    }
    return nullptr;
}

// ============================================================
//  ĐẾM SÁCH ĐANG MƯỢN
// ============================================================

int demSachDangMuon(DocGia* dg) {
    if (dg == nullptr) return 0;
    int count = 0;
    MuonTra* temp = dg->dsMuonTra.pHead;
    while (temp != nullptr) {
        if (temp->trangThai == 0) count++;
        temp = temp->pNext;
    }
    return count;
}

// ============================================================
//  KIỂM TRA QUÁ HẠN (hạn mượn: 14 ngày)
// ============================================================

bool kiemTraQuaHan(DocGia* dg) {
    if (dg == nullptr) return false;

    Ngay homNay = nhapNgay("Nhap ngay hom nay de kiem tra (dd mm yyyy): ");

    bool coQuaHan = false;
    MuonTra* temp = dg->dsMuonTra.pHead;
    while (temp != nullptr) {
        if (temp->trangThai == 0) {  // đang mượn
            int soNgay = soNgayChenhLech(temp->ngayMuon, homNay);
            if (soNgay > 14) {
                cout << "  [!] Sach ISBN [" << temp->maSach
                     << "] da muon " << soNgay << " ngay - QUA HAN!\n";
                coQuaHan = true;
            }
        }
        temp = temp->pNext;
    }
    return coQuaHan;
}

// ============================================================
//  GIAO DỊCH: MƯỢN SÁCH
// ============================================================

bool muonSach(TREE_DocGia root, ListDauSach& ds) {
    cout << "\n--- MUON SACH ---\n";

    // 1. Tìm độc giả
    int maThe;
    cout << "Nhap ma the: "; cin >> maThe;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    DocGia* dg = timDocGia(root, maThe);
    if (dg == nullptr) {
        cout << "  [!] Khong tim thay doc gia.\n"; return false;
    }
    if (dg->trangThaiThe == 0) {
        cout << "  [!] The doc gia dang bi khoa.\n"; return false;
    }
    if (demSachDangMuon(dg) >= 3) {
        cout << "  [!] Doc gia dang muon du 3 quyen, khong the muon them.\n"; return false;
    }

    // 2. Tìm đầu sách theo ISBN
    char ISBN[25];
    cout << "Nhap ISBN sach: "; cin >> ISBN;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    DauSach* dauSach = timDauSach(ds, ISBN);
    if (dauSach == nullptr) {
        cout << "  [!] Khong tim thay sach co ISBN: " << ISBN << "\n"; return false;
    }

    // 3. Tìm cuốn cụ thể còn có thể mượn
    Sach* cuon = timSachCoTheMuon(dauSach);
    if (cuon == nullptr) {
        cout << "  [!] Tat ca cac cuon \"" << dauSach->tenSach << "\" da duoc muon het.\n";
        return false;
    }

    // 4. Nhập ngày mượn
    Ngay ngayMuon = nhapNgay("Nhap ngay muon (dd mm yyyy): ");

    // 5. Tạo bản ghi mượn và thêm vào danh sách
    MuonTra* mt = new MuonTra;
    strcpy(mt->maSach, ISBN);
    mt->ngayMuon  = ngayMuon;
    mt->trangThai = 0;       // 0: đang mượn
    mt->pNext     = nullptr;

    addTailMuonTra(dg->dsMuonTra, mt);

    // 6. Đánh dấu cuốn cụ thể đã được mượn
    cuon->trangThai = 1;     // 1: đã mượn
    dauSach->soLuotMuon++;

    cout << "  [OK] Muon sach \"" << dauSach->tenSach << "\" thanh cong!\n";
    cout << "       Vi tri: " << cuon->viTri << "\n";
    return true;
}

// ============================================================
//  GIAO DỊCH: TRẢ SÁCH
// ============================================================

bool traSach(TREE_DocGia root, ListDauSach& ds) {
    cout << "\n--- TRA SACH ---\n";

    // 1. Tìm độc giả
    int maThe;
    cout << "Nhap ma the: "; cin >> maThe;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    DocGia* dg = timDocGia(root, maThe);
    if (dg == nullptr) {
        cout << "  [!] Khong tim thay doc gia.\n"; return false;
    }

    // 2. Nhập ISBN cần trả
    char ISBN[25];
    cout << "Nhap ISBN sach muon tra: "; cin >> ISBN;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // 3. Tìm bản ghi mượn còn đang mượn (trangThai == 0)
    MuonTra* target = dg->dsMuonTra.pHead;
    while (target != nullptr) {
        if (strcmp(target->maSach, ISBN) == 0 && target->trangThai == 0) break;
        target = target->pNext;
    }
    if (target == nullptr) {
        cout << "  [!] Khong tim thay ban ghi muon hop le cho ISBN nay.\n"; return false;
    }

    // 4. Nhập ngày trả và kiểm tra hợp lệ
    Ngay ngayTra = nhapNgay("Nhap ngay tra (dd mm yyyy): ");
    if (ngayLonHon(target->ngayMuon, ngayTra)) {
        cout << "  [!] Ngay tra phai >= ngay muon. Huy thao tac.\n"; return false;
    }

    // 5. Cập nhật bản ghi mượn
    target->ngayTra   = ngayTra;
    target->trangThai = 1;   // 1: đã trả

    // 6. Hoàn lại trạng thái cuốn sách cụ thể
    DauSach* dauSach = timDauSach(ds, ISBN);
    if (dauSach != nullptr) {
        Sach* temp = dauSach->dsSach.pHead;
        while (temp != nullptr) {
            if (temp->trangThai == 1) {  // tìm cuốn đang được mượn
                temp->trangThai = 0;     // hoàn lại: cho mượn được
                break;
            }
            temp = temp->pNext;
        }
        cout << "  [OK] Tra sach \"" << dauSach->tenSach << "\" thanh cong!\n";
    } else {
        cout << "  [OK] Tra sach thanh cong!\n";
    }

    return true;
}

// ============================================================
//  MENU QUẢN LÝ ĐỘC GIẢ
// ============================================================

void themDocGiaMenu(TREE_DocGia& root) {
    cout << "\n--- THEM DOC GIA ---\n";

    int maThe;
    cout << "Nhap ma the: "; cin >> maThe;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    DocGia* dg = taoDocGia(maThe);

    cout << "Nhap ho           : "; cin.getline(dg->ho,      50);
    cout << "Nhap ten          : "; cin.getline(dg->ten,     20);
    cout << "Gioi tinh (Nam/Nu): "; cin.getline(dg->giotinh,  5);

    int tt;
    cout << "Trang thai (1-Hoat dong / 0-Khoa): "; cin >> tt;
    dg->trangThaiThe = (tt == 1) ? 1 : 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (themDocGia(root, dg))
        cout << "  [OK] Them doc gia thanh cong!\n";
    // Nếu thất bại, themDocGia() đã tự in thông báo và delete dg
}

void inDocGiaMenu(const TREE_DocGia root) {
    cout << "\n--- DANH SACH DOC GIA ---\n";
    if (root == nullptr) {
        cout << "  (Chua co doc gia nao)\n";
        return;
    }
    inDanhSachDocGia(root);
}

void khoaMoTheMenu(TREE_DocGia root) {
    cout << "\n--- KHOA / MO THE DOC GIA ---\n";

    int maThe;
    cout << "Nhap ma the: "; cin >> maThe;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    DocGia* dg = timDocGia(root, maThe);
    if (dg == nullptr) {
        cout << "  [!] Khong tim thay doc gia co ma the " << maThe << "\n";
        return;
    }

    if (dg->trangThaiThe == 1) {
        // Đang hoạt động → muốn khoá: kiểm tra còn sách đang mượn không
        int soSach = demSachDangMuon(dg);
        if (soSach > 0) {
            cout << "  [!] Doc gia dang muon " << soSach
                 << " quyen. Khong the khoa the!\n";
            return;
        }
        dg->trangThaiThe = 0;
        cout << "  [OK] Da KHOA the doc gia " << maThe << "\n";
    } else {
        // Đang khoá → mở
        dg->trangThaiThe = 1;
        cout << "  [OK] Da MO the doc gia " << maThe << "\n";
    }
}

void menuDocGia(TREE_DocGia& root) {
    int chon;
    do {
        cout << "\n====== QUAN LY DOC GIA ======\n"
             << "  1. Them doc gia\n"
             << "  2. In danh sach doc gia\n"
             << "  3. Khoa / Mo the doc gia\n"
             << "  0. Quay lai\n"
             << "Chon: ";
        cin >> chon;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (chon) {
            case 1: themDocGiaMenu(root); break;
            case 2: inDocGiaMenu(root);   break;
            case 3: khoaMoTheMenu(root);  break;
            case 0: break;
            default: cout << "  [!] Lua chon khong hop le.\n";
        }
    } while (chon != 0);
}

// ============================================================
//  MENU GIAO DỊCH
// ============================================================

void menuGiaoDich(TREE_DocGia& rootDocGia, ListDauSach& ds) {
    int chon;
    do {
        cout << "\n====== GIAO DICH ======\n"
             << "  1. Muon sach\n"
             << "  2. Tra sach\n"
             << "  0. Quay lai\n"
             << "Chon: ";
        cin >> chon;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (chon) {
            case 1: muonSach(rootDocGia, ds); break;
            case 2: traSach(rootDocGia, ds);  break;
            case 0: break;
            default: cout << "  [!] Lua chon khong hop le.\n";
        }
    } while (chon != 0);
}
