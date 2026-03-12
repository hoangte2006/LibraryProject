#include "Sach.h"
#include "Structs.h"
#include <iostream>
#include <cstring>
#include <limits>

using namespace std;

// ================================================================
//                     PHAN QUAN LY DOC GIA (BST)
// ================================================================

// --- Tim kiem doc gia theo ma the (BST) ---
DocGia* timDocGia(TREE_DocGia root, int maThe) {
    if (root == nullptr) return nullptr;
    if (maThe == root->maThe) return root;
    if (maThe < root->maThe)  return timDocGia(root->pLeft, maThe);
    return timDocGia(root->pRight, maThe);
}

// --- Them doc gia vao BST ---
DocGia* themDocGia(TREE_DocGia &root) {
    DocGia* p = new DocGia;

    cout << "Nhap ma the: ";
    cin >> p->maThe;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Kiem tra trung ma the
    if (timDocGia(root, p->maThe) != nullptr) {
        cout << "Ma the da ton tai! Them doc gia that bai.\n";
        delete p;
        return nullptr;
    }

    cout << "Nhap ho: ";
    cin.getline(p->ho, 50);

    cout << "Nhap ten: ";
    cin.getline(p->ten, 20);

    cout << "Nhap gioi tinh (Nam/Nu): ";
    cin.getline(p->giotinh, 5);

    p->trangThaiThe       = 1; // mac dinh: hoat dong
    p->dsMuonTra.pHead    = nullptr;
    p->dsMuonTra.pTail    = nullptr;
    p->pLeft              = nullptr;
    p->pRight             = nullptr;

    // Chen vao BST
    if (root == nullptr) {
        root = p;
    } else {
        DocGia* cur  = root;
        DocGia* prev = nullptr;
        while (cur != nullptr) {
            prev = cur;
            cur  = (p->maThe < cur->maThe) ? cur->pLeft : cur->pRight;
        }
        if (p->maThe < prev->maThe)
            prev->pLeft  = p;
        else
            prev->pRight = p;
    }

    cout << "Them doc gia thanh cong!\n";
    return p;
}

// --- In danh sach doc gia (inorder: tang dan theo maThe) ---
void inDanhSachDocGia_inorder(TREE_DocGia root) {
    if (root == nullptr) return;
    inDanhSachDocGia_inorder(root->pLeft);
    cout << "Ma the: " << root->maThe
         << " | Ho ten: " << root->ho << " " << root->ten
         << " | Gioi tinh: " << root->giotinh
         << " | Trang thai the: " << (root->trangThaiThe == 1 ? "Hoat dong" : "Da bi khoa")
         << "\n";
    inDanhSachDocGia_inorder(root->pRight);
}

void inDanhSachDocGia(TREE_DocGia root) {
    if (root == nullptr) {
        cout << "Danh sach doc gia rong!\n";
        return;
    }
    cout << "\n--- DANH SACH DOC GIA ---\n";
    inDanhSachDocGia_inorder(root);
}

// --- Khoa / Mo the doc gia ---
void khoaMoTheDocGia(TREE_DocGia root) {
    int ma;
    cout << "Nhap ma the doc gia: ";
    cin >> ma;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    DocGia* dg = timDocGia(root, ma);
    if (dg == nullptr) {
        cout << "Khong tim thay doc gia co ma the: " << ma << "\n";
        return;
    }

    if (dg->trangThaiThe == 1) {
        // Kiem tra con sach dang muon chua
        MuonTra* temp = dg->dsMuonTra.pHead;
        while (temp != nullptr) {
            if (temp->trangThai == 0) {
                cout << "Khong the khoa the. Doc gia dang co sach chua tra!\n";
                return;
            }
            temp = temp->pNext;
        }
        dg->trangThaiThe = 0;
        cout << "Da KHOA the doc gia " << ma << " thanh cong!\n";
    } else {
        dg->trangThaiThe = 1;
        cout << "Da MO the doc gia " << ma << " thanh cong!\n";
    }
}

// ================================================================
//                     PHAN GIAO DICH
// ================================================================

// --- Dem so sach dang muon chua tra ---
int demSachDangMuon(DocGia* dg) {
    int count = 0;
    MuonTra* temp = dg->dsMuonTra.pHead;
    while (temp != nullptr) {
        if (temp->trangThai == 0) count++;
        temp = temp->pNext;
    }
    return count;
}

// --- Nhap ngay ---
Ngay nhapNgay(const char* label) {
    Ngay ng;
    cout << label << " (dd mm yyyy): ";
    cin >> ng.ngay >> ng.thang >> ng.nam;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return ng;
}

// --- Muon sach ---
void muonSach(TREE_DocGia root, ListDauSach &ds) {
    // 1. Tim doc gia
    int ma;
    cout << "Nhap ma the doc gia: ";
    cin >> ma;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    DocGia* dg = timDocGia(root, ma);
    if (dg == nullptr) {
        cout << "Khong tim thay doc gia!\n";
        return;
    }
    if (dg->trangThaiThe == 0) {
        cout << "The doc gia dang bi khoa! Khong the muon sach.\n";
        return;
    }
    if (demSachDangMuon(dg) >= 3) {
        cout << "Doc gia dang muon du 3 cuon! Khong the muon them.\n";
        return;
    }

    // 2. Tim cuon sach
    char maSach[25];
    cout << "Nhap ma sach can muon: ";
    cin.getline(maSach, 25);

    DauSach* dauSachOut = nullptr;
    Sach* sach = timSachTheoMa(ds, maSach, dauSachOut);

    if (sach == nullptr) {
        cout << "Khong tim thay cuon sach co ma: " << maSach << "\n";
        return;
    }
    if (sach->trangThai != 0) {
        cout << "Cuon sach nay khong o trang thai cho muon!\n";
        return;
    }

    // 3. Nhap ngay muon
    Ngay ngayMuon = nhapNgay("Nhap ngay muon");

    // 4. Tao node MuonTra va them vao cuoi danh sach (AddTail O(1))
    MuonTra* mt  = new MuonTra;
    strcpy(mt->maSach, maSach);
    mt->ngayMuon  = ngayMuon;
    mt->ngayTra   = {0, 0, 0};
    mt->trangThai = 0; // dang muon
    mt->pNext     = nullptr;

    if (dg->dsMuonTra.pHead == nullptr) {
        dg->dsMuonTra.pHead = mt;
        dg->dsMuonTra.pTail = mt;
    } else {
        dg->dsMuonTra.pTail->pNext = mt;
        dg->dsMuonTra.pTail        = mt;
    }

    // 5. Cap nhat trang thai cuon sach va so luot muon
    sach->trangThai = 1; // da muon
    dauSachOut->soLuotMuon++;

    cout << "Muon sach thanh cong!\n";
}

// --- Tra sach ---
void traSach(TREE_DocGia root, ListDauSach &ds) {
    // 1. Tim doc gia
    int ma;
    cout << "Nhap ma the doc gia: ";
    cin >> ma;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    DocGia* dg = timDocGia(root, ma);
    if (dg == nullptr) {
        cout << "Khong tim thay doc gia!\n";
        return;
    }

    // 2. Nhap ma sach tra
    char maSach[25];
    cout << "Nhap ma sach can tra: ";
    cin.getline(maSach, 25);

    // 3. Tim node MuonTra tuong ung (dang muon - trangThai == 0)
    MuonTra* mt = dg->dsMuonTra.pHead;
    bool found  = false;
    while (mt != nullptr) {
        if (strcmp(mt->maSach, maSach) == 0 && mt->trangThai == 0) {
            found = true;
            break;
        }
        mt = mt->pNext;
    }

    if (!found) {
        cout << "Khong tim thay phieu muon hop le cho ma sach: " << maSach << "\n";
        return;
    }

    // 4. Nhap ngay tra, cap nhat trang thai phieu
    mt->ngayTra   = nhapNgay("Nhap ngay tra");
    mt->trangThai = 1; // da tra

    // 5. Cap nhat trang thai cuon sach -> cho muon duoc
    DauSach* dauSachOut = nullptr;
    Sach* sach = timSachTheoMa(ds, maSach, dauSachOut);
    if (sach != nullptr) {
        sach->trangThai = 0; // cho muon duoc
    }

    cout << "Tra sach thanh cong!\n";
}

// ================================================================
//                          MENU
// ================================================================

void menuDocGia(TREE_DocGia &root) {
    int choice;
    do {
        cout << "\n===== MENU QUAN LY DOC GIA =====\n";
        cout << "1. Them doc gia\n";
        cout << "2. In danh sach doc gia\n";
        cout << "3. Khoa / Mo the doc gia\n";
        cout << "0. Quay lai\n";
        cout << "Lua chon: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: themDocGia(root);       break;
            case 2: inDanhSachDocGia(root); break;
            case 3: khoaMoTheDocGia(root);  break;
            case 0: break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);
}

void menuGiaoDich(TREE_DocGia &root, ListDauSach &ds) {
    int choice;
    do {
        cout << "\n===== MENU GIAO DICH =====\n";
        cout << "1. Muon sach\n";
        cout << "2. Tra sach\n";
        cout << "0. Quay lai\n";
        cout << "Lua chon: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: muonSach(root, ds); break;
            case 2: traSach(root, ds);  break;
            case 0: break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);
}
