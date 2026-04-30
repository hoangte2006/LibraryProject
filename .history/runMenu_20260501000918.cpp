#include <iostream>
#include <conio.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <iomanip>
#include <windows.h>
#include <algorithm>
#include "Structs.h"
#include "Docgia.h"
#include "Sach.h"
#include "Giaodich.h"
#include "Thongke.h"
#include "File.h"
#include "MuonTra.h"
#include "InputUtils.h"
#include "Test.h"

using namespace std;

const char* menuTrai[] = { "Tong quan", "Quan ly doc gia", "Quan ly sach", "Giao dich", "Thong ke", "Thoat" };

// --- CAC HAM DO HOA (Giu nguyen de dam bao giao dien dep) ---
void gotoxy(int x, int y) {
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Cursor_an_Pos = { (short)x, (short)y };
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void setColor(int ansiCode) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD color;
    switch (ansiCode) {
        case 31: color = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
        case 32: color = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case 33: color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case 34: color = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        case 35: color = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        case 36: color = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        default: color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
    }
    SetConsoleTextAttribute(h, color);
}
void resetColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
void showCursor(bool show) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = show;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void setupConsole() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    showCursor(false);
}

void xoaVung(int x, int y, int width, int height) {
    for (int i = 0; i < height; i++) {
        gotoxy(x, y + i);
        for (int j = 0; j < width; j++) cout << " ";
    }
}

// --- LOGIC TRO GIUP ---
void demThongKeTongQuan(TREE_DocGia root, int& soSachDangMuon, int& soDocGiaQuaHan) {
    if (root == nullptr) return;
    demThongKeTongQuan(root->pLeft, soSachDangMuon, soDocGiaQuaHan);
    soSachDangMuon += root->soSachDangMuon;
    if (coSachQuaHan(root)) soDocGiaQuaHan++;
    demThongKeTongQuan(root->pRight, soSachDangMuon, soDocGiaQuaHan);
}

// --- VE GIAO DIEN ---
void veKhungGiaoDien() {
    system("cls");
    setColor(36); 
    gotoxy(0, 0); cout << "==================================================================================";
    gotoxy(0, 1); cout << "||                     HE THONG QUAN LY THU VIEN CAO CAP                        ||";
    gotoxy(0, 2); cout << "==================================================================================";
    for (int i = 3; i <= 22; i++) { gotoxy(25, i); cout << "||"; }
    gotoxy(0, 23); cout << "==================================================================================";
    resetColor();
}

void hienThiTongQuan(int tongDauSach, int tongDocGia, int dangMuon, int quaHan) {
    int x = 30, y = 6;
    xoaVung(28, 4, 50, 18);
    setColor(33); gotoxy(x+10, 4); cout << "--- TONG QUAN HE THONG ---";
    setColor(37); 
    gotoxy(x, y);   cout << "┌──────────────────────────────────────────┐";
    gotoxy(x, y+1); cout << "│  Tong so dau sach    : " << tongDauSach; gotoxy(x+43, y+1); cout << "│";
    gotoxy(x, y+2); cout << "│  Tong so doc gia     : " << tongDocGia; gotoxy(x+43, y+2); cout << "│";
    gotoxy(x, y+3); cout << "├──────────────────────────────────────────┤";
    gotoxy(x, y+4); cout << "│  Sach dang muon      : " << dangMuon;   gotoxy(x+43, y+4); cout << "│";
    gotoxy(x, y+5); cout << "│  Doc gia qua han     : " << quaHan;     gotoxy(x+43, y+5); cout << "│";
    gotoxy(x, y+6); cout << "└──────────────────────────────────────────┘";
    resetColor();
}

void veMenuList(const char* options[], int n, int selected, int x, int y, bool isFocus) {
    for (int i = 0; i < n; i++) {
        gotoxy(x, y + i * 2);
        if (i == selected) {
            if (isFocus) { setColor(32); cout << " > " << options[i] << "   "; }
            else { setColor(33); cout << "   " << options[i] << "   "; }
        } else { resetColor(); cout << "   " << options[i] << "   "; }
    }
}

int chonMenu(const char* options[], int n, int x, int y, int &currentSelection) {
    while (true) {
        veMenuList(options, n, currentSelection, x, y, true);
        int key = _getch();
        if (key == 'g' || key == 'G') return -2; // Phim tat sinh doc gia
        if (key == 224) {
            key = _getch();
            if (key == 72) currentSelection = (currentSelection - 1 + n) % n; 
            if (key == 80) currentSelection = (currentSelection + 1) % n;     
        } 
        else if (key == 13) return currentSelection; 
        else if (key == 27) return -1;               
    }
}

int chonTuBangDocGia(QuanLyDocGia& ql) {
    if (ql.soLuongDocGia == 0) {
        cout << "Danh sach doc gia rong!\nNhan phim bat ky de thoat.";
        _getch();
        return -1;
    }

    DocGia** fullArr = new DocGia*[ql.soLuongDocGia];
    int full_n = 0;
    BSTtoArray(ql.root, fullArr, full_n);

    char searchKeyword[100] = ""; 
    int vtSearch = 0;
    bool searchMode = false; 
    bool isTypingSearch = false;
    int luaChon = 0;
    const int ITEM_PER_PAGE = 10;

    system("cls");

    while (true) {
        DocGia** displayArr = new DocGia*[full_n];
        int n = 0;
        if (searchMode && strlen(searchKeyword) > 0) {
            string keywordLower = searchKeyword;
            transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), [](unsigned char c){ return tolower(c); });
            for (int i = 0; i < full_n; i++) {
                string hoTen = string(fullArr[i]->ho) + " " + string(fullArr[i]->ten);
                string hoTenLower = hoTen;
                transform(hoTenLower.begin(), hoTenLower.end(), hoTenLower.begin(), [](unsigned char c){ return tolower(c); });
                if (hoTenLower.find(keywordLower) != string::npos || to_string(fullArr[i]->maThe).find(keywordLower) != string::npos) {
                    displayArr[n++] = fullArr[i];
                }
            }
        } else {
            for(int i=0; i<full_n; ++i) displayArr[i] = fullArr[i];
            n = full_n;
        }

        gotoxy(0, 0); // ve de len ma khong giat

        if (n == 0) {
            cout << "--- CHON DOC GIA ---" << string(50, ' ') << "\n";
            if (isTypingSearch || searchMode) {
                cout << "Tim kiem: " << searchKeyword << (isTypingSearch ? "_" : "") << string(40, ' ') << "\n";
            } else {
                cout << string(80, ' ') << "\n";
            }
            cout << "Khong tim thay doc gia nao khop voi '" << searchKeyword << "'!" << string(30, ' ') << "\n";
            for(int i=0; i<13; ++i) cout << string(80, ' ') << "\n"; 
        } else {
            if (luaChon >= n) luaChon = max(0, n - 1);
            int tongTrang = (n > 0) ? ((n - 1) / ITEM_PER_PAGE + 1) : 1;
            int trang = luaChon / ITEM_PER_PAGE;

            cout << "--- CHON DOC GIA (Trang " << trang + 1 << "/" << tongTrang << ") ---" << string(30, ' ') << "\n";
            if (isTypingSearch || searchMode) {
                cout << "Tim kiem: " << searchKeyword << (isTypingSearch ? "_" : "") << string(40, ' ') << "\n";
            } else {
                cout << string(80, ' ') << "\n";
            }
            cout << "   ┌" << string(12, '-') << "┬" << string(27, '-') << "┬" << string(12, '-') << "┬" << string(17, '-') << "┐   " << endl;
            cout << "   │ " << left << setw(10) << "Ma The" << " │ " << left << setw(25) << "Ho Ten" << " │ " << left << setw(10) << "Gioi Tinh" << " │ " << left << setw(15) << "Trang Thai" << " │   " << endl;
            cout << "   ├" << string(12, '-') << "┼" << string(27, '-') << "┼" << string(12, '-') << "┼" << string(17, '-') << "┤   " << endl;

            int start = trang * ITEM_PER_PAGE;
            int end = min(start + ITEM_PER_PAGE, n);

            for (int i = 0; i < ITEM_PER_PAGE; i++) {
                if (start + i < end) {
                    int idx = start + i;
                    if (idx == luaChon) { setColor(47); cout << ">> "; } 
                    else { cout << "   "; }

                    string hoTen = string(displayArr[idx]->ho) + " " + string(displayArr[idx]->ten);
                    if (hoTen.length() > 25) hoTen = hoTen.substr(0, 22) + "...";

                    cout << "│ " << left << setw(10) << displayArr[idx]->maThe << " │ " << left << setw(25) << hoTen << " │ " << left << setw(10) << displayArr[idx]->giotinh << " │ " << left << setw(15) << (displayArr[idx]->trangThaiThe == 1 ? "Hoat dong" : "Khoa") << " │   ";
                    
                    if (idx == luaChon) resetColor();
                    cout << endl;
                } else {
                    cout << "   │ " << string(10, ' ') << " │ " << string(25, ' ') << " │ " << string(10, ' ') << " │ " << string(15, ' ') << " │   \n";
                }
            }

            cout << "   └" << string(12, '-') << "┴" << string(27, '-') << "┴" << string(12, '-') << "┴" << string(17, '-') << "┘   " << endl;
        }

        setColor(36);
        cout << "   (Mui ten: Len/Xuong, PgUp/PgDn, Enter: Chon, F: Tim, ESC: Thoat)" << string(10, ' ') << "\n";
        resetColor();
        
        // Xoa cac dong thua ben duoi de tranh bi luu lai chu cua khung cu
        for(int i=0; i<3; ++i) cout << string(80, ' ') << "\n";

        int key = _getch();
        
        if (isTypingSearch) {
            if (key == 27) { 
                isTypingSearch = false;
            } else if (key == 13) { 
                isTypingSearch = false;
            } else if (key == 8) { 
                if (vtSearch > 0) {
                    vtSearch--; searchKeyword[vtSearch] = '\0'; searchMode = (vtSearch > 0); luaChon = 0;
                } else { searchMode = false; }
            } else if (key == 224 || key == 0) { 
                key = _getch();
                if (key == 72) luaChon = max(0, luaChon - 1);
                else if (key == 80) luaChon = min(n - 1, luaChon + 1);
                else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE);
                else if (key == 81) luaChon = min(n - 1, luaChon + ITEM_PER_PAGE);
            } else if (isprint(key) && vtSearch < 99) {
                searchKeyword[vtSearch++] = (char)key; searchKeyword[vtSearch] = '\0'; searchMode = true; luaChon = 0;
            }
        } else {
            if (key == 'f' || key == 'F') {
                isTypingSearch = true;
                searchMode = true;
            } else if (key == 224 || key == 0) {
                key = _getch();
                if (key == 72) luaChon = max(0, luaChon - 1); 
                else if (key == 80) luaChon = min(n - 1, luaChon + 1);
                else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE);
                else if (key == 81) luaChon = min(n - 1, luaChon + ITEM_PER_PAGE);
            } else if (key == 13) {
                if (n > 0) {
                    int maThe = displayArr[luaChon]->maThe;
                    delete[] displayArr; delete[] fullArr; return maThe;
                }
            } else if (key == 27) {
                delete[] displayArr; delete[] fullArr; return -1;
            }
        }
        delete[] displayArr;
    }
}

DauSach* chonTuBangDauSach(ListDauSach& ds, DocGia* dg = nullptr) {
    if (ds.n == 0) {
        cout << "Danh sach dau sach rong!\n";
        return nullptr;
    }

    int luaChon = 0;
    int trang = 0;
    const int ITEM_PER_PAGE = 10;
    char searchKeyword[100] = "";
    int vtSearch = 0;
    bool searchMode = false;
    bool isTypingSearch = false;

    system("cls"); 

    while (true) {
        DauSach** displayArr = new DauSach*[ds.n];
        int n = 0;

        if (searchMode && strlen(searchKeyword) > 0) {
            string keywordLower = searchKeyword;
            transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), [](unsigned char c){ return tolower(c); });
            
            string keywordUpper = searchKeyword;
            transform(keywordUpper.begin(), keywordUpper.end(), keywordUpper.begin(), [](unsigned char c){ return toupper(c); });
            for (int i = 0; i < ds.n; i++) {
                if (strstr(ds.nodes[i]->tenSachSearch, keywordLower.c_str()) != nullptr ||
                    strstr(ds.nodes[i]->tacGiaSearch, keywordLower.c_str()) != nullptr ||
                    strstr(ds.nodes[i]->ISBN, keywordUpper.c_str()) != nullptr) {
                    displayArr[n++] = ds.nodes[i];
                }
            }
        } else {
            for (int i = 0; i < ds.n; i++) {
                displayArr[n++] = ds.nodes[i];
            }
        }

        gotoxy(0, 0);
        if (dg != nullptr) {
            setColor(33); cout << "=== DOC GIA: " << dg->ho << " " << dg->ten << " ===" << string(30, ' ') << "\n"; resetColor();
            xemSachDangMuon(dg, ds);
            cout << "------------------------------------------" << string(30, ' ') << "\n";
        }
        
        if (n == 0) {
            cout << "--- CHON DAU SACH ---" << string(30, ' ') << "\n";
            if (isTypingSearch || searchMode) {
                cout << "Tim kiem: " << searchKeyword << (isTypingSearch ? "_" : "") << string(40, ' ') << "\n";
            } else {
                cout << string(80, ' ') << "\n";
            }
            cout << "Khong tim thay dau sach nao khop voi '" << searchKeyword << "'!" << string(30, ' ') << "\n";
            for(int i=0; i<15; ++i) cout << string(80, ' ') << "\n"; 
        } else {
            if (luaChon >= n) luaChon = max(0, n - 1);
            int tongTrang = (n > 0) ? ((n - 1) / ITEM_PER_PAGE + 1) : 1;
            trang = luaChon / ITEM_PER_PAGE;
            
            cout << "--- CHON DAU SACH (Trang " << trang + 1 << "/" << tongTrang << ") ---" << string(30, ' ') << "\n";
            if (isTypingSearch || searchMode) {
                cout << "Tim kiem: " << searchKeyword << (isTypingSearch ? "_" : "") << string(40, ' ') << "\n";
            } else {
                cout << string(80, ' ') << "\n";
            }
            
            cout << "   ┌" << string(17, '-') << "┬" << string(32, '-') << "┬" << string(22, '-') << "┬" << string(17, '-') << "┐   " << endl;
            cout << "   │ " << left << setw(15) << "ISBN" << " │ " << left << setw(30) << "Ten Sach" << " │ " << left << setw(20) << "Tac Gia" << " │ " << left << setw(15) << "The Loai" << " │   " << endl;
            cout << "   ├" << string(17, '-') << "┼" << string(32, '-') << "┼" << string(22, '-') << "┼" << string(17, '-') << "┤   " << endl;

            int start = trang * ITEM_PER_PAGE;
            int end = min(start + ITEM_PER_PAGE, n);

            for (int i = 0; i < ITEM_PER_PAGE; i++) {
                if (start + i < end) {
                    int idx = start + i;
                    if (idx == luaChon) { setColor(47); cout << ">> "; } 
                    else { cout << "   "; }

                    string tenSach = displayArr[idx]->tenSach;
                    if (tenSach.length() > 30) tenSach = tenSach.substr(0, 27) + "...";
                    string tacGia = displayArr[idx]->tacGia;
                    if (tacGia.length() > 20) tacGia = tacGia.substr(0, 17) + "...";

                    cout << "│ " << left << setw(15) << displayArr[idx]->ISBN << " │ " << left << setw(30) << tenSach << " │ " << left << setw(20) << tacGia << " │ " << left << setw(15) << displayArr[idx]->theLoai << " │   ";
                    
                    if (idx == luaChon) resetColor();
                    cout << endl;
                } else {
                    cout << "   │ " << string(15, ' ') << " │ " << string(30, ' ') << " │ " << string(20, ' ') << " │ " << string(15, ' ') << " │   \n";
                }
            }
            cout << "   └" << string(17, '-') << "┴" << string(32, '-') << "┴" << string(22, '-') << "┴" << string(17, '-') << "┘   " << endl;
        }
        
        setColor(36);
        cout << "   (Mui ten: Len/Xuong, PgUp/PgDn, Enter: Chon, F: Tim, ESC: Thoat)" << string(10, ' ') << "\n";
        resetColor();

        for(int i=0; i<3; ++i) cout << string(80, ' ') << "\n";

        int key = _getch();
        if (isTypingSearch) {
            if (key == 27) { 
                isTypingSearch = false;
            } else if (key == 13) { 
                isTypingSearch = false;
            } else if (key == 8) { 
                if (vtSearch > 0) {
                    vtSearch--; searchKeyword[vtSearch] = '\0'; searchMode = (vtSearch > 0); luaChon = 0;
                } else { searchMode = false; }
            } else if (key == 224 || key == 0) { 
                key = _getch();
                if (key == 72) luaChon = max(0, luaChon - 1);
                else if (key == 80) luaChon = min(n - 1, luaChon + 1);
                else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE);
                else if (key == 81) luaChon = min(n - 1, luaChon + ITEM_PER_PAGE);
            } else if (isprint(key) && vtSearch < 99) {
                searchKeyword[vtSearch++] = (char)key; searchKeyword[vtSearch] = '\0'; searchMode = true; luaChon = 0;
            }
        } else {
            if (key == 'f' || key == 'F') {
                isTypingSearch = true;
                searchMode = true;
            } else if (key == 224 || key == 0) {
                key = _getch();
                if (key == 72) luaChon = max(0, luaChon - 1);
                else if (key == 80) luaChon = min(n - 1, luaChon + 1);
                else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE);
                else if (key == 81) luaChon = min(n - 1, luaChon + ITEM_PER_PAGE);
            } else if (key == 13) {
                if (n > 0) {
                    DauSach* selected = displayArr[luaChon];
                    delete[] displayArr;
                    return selected;
                }
            } else if (key == 27) {
                delete[] displayArr;
                return nullptr;
            }
        }
        delete[] displayArr;
    }
}

Sach* chonCuonSachTuDauSach(DauSach* dauSach, DocGia* dg = nullptr, ListDauSach* ds = nullptr) {
    if (dauSach == nullptr) return nullptr;

    // 1. Filter available books into an array
    Sach** availableBooks = new Sach*[dauSach->dsSach.tongSoSach];
    int n = 0;
    Sach* current = dauSach->dsSach.pHead;
    while (current != nullptr) {
        if (current->trangThai == 0) { // Chi lay sach "Cho muon duoc"
            availableBooks[n++] = current;
        }
        current = current->pNext;
    }

    if (n == 0) {
        cout << "Tat ca cac cuon cua dau sach nay da duoc muon hoac thanh ly.\nNhan phim bat ky de quay lai...";
        _getch();
        delete[] availableBooks;
        return nullptr;
    }

    int luaChon = 0;
    const int ITEM_PER_PAGE = 10;
    
    system("cls");

    while (true) {
        gotoxy(0, 0);
        if (dg != nullptr && ds != nullptr) {
            setColor(33); cout << "=== DOC GIA: " << dg->ho << " " << dg->ten << " ===" << string(30, ' ') << "\n"; resetColor();
            xemSachDangMuon(dg, *ds);
            cout << "------------------------------------------" << string(30, ' ') << "\n";
        }
        int tongTrang = (n > 0) ? ((n - 1) / ITEM_PER_PAGE + 1) : 1;
        int trang = luaChon / ITEM_PER_PAGE;

        cout << "--- CHON CUON SACH DE MUON (Trang " << trang + 1 << "/" << tongTrang << ") ---" << string(30, ' ') << "\n";
        cout << "Dau sach: " << dauSach->tenSach << string(40, ' ') << "\n\n";
        cout << "   ┌" << string(26, '-') << "┬" << string(22, '-') << "┐   " << endl;
        cout << "   │ " << left << setw(24) << "Ma Sach" << " │ " << left << setw(20) << "Vi Tri" << " │   " << endl;
        cout << "   ├" << string(26, '-') << "┼" << string(22, '-') << "┤   " << endl;

        int start = trang * ITEM_PER_PAGE;
        int end = min(start + ITEM_PER_PAGE, n);

        for (int i = 0; i < ITEM_PER_PAGE; i++) {
            if (start + i < end) {
                int idx = start + i;
                if (idx == luaChon) { setColor(47); cout << ">> "; } 
                else { cout << "   "; }
                cout << "│ " << left << setw(24) << availableBooks[idx]->maSach << " │ " << left << setw(20) << availableBooks[idx]->viTri << " │   ";
                if (idx == luaChon) resetColor();
                cout << endl;
            } else {
                cout << "   │ " << string(24, ' ') << " │ " << string(20, ' ') << " │   \n";
            }
        }
        cout << "   └" << string(26, '-') << "┴" << string(22, '-') << "┘   " << endl;
        
        setColor(36);
        cout << "   (Mui ten: Chon, Enter: Xac nhan, ESC: Huy)" << string(30, ' ') << "\n";
        resetColor();

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) luaChon = max(0, luaChon - 1);
            else if (key == 80) luaChon = min(n - 1, luaChon + 1);
            else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE);
            else if (key == 81) luaChon = min(n - 1, luaChon + ITEM_PER_PAGE);
        } else if (key == 13) {
            Sach* selected = availableBooks[luaChon];
            delete[] availableBooks;
            return selected;
        } else if (key == 27) {
            delete[] availableBooks;
            return nullptr;
        }
    }
}

// --- UI CHON SACH DANG MUON (DE TRA HOAC BAO MAT) ---
const char* chonSachDangMuonUI(DocGia* docGia, ListDauSach& ds) {
    if (docGia == nullptr || docGia->soSachDangMuon == 0) {
        cout << "Doc gia nay khong muon cuon sach nao.\n";
        _getch();
        return nullptr;
    }

    // 1. Filter borrowed books into an array
    MuonTra** borrowedItems = new MuonTra*[docGia->soSachDangMuon];
    int n = 0;
    MuonTra* current = docGia->dsMuonTra.pHead;
    while (current != nullptr) {
        if (current->trangThai == 0) { // Chi lay sach "Dang muon"
            borrowedItems[n++] = current;
        }
        current = current->pNext;
    }

    if (n == 0) { // Should not happen if soSachDangMuon > 0, but for safety
        cout << "Khong co sach nao dang muon.\n";
        _getch();
        delete[] borrowedItems;
        return nullptr;
    }

    int luaChon = 0;
    const int ITEM_PER_PAGE = 10;
    
    system("cls");

    while (true) {
        gotoxy(0, 0);
        int tongTrang = (n > 0) ? ((n - 1) / ITEM_PER_PAGE + 1) : 1;
        int trang = luaChon / ITEM_PER_PAGE;

        setColor(33); cout << "=== DOC GIA: " << docGia->ho << " " << docGia->ten << " ===" << string(30, ' ') << "\n"; resetColor();
        xemSachDangMuon(docGia, ds);
        cout << "------------------------------------------" << string(30, ' ') << "\n";
        cout << "--- CHON SACH DE TRA/BAO MAT (Trang " << trang + 1 << "/" << tongTrang << ") ---" << string(30, ' ') << "\n";
        cout << "   ┌" << string(26, '-') << "┬" << string(40, '-') << "┬" << string(15, '-') << "┐   " << endl;
        cout << "   │ " << left << setw(24) << "Ma Sach" << " │ " << left << setw(38) << "Ten Sach" << " │ " << left << setw(13) << "Ngay Muon" << " │   " << endl;
        cout << "   ├" << string(26, '-') << "┼" << string(40, '-') << "┼" << string(15, '-') << "┤   " << endl;

        int start = trang * ITEM_PER_PAGE;
        int end = min(start + ITEM_PER_PAGE, n);

        for (int i = 0; i < ITEM_PER_PAGE; i++) {
            if (start + i < end) {
                int idx = start + i;
                if (idx == luaChon) { setColor(47); cout << ">> "; } 
                else { cout << "   "; }

                DauSach* dauSach = nullptr;
                timSachTheoMa(ds, borrowedItems[idx]->maSach, dauSach);
                string tenSach = (dauSach != nullptr) ? dauSach->tenSach : "N/A";
                if (tenSach.length() > 38) tenSach = tenSach.substr(0, 35) + "...";
                
                char ngayMuonStr[12];
                snprintf(ngayMuonStr, sizeof(ngayMuonStr), "%02d/%02d/%d", borrowedItems[idx]->ngayMuon.ngay, borrowedItems[idx]->ngayMuon.thang, borrowedItems[idx]->ngayMuon.nam);

                cout << "│ " << left << setw(24) << borrowedItems[idx]->maSach << " │ " << left << setw(38) << tenSach << " │ " << left << setw(13) << ngayMuonStr << " │   ";
                if (idx == luaChon) resetColor();
                cout << endl;
            } else {
                cout << "   │ " << string(24, ' ') << " │ " << string(38, ' ') << " │ " << string(13, ' ') << " │   \n";
            }
        }
        cout << "   └" << string(26, '-') << "┴" << string(40, '-') << "┴" << string(15, '-') << "┘   " << endl;
        
        setColor(36);
        cout << "   (Mui ten: Chon, Enter: Xac nhan, ESC: Huy)" << string(30, ' ') << "\n";
        resetColor();

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) luaChon = max(0, luaChon - 1); else if (key == 80) luaChon = min(n - 1, luaChon + 1); else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE); else if (key == 81) luaChon = min(n - 1, luaChon + ITEM_PER_PAGE);
        } else if (key == 13) {
            const char* selectedMaSach = borrowedItems[luaChon]->maSach;
            delete[] borrowedItems;
            return selectedMaSach;
        } else if (key == 27) { delete[] borrowedItems; return nullptr; }
    }
}

// --- FORM NHAP/SUA DAU SACH ---
void formNhapDauSach(ListDauSach& ds, DauSach* dsPtr, bool isThemMoi) {
        int winX = 3, winY = 2, winW = 75, winH = 14;
    xoaVung(winX, winY, winW, winH);
    setColor(33);
    for (int i = 0; i < winW; i++) { gotoxy(winX + i, winY); cout << "─"; gotoxy(winX + i, winY + winH - 1); cout << "─"; }
    for (int i = 0; i < winH; i++) { gotoxy(winX, winY + i); cout << "│"; gotoxy(winX + winW - 1, winY + i); cout << "│"; }
    gotoxy(winX, winY); cout << "┌"; gotoxy(winX + winW - 1, winY); cout << "┐"; gotoxy(winX, winY + winH - 1); cout << "└"; gotoxy(winX + winW - 1, winY + winH - 1); cout << "┘";
    gotoxy(winX + 2, winY); cout << (isThemMoi ? "[ THEM DAU SACH MOI ]" : "[ HIEU CHINH DAU SACH ]");
    resetColor();

    char isbn[21], tenSach[101], tacGia[51], theLoai[31];
    int soTrang = 0, namXB = 2024;

    if (isThemMoi) {
        isbn[0] = '\0'; tenSach[0] = '\0'; tacGia[0] = '\0'; theLoai[0] = '\0';
    } else {
        strcpy(isbn, dsPtr->ISBN); strcpy(tenSach, dsPtr->tenSach); strcpy(tacGia, dsPtr->tacGia);
        strcpy(theLoai, dsPtr->theLoai); soTrang = dsPtr->soTrang; namXB = dsPtr->namXuatBan;
    }

    int currentField = 0;
    while (true) {
        setColor(37);
        gotoxy(winX + 2, winY + 3); cout << "ISBN         : ";
        gotoxy(winX + 2, winY + 4); cout << "Ten sach     : ";
        gotoxy(winX + 2, winY + 5); cout << "Tac gia      : ";
        gotoxy(winX + 2, winY + 6); cout << "The loai     : ";
        gotoxy(winX + 2, winY + 7); cout << "So trang     : ";
        gotoxy(winX + 2, winY + 8); cout << "Nam xuat ban : ";

        gotoxy(winX + 17, winY + 3); cout << string(55, ' '); gotoxy(winX + 17, winY + 3); cout << isbn;
        gotoxy(winX + 17, winY + 4); cout << string(55, ' '); gotoxy(winX + 17, winY + 4); cout << tenSach;
        gotoxy(winX + 17, winY + 5); cout << string(55, ' '); gotoxy(winX + 17, winY + 5); cout << tacGia;
        gotoxy(winX + 17, winY + 6); cout << string(55, ' '); gotoxy(winX + 17, winY + 6); cout << theLoai;
        gotoxy(winX + 17, winY + 7); cout << string(55, ' '); gotoxy(winX + 17, winY + 7); cout << soTrang;
        gotoxy(winX + 17, winY + 8); cout << string(55, ' '); gotoxy(winX + 17, winY + 8); cout << namXB;

        int status;
        switch (currentField) {
            case 0: status = isThemMoi ? nhapChuoiForm(winX + 17, winY + 3, isbn, 20) : INPUT_DOWN; break;
            case 1: status = nhapChuoiForm(winX + 17, winY + 4, tenSach, 55, false, true); break;
            case 2: status = nhapChuoiForm(winX + 17, winY + 5, tacGia, 50, false, true); break;
            case 3: status = nhapChuoiForm(winX + 17, winY + 6, theLoai, 30, false, true); break;
            case 4: status = nhapSoForm(winX + 17, winY + 7, soTrang); break;
            case 5: status = nhapSoForm(winX + 17, winY + 8, namXB); break;
        }

        if (status == INPUT_UP) currentField = (currentField - 1 + 6) % 6;
        else if (status == INPUT_DOWN) currentField = (currentField + 1) % 6;
        else if (status == INPUT_CANCEL) { gotoxy(winX + 2, winY + 10); cout << "Da huy thao tac."; return; }
        else if (status == INPUT_OK) {
            currentField = (currentField + 1);
            if (currentField >= 6) {
                if (strlen(isbn) == 0 || strlen(tenSach) == 0 || strlen(tacGia) == 0 || strlen(theLoai) == 0 || soTrang <= 0 || namXB <= 0) {
                    gotoxy(winX + 2, winY + 10); setColor(31); cout << "Vui long dien day du va hop le thong tin!"; resetColor(); _getch();
                    gotoxy(winX + 2, winY + 10); cout << string(winW - 4, ' '); currentField = 0; continue;
                }
                chuanHoaISBN(isbn);

                if (isThemMoi) {
                    if (timTheoISBN(ds, isbn) != nullptr) {
                        gotoxy(winX + 2, winY + 10); setColor(31); cout << "Loi: ISBN da ton tai!"; resetColor(); _getch();
                        gotoxy(winX + 2, winY + 10); cout << string(winW - 4, ' '); currentField = 0; continue;
                    }
                    DauSach* p = new DauSach;
                    strcpy(p->ISBN, isbn); strcpy(p->tenSach, tenSach); strcpy(p->tacGia, tacGia); strcpy(p->theLoai, theLoai);
                    p->soTrang = soTrang; p->namXuatBan = namXB; p->soLuotMuon = 0;
                    p->dsSach.pHead = p->dsSach.pTail = nullptr; p->dsSach.tongSoSach = 0;
                    capNhatTenSachSearch(p); capNhatTacGiaSearch(p);
                    int pos = timViTriChen(ds, p->tenSach);
                    for (int i = ds.n; i > pos; i--) ds.nodes[i] = ds.nodes[i - 1];
                    ds.nodes[pos] = p; ds.n++; insertHashISBN(ds, p);
                    gotoxy(winX + 2, winY + 10); setColor(32); cout << "Them dau sach thanh cong!";
                } else {
                    bool sachDangMuon = false;
                    Sach* temp = dsPtr->dsSach.pHead;
                    while (temp != nullptr) {
                        if (temp->trangThai == 1) { // 1 = Da muon
                            sachDangMuon = true;
                            break;
                        }
                        temp = temp->pNext;
                    }
                    if (sachDangMuon) {
                        gotoxy(winX + 2, winY + 10); setColor(31); cout << "Loi: Khong the sua vi co sach dang duoc muon!"; resetColor();
                        _getch();
                        gotoxy(winX + 2, winY + 10); cout << string(winW - 4, ' ');
                        currentField = 0;
                        continue;
                    }
                    int i; for (i = 0; i < ds.n; i++) if (ds.nodes[i] == dsPtr) break;
                    for (int j = i; j < ds.n - 1; j++) ds.nodes[j] = ds.nodes[j + 1];
                    ds.n--;
                    strcpy(dsPtr->tenSach, tenSach); strcpy(dsPtr->tacGia, tacGia); strcpy(dsPtr->theLoai, theLoai);
                    dsPtr->soTrang = soTrang; dsPtr->namXuatBan = namXB;
                    capNhatTenSachSearch(dsPtr); capNhatTacGiaSearch(dsPtr);
                    int pos = timViTriChen(ds, dsPtr->tenSach);
                    for (int j = ds.n; j > pos; j--) ds.nodes[j] = ds.nodes[j - 1];
                    ds.nodes[pos] = dsPtr; ds.n++;
                    gotoxy(winX + 2, winY + 10); setColor(32); cout << "Hieu chinh thanh cong!";
                }
                resetColor(); return;
            }
        }
    }
}

// --- FORM NHAP/SUA DOC GIA ---
void formNhapDocGia(QuanLyDocGia& ql, DocGia* dg, bool isThemMoi) {
    int winX = 28, winY = 4, winW = 50, winH = 10;
    xoaVung(winX, winY, winW, winH);
    setColor(33);
    for (int i = 0; i < winW; i++) { gotoxy(winX + i, winY); cout << "─"; gotoxy(winX + i, winY + winH - 1); cout << "─"; }
    for (int i = 0; i < winH; i++) { gotoxy(winX, winY + i); cout << "│"; gotoxy(winX + winW - 1, winY + i); cout << "│"; }
    gotoxy(winX, winY); cout << "┌"; gotoxy(winX + winW - 1, winY); cout << "┐"; gotoxy(winX, winY + winH - 1); cout << "└"; gotoxy(winX + winW - 1, winY + winH - 1); cout << "┘";
    gotoxy(winX + 2, winY); cout << (isThemMoi ? "[ THEM DOC GIA MOI ]" : "[ HIEU CHINH DOC GIA ]");
    resetColor();

    char ho[51], ten[21];
    int gioiTinh; // 1 for NAM, 2 for NU

    if (isThemMoi) {
        ho[0] = '\0';
        ten[0] = '\0';
        gioiTinh = 1;
    } else {
        strcpy(ho, dg->ho);
        strcpy(ten, dg->ten);
        gioiTinh = (strcmp(dg->giotinh, "NAM") == 0) ? 1 : 2;
    }

    int currentField = 0;
    while (true) {
        // Draw prompts
        setColor(37);
        gotoxy(winX + 2, winY + 3); cout << "Ho       : ";
        gotoxy(winX + 2, winY + 4); cout << "Ten      : ";
        gotoxy(winX + 2, winY + 5); cout << "Gioi tinh: ";

        // Draw values
        gotoxy(winX + 13, winY + 3); cout << string(30, ' '); gotoxy(winX + 13, winY + 3); cout << ho;
        gotoxy(winX + 13, winY + 4); cout << string(30, ' '); gotoxy(winX + 13, winY + 4); cout << ten;
        gotoxy(winX + 13, winY + 5); cout << string(30, ' '); gotoxy(winX + 13, winY + 5); cout << (gioiTinh == 1 ? "NAM" : "NU");
        if (currentField == 2) {
            setColor(36);
            cout << "  (<- / -> NAM <-> NU)";
            resetColor();
        }
        int status;
        switch (currentField) {
            case 0:
                status = nhapChuoiForm(winX + 13, winY + 3, ho, 35, false, true);
                break;
            case 1:
                status = nhapChuoiForm(winX + 13, winY + 4, ten, 20, false, true);
                break;
            case 2: {
                showCursor(true);
                gotoxy(winX + 13, winY + 5);
                int key;
                while (true) {
                    key = _getch();
                    if (key == 224 || key == 0) {
                        key = _getch();
                        if (key == 75 || key == 77) { // LEFT or RIGHT
                            gioiTinh = (gioiTinh == 1) ? 2 : 1;
                            gotoxy(winX + 13, winY + 5); cout << string(10, ' ');
                            gotoxy(winX + 13, winY + 5); cout << (gioiTinh == 1 ? "NAM" : "NU");
                        } else if (key == 72) { status = INPUT_UP; break; }
                        else if (key == 80) { status = INPUT_DOWN; break; }
                    } else if (key == 13) { status = INPUT_OK; break; }
                    else if (key == 27) { status = INPUT_CANCEL; break; }
                }
                showCursor(false);
                break;
            }
        }

        if (status == INPUT_UP) currentField = (currentField - 1 + 3) % 3;
        else if (status == INPUT_DOWN) currentField = (currentField + 1) % 3;
        else if (status == INPUT_CANCEL) {
            gotoxy(winX + 2, winY + 7); cout << "Da huy thao tac.";
            return;
        } else if (status == INPUT_OK) {
            currentField = (currentField + 1);
            if (currentField >= 3) { // Reached end of form, process save
                if (strlen(ho) == 0 || strlen(ten) == 0) {
                    gotoxy(winX + 2, winY + 7); setColor(31); cout << "Ho va Ten khong duoc de trong!"; resetColor();
                    currentField = (strlen(ho) == 0) ? 0 : 1; // Focus on the empty field
                    _getch();
                    gotoxy(winX + 2, winY + 7); cout << string(winW - 4, ' ');
                    continue;
                }

                if (isThemMoi) {
                    int maThe = taoMaTheMoi();
                    if (maThe == -1) {
                        gotoxy(winX + 2, winY + 7); setColor(31); cout << "Loi: Kho ma the da can kiet!";
                        resetColor();
                        return;
                    }
                    DocGia* newDg = taoDocGia(maThe);
                    strcpy(newDg->ho, ho);
                    strcpy(newDg->ten, ten);
                    strcpy(newDg->giotinh, (gioiTinh == 1 ? "NAM" : "NU"));
                    if (themDocGia(ql, newDg)) {
                        gotoxy(winX + 2, winY + 7); setColor(32); cout << "Them doc gia thanh cong!";
                    } else {
                        gotoxy(winX + 2, winY + 7); setColor(31); cout << "Them that bai!";
                    }
                } else {
                    strcpy(dg->ho, ho);
                    strcpy(dg->ten, ten);
                    strcpy(dg->giotinh, (gioiTinh == 1 ? "NAM" : "NU"));
                    gotoxy(winX + 2, winY + 7); setColor(32); cout << "Hieu chinh thanh cong!";
                }
                resetColor();
                return;
            }
        }
    }
}

// --- CHUC NANG QUAN LY SACH ---
void quanLySachUI(ListDauSach& ds) {
    int luaChon = 0;
    const int ITEM_PER_PAGE = 15;
    char searchKeyword[100] = "";
    int vtSearch = 0;
    bool searchMode = false;
    bool isTypingSearch = false;

    system("cls");

    while (true) {
        DauSach** displayArr = new DauSach*[ds.n];
        int n = 0;

        if (searchMode && strlen(searchKeyword) > 0) {
            string keywordLower = searchKeyword;
            transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), [](unsigned char c){ return tolower(c); });
            
            string keywordUpper = searchKeyword;
            transform(keywordUpper.begin(), keywordUpper.end(), keywordUpper.begin(), [](unsigned char c){ return toupper(c); });
            for (int i = 0; i < ds.n; i++) {
                if (strstr(ds.nodes[i]->tenSachSearch, keywordLower.c_str()) != nullptr ||
                    strstr(ds.nodes[i]->tacGiaSearch, keywordLower.c_str()) != nullptr ||
                    strstr(ds.nodes[i]->ISBN, keywordUpper.c_str()) != nullptr) {
                    displayArr[n++] = ds.nodes[i];
                }
            }
        } else {
            for (int i = 0; i < ds.n; i++) {
                displayArr[n++] = ds.nodes[i];
            }
        }

        gotoxy(0, 0); // Ve de len ma khong dung cls chong giat

        if (n == 0) {
            setColor(33); cout << "--- QUAN LY DAU SACH ---" << string(50, ' ') << "\n\n"; resetColor();
            if (isTypingSearch || searchMode) {
                cout << "Tim kiem: " << searchKeyword << (isTypingSearch ? "_" : "") << string(40, ' ') << "\n";
            } else {
                cout << string(80, ' ') << "\n";
            }
            cout << "Khong tim thay dau sach nao khop voi '" << searchKeyword << "'!" << string(30, ' ') << "\n";
            for(int i=0; i<18; ++i) cout << string(80, ' ') << "\n"; 
        } else {
            if (luaChon >= n) luaChon = max(0, n - 1);
            int tongTrang = (n > 0) ? ((n - 1) / ITEM_PER_PAGE + 1) : 1;
            int trang = luaChon / ITEM_PER_PAGE;

            setColor(33); 
            if(searchMode) cout << "--- TIM KIEM: '" << searchKeyword << "' (" << n << " ket qua) - Trang " << trang + 1 << "/" << tongTrang << " ---" << string(15, ' ') << "\n\n";
            else cout << "--- QUAN LY DAU SACH (Trang " << trang + 1 << "/" << tongTrang << ") ---" << string(30, ' ') << "\n\n";
            resetColor();

            if (isTypingSearch || searchMode) {
                cout << "Tim kiem: " << searchKeyword << (isTypingSearch ? "_" : "") << string(40, ' ') << "\n";
            } else {
                cout << string(80, ' ') << "\n";
            }
            
            cout << "   ┌" << string(17, '-') << "┬" << string(32, '-') << "┬" << string(22, '-') << "┬" << string(17, '-') << "┐   " << endl;
            cout << "   │ " << left << setw(15) << "ISBN" << " │ " << left << setw(30) << "Ten Sach" << " │ " << left << setw(20) << "Tac Gia" << " │ " << left << setw(15) << "The Loai" << " │   " << endl;
            cout << "   ├" << string(17, '-') << "┼" << string(32, '-') << "┼" << string(22, '-') << "┼" << string(17, '-') << "┤   " << endl;

            int start = trang * ITEM_PER_PAGE;
            int end = min(start + ITEM_PER_PAGE, n);

            for (int i = 0; i < ITEM_PER_PAGE; i++) {
                if (start + i < end) {
                    int idx = start + i;
                    if (idx == luaChon) { setColor(47); cout << ">> "; } 
                    else { cout << "   "; }

                    string tenSach = displayArr[idx]->tenSach; if (tenSach.length() > 30) tenSach = tenSach.substr(0, 27) + "...";
                    string tacGia = displayArr[idx]->tacGia; if (tacGia.length() > 20) tacGia = tacGia.substr(0, 17) + "...";

                    cout << "│ " << left << setw(15) << displayArr[idx]->ISBN << " │ " << left << setw(30) << tenSach << " │ " << left << setw(20) << tacGia << " │ " << left << setw(15) << displayArr[idx]->theLoai << " │   ";
                    
                    if (idx == luaChon) resetColor();
                    cout << endl;
                } else {
                    cout << "   │ " << string(15, ' ') << " │ " << string(30, ' ') << " │ " << string(20, ' ') << " │ " << string(15, ' ') << " │   \n";
                }
            }
            cout << "   └" << string(17, '-') << "┴" << string(32, '-') << "┴" << string(22, '-') << "┴" << string(17, '-') << "┘   " << endl;
        }
        
        setColor(36);
        cout << "\n   (T) Them | (S) Sua | (X) Xoa | (C) Cuon sach | (F) Tim | (I) In DS | Mui ten, PgUp/Dn | ESC: Quay lai\n";
        resetColor();
        
        for(int i=0; i<3; ++i) cout << string(80, ' ') << "\n";

        int key = _getch();
        bool actionTaken = false;

        if (isTypingSearch) {
            if (key == 27) { 
                isTypingSearch = false;
            } else if (key == 13) { 
                isTypingSearch = false;
            } else if (key == 8) { 
                if (vtSearch > 0) {
                    vtSearch--; searchKeyword[vtSearch] = '\0'; searchMode = (vtSearch > 0); luaChon = 0;
                } else { searchMode = false; }
            } else if (key == 224 || key == 0) { 
                key = _getch();
                if (key == 72) luaChon = max(0, luaChon - 1);
                else if (key == 80) luaChon = min(n - 1, luaChon + 1);
                else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE);
                else if (key == 81) luaChon = min(n - 1, luaChon + ITEM_PER_PAGE);
            } else if (isprint(key) && vtSearch < 99) {
                searchKeyword[vtSearch++] = (char)key; searchKeyword[vtSearch] = '\0'; searchMode = true; luaChon = 0;
            }
        } else {
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 72) luaChon = max(0, luaChon - 1);
                else if (key == 80) luaChon = min(n - 1, luaChon + 1);
                else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE);
                else if (key == 81) luaChon = min(n - 1, luaChon + ITEM_PER_PAGE);
                else if (key == 83) { // DELETE key
                    int px = 15, py = 10, pw = 55, ph = 5;
                    xoaVung(px, py, pw, ph);
                    setColor(33);
                    for(int i=0; i<pw; i++) { gotoxy(px+i, py); cout << "─"; gotoxy(px+i, py+ph-1); cout << "─"; }
                    for(int i=0; i<ph; i++) { gotoxy(px, py+i); cout << "│"; gotoxy(px+pw-1, py+i); cout << "│"; }
                    gotoxy(px, py); cout << "┌"; gotoxy(px+pw-1, py); cout << "┐"; gotoxy(px, py+ph-1); cout << "└"; gotoxy(px+pw-1, py+ph-1); cout << "┘";
                    gotoxy(px + 2, py + 2);
                    cout << "Xoa dau sach ISBN: " << displayArr[luaChon]->ISBN << "? (Y/N): ";
                    char confirm = _getch();
                    if (toupper(confirm) == 'Y') {
                        gotoxy(px + 2, py + 2); cout << string(pw - 4, ' ');
                        gotoxy(0, 24); 
                        if (xoaDauSach(ds, displayArr[luaChon]->ISBN)) {
                            gotoxy(px + 2, py + 2); setColor(32); cout << "Xoa thanh cong!"; resetColor();
                        } else {
                            gotoxy(px + 2, py + 2); setColor(31); cout << "Xoa that bai (sach dang muon)!"; resetColor();
                        }
                    } else {
                        gotoxy(px + 2, py + 2); cout << string(pw - 4, ' ');
                        gotoxy(px + 2, py + 2); cout << "Da huy thao tac xoa.";
                    }
                    Sleep(1000);
                }
            } 
            else if (key == 27) { delete[] displayArr; return; }
            else if (key == 't' || key == 'T') { formNhapDauSach(ds, nullptr, true); actionTaken = true; }
            else if (key == 's' || key == 'S') { formNhapDauSach(ds, displayArr[luaChon], false); actionTaken = true; }
            else if (key == 'x' || key == 'X') { 
                ungetch(83); 
                ungetch(224); 
            }
            else if (key == 'c' || key == 'C') {
                while (true) {
                    system("cls");
                    inDanhSachCuonSach(displayArr[luaChon]); 
                    setColor(36);
                    cout << "\n\n   (T) Them cuon sach | (X) Thanh ly | ESC: Quay lai\n";
                    resetColor();
                    int k = _getch();
                    if (k == 't' || k == 'T') {
                        system("cls"); showCursor(true);
                        cout << "--- THEM CUON SACH ---\n";
                        cout << "Dau sach: " << displayArr[luaChon]->tenSach << "\n";
                        themCuonSach(displayArr[luaChon]);
                        showCursor(false);
                        actionTaken = true;
                    }
                    else if (k == 'x' || k == 'X') {
                        char suffix[10];
                        char prompt[100];
                        snprintf(prompt, sizeof(prompt), "Nhap Ma Sach can thanh ly: %s_", displayArr[luaChon]->ISBN);
                        cout << "\n";
                        showCursor(true);
                        if (nhapChuoiTuDo(prompt, suffix, 10)) {
                            showCursor(false);
                            char maSach[30];
                            bool isNumber = true;
                            for (int i = 0; suffix[i] != '\0'; i++) if (!isdigit(suffix[i])) isNumber = false;
                            if (isNumber && strlen(suffix) > 0) {
                                snprintf(maSach, sizeof(maSach), "%s_%04d", displayArr[luaChon]->ISBN, atoi(suffix));
                            } else {
                                snprintf(maSach, sizeof(maSach), "%s_%s", displayArr[luaChon]->ISBN, suffix);
                            }
                            Sach* current = displayArr[luaChon]->dsSach.pHead;
                            bool found = false;
                            while (current != nullptr) {
                                if (strcmp(current->maSach, maSach) == 0) {
                                    found = true;
                                    if (current->trangThai == 1) {
                                        setColor(31); cout << "Khong the thanh ly. Sach dang duoc muon!\n"; resetColor();
                                    } else if (current->trangThai == 2) {
                                        setColor(33); cout << "Sach nay da thanh ly roi!\n"; resetColor();
                                    } else {
                                        current->trangThai = 2; // Cap nhat thanh ly
                                        setColor(32); cout << "Da thanh ly sach thanh cong!\n"; resetColor();
                                    }
                                    break;
                                }
                                current = current->pNext;
                            }
                            if (!found) { setColor(31); cout << "Khong tim thay Ma Sach: " << maSach << "\n"; resetColor(); }
                            cout << "Nhan phim bat ky de tiep tuc...";
                            _getch();
                        } else {
                            showCursor(false);
                        }
                    }
                    else if (k == 27) { break; }
                }
                actionTaken = true;
            }
            else if (key == 'f' || key == 'F') { 
                isTypingSearch = true; 
                searchMode = true; 
            }
            else if (key == 'i' || key == 'I') {
                system("cls"); showCursor(true); inTheoTheLoai_TrongDoTenTangDan(ds); actionTaken = true;
            }
        }
        
        if (actionTaken) { 
            cout << "\nNhan phim bat ky de tiep tuc..."; 
            int k = _getch(); 
            if (k == 0 || k == 224) _getch(); // Xoá sạch byte rác trong bộ nhớ đệm nếu người dùng lỡ bấm mũi tên
            system("cls");
        }
        
        delete[] displayArr;
    }
}

// --- CHUC NANG QUAN LY DOC GIA ---
void quanLyDocGiaUI(QuanLyDocGia& ql) {
    int luaChon = 0;
    const int ITEM_PER_PAGE = 15;
    char searchKeyword[100] = "";
    int vtSearch = 0;
    bool searchMode = false;
    bool isTypingSearch = false;

    system("cls");

    while (true) {
        DocGia** displayArr = new DocGia*[ql.soLuongDocGia];
        int n = 0;

        if (searchMode && strlen(searchKeyword) > 0) {
            DocGia** allReaders = new DocGia*[ql.soLuongDocGia];
            int total_n = 0;
            BSTtoArray(ql.root, allReaders, total_n);
    
            string keywordLower = searchKeyword;
            transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(),
                [](unsigned char c){ return tolower(c); });

            for (int i = 0; i < total_n; i++) {
                string hoTen = string(allReaders[i]->ho) + " " + string(allReaders[i]->ten);
                string hoTenLower = hoTen;
                transform(hoTenLower.begin(), hoTenLower.end(), hoTenLower.begin(), 
                    [](unsigned char c){ return tolower(c); }); 
                
                if (hoTenLower.find(keywordLower) != string::npos || to_string(allReaders[i]->maThe).find(keywordLower) != string::npos) {
                    displayArr[n++] = allReaders[i];
                }
            }
            delete[] allReaders;
        } else {
            BSTtoArray(ql.root, displayArr, n);
        }

        gotoxy(0, 0); // Ve de ma khong xoa manh chong giat

        if (n == 0) {
            setColor(33); cout << "--- QUAN LY DOC GIA ---" << string(50, ' ') << "\n\n"; resetColor();
            if (isTypingSearch || searchMode) {
                cout << "Tim kiem: " << searchKeyword << (isTypingSearch ? "_" : "") << string(40, ' ') << "\n";
            } else {
                cout << string(80, ' ') << "\n";
            }
            cout << "Khong tim thay doc gia nao khop voi '" << searchKeyword << "'!" << string(30, ' ') << "\n";
            for(int i=0; i<18; ++i) cout << string(80, ' ') << "\n"; 
        } else {
            if (luaChon >= n) luaChon = max(0, n - 1);
            int tongTrang = (n > 0) ? ((n - 1) / ITEM_PER_PAGE + 1) : 1;
            int trang = luaChon / ITEM_PER_PAGE;

            setColor(33); 
            if(searchMode) cout << "--- TIM KIEM: '" << searchKeyword << "' (" << n << " ket qua) - Trang " << trang + 1 << "/" << tongTrang << " ---" << string(15, ' ') << "\n\n";
            else cout << "--- QUAN LY DOC GIA (Trang " << trang + 1 << "/" << tongTrang << ") ---" << string(30, ' ') << "\n\n";
            resetColor();

            if (isTypingSearch || searchMode) {
                cout << "Tim kiem: " << searchKeyword << (isTypingSearch ? "_" : "") << string(40, ' ') << "\n";
            } else {
                cout << string(80, ' ') << "\n";
            }
            
            cout << "   ┌" << string(8, '-') << "┬" << string(22, '-') << "┬" << string(12, '-') << "┬" << string(12, '-') << "┬" << string(17, '-') << "┐   " << endl;
            cout << "   │ " << left << setw(6) << "Ma The" << " │ " << left << setw(20) << "Ho" << " │ " << left << setw(10) << "Ten" << " │ " << left << setw(10) << "Gioi Tinh" << " │ " << left << setw(15) << "Trang Thai" << " │   " << endl;
            cout << "   ├" << string(8, '-') << "┼" << string(22, '-') << "┼" << string(12, '-') << "┼" << string(12, '-') << "┼" << string(17, '-') << "┤   " << endl;

            int start = trang * ITEM_PER_PAGE;
            int end = min(start + ITEM_PER_PAGE, n);

            for (int i = 0; i < ITEM_PER_PAGE; i++) {
                if (start + i < end) {
                    int idx = start + i;
                    if (idx == luaChon) { setColor(47); cout << ">> "; } 
                    else { cout << "   "; }

                    string ho = displayArr[idx]->ho;
                    if (ho.length() > 20) ho = ho.substr(0, 17) + "...";

                    cout << "│ " << left << setw(6) << displayArr[idx]->maThe << " │ " << left << setw(20) << ho << " │ " << left << setw(10) << displayArr[idx]->ten << " │ " << left << setw(10) << displayArr[idx]->giotinh << " │ " << left << setw(15) << (displayArr[idx]->trangThaiThe == 1 ? "Hoat dong" : "Khoa") << " │   ";
                    
                    if (idx == luaChon) resetColor();
                    cout << endl;
                } else {
                    cout << "   │ " << string(6, ' ') << " │ " << string(20, ' ') << " │ " << string(10, ' ') << " │ " << string(10, ' ') << " │ " << string(15, ' ') << " │   \n";
                }
            }

            cout << "   └" << string(8, '-') << "┴" << string(22, '-') << "┴" << string(12, '-') << "┴" << string(12, '-') << "┴" << string(17, '-') << "┘   " << endl;
        }
        
        setColor(36);
        cout << "\n   (T) Them | (S) Sua | (X) Xoa | (K) Khoa/Mo | (F) Tim | (I) In DS | Mui ten, PgUp/Dn | ESC: Quay lai\n";
        resetColor();
        
        for(int i=0; i<3; ++i) cout << string(80, ' ') << "\n";

        int key = _getch();
        bool actionTaken = false;

        if (isTypingSearch) {
            if (key == 27) { 
                isTypingSearch = false;
            } else if (key == 13) { 
                isTypingSearch = false;
            } else if (key == 8) { 
                if (vtSearch > 0) {
                    vtSearch--; searchKeyword[vtSearch] = '\0'; searchMode = (vtSearch > 0); luaChon = 0;
                } else { searchMode = false; }
            } else if (key == 224 || key == 0) { 
                key = _getch();
                if (key == 72) luaChon = max(0, luaChon - 1);
                else if (key == 80) luaChon = min(n - 1, luaChon + 1);
                else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE);
                else if (key == 81) luaChon = min(n - 1, luaChon + ITEM_PER_PAGE);
            } else if (isprint(key) && vtSearch < 99) { // isprint la ham check xem co phai la ky tu in duoc khong, tranh truong hop nguoi dung bam cac phim dieu khien hoac phim khong hop le
                searchKeyword[vtSearch++] = (char)key; searchKeyword[vtSearch] = '\0'; searchMode = true; luaChon = 0;
            }
        } else {
            if (key == 0 || key == 224) { 
                key = _getch();
                if (key == 72) luaChon = max(0, luaChon - 1);
                else if (key == 80) luaChon = min(n - 1, luaChon + 1);
                else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE);
                else if (key == 81) luaChon = min(n - 1, luaChon + ITEM_PER_PAGE);
                else if (key == 83) { // DELETE key
                    int px = 20, py = 10, pw = 45, ph = 5;
                    xoaVung(px, py, pw, ph);
                    setColor(33);
                    for(int i=0; i<pw; i++) { gotoxy(px+i, py); cout << "─"; gotoxy(px+i, py+ph-1); cout << "─"; }
                    for(int i=0; i<ph; i++) { gotoxy(px, py+i); cout << "│"; gotoxy(px+pw-1, py+i); cout << "│"; }
                    gotoxy(px, py); cout << "┌"; gotoxy(px+pw-1, py); cout << "┐"; gotoxy(px, py+ph-1); cout << "└"; gotoxy(px+pw-1, py+ph-1); cout << "┘";
                    gotoxy(px + 2, py + 2);
                    cout << "Xoa doc gia " << displayArr[luaChon]->maThe << "? (Y/N): ";
                    char confirm = _getch();
                    if (toupper(confirm) == 'Y') {
                        gotoxy(px + 2, py + 2); cout << string(pw - 4, ' ');
                        gotoxy(0, 24);
                        if (xoaDocGia(ql, displayArr[luaChon]->maThe)) {
                            gotoxy(px + 2, py + 2); setColor(32); cout << "Xoa thanh cong!"; resetColor();
                        } else {
                            gotoxy(px + 2, py + 2); setColor(31); cout << "Xoa that bai!"; resetColor();
                        }
                    } else {
                        gotoxy(px + 2, py + 2); cout << string(pw - 4, ' ');
                        gotoxy(px + 2, py + 2); cout << "Da huy thao tac xoa.";
                    }
                    Sleep(1000);
                }
            } 
            else if (key == 27) { delete[] displayArr; return; }
            else if (key == 't' || key == 'T') { 
                formNhapDocGia(ql, nullptr, true);
                actionTaken = true;
            }
            else if (key == 's' || key == 'S') { 
                formNhapDocGia(ql, displayArr[luaChon], false);
                actionTaken = true; 
            }
            else if (key == 'x' || key == 'X') { 
                ungetch(83); ungetch(224); // Đẩy phím giả lập DELETE
            }
            else if (key == 'k' || key == 'K') { 
                int ma = displayArr[luaChon]->maThe;
                DocGia* dg = timDocGia(ql.root, ma);
                if (dg) {
                    dg->trangThaiThe = (dg->trangThaiThe == 1) ? 0 : 1;
                    
                    int px = 25, py = 10, pw = 36, ph = 5;
                    xoaVung(px, py, pw, ph);
                    setColor(33);
                    for(int i=0; i<pw; i++) { gotoxy(px+i, py); cout << "─"; gotoxy(px+i, py+ph-1); cout << "─"; }
                    for(int i=0; i<ph; i++) { gotoxy(px, py+i); cout << "│"; gotoxy(px+pw-1, py+i); cout << "│"; }
                    gotoxy(px, py); cout << "┌"; gotoxy(px+pw-1, py); cout << "┐"; gotoxy(px, py+ph-1); cout << "└"; gotoxy(px+pw-1, py+ph-1); cout << "┘";
                    
                    gotoxy(px + 2, py + 2);
                    setColor(32);
                    cout << "Trang thai the: " << (dg->trangThaiThe == 1 ? "Hoat dong" : "Khoa ");
                    resetColor();
                    Sleep(800); // Dừng màn hình 0.8 giây rồi tự động update lại thay vì phải bấm nút
                }
            }
            else if (key == 'f' || key == 'F') { 
                isTypingSearch = true; 
                searchMode = true; 
            }
            else if (key == 'i' || key == 'I') { const char* subInDocGia[] = { "In theo Ma The (tang dan)", "In theo Ten (A-Z)", "<- Quay lai" }; int indexIn = 0; system("cls"); gotoxy(10, 4); setColor(33); cout << "--- CHON KIEU IN ---"; int chonIn = chonMenu(subInDocGia, 3, 10, 7, indexIn); if (chonIn != 2 && chonIn != -1) { system("cls"); showCursor(true); if (chonIn == 0) inDanhSachDocGia(ql.root); else if (chonIn == 1) inTheoTen(ql); actionTaken = true; } }
        }
        
        if (actionTaken) { 
            showCursor(false);
            cout << "\nNhan phim bat ky de tiep tuc..."; 
            int k = _getch(); 
            if (k == 0 || k == 224) _getch(); // Dọn bộ nhớ đệm
            system("cls");
        }
        
        delete[] displayArr;
    }
}

// --- CHUC NANG GIAO DICH (MUON/TRA/MAT) ---
// ESC tai moi buoc chi lui ve buoc truoc do (khong nhay thang ra menu Giao dich):
//   chon cuon sach  --ESC--> chon dau sach
//   chon dau sach   --ESC--> chon doc gia
//   chon sach tra/mat --ESC--> chon doc gia
//   chon doc gia    --ESC--> menu Giao dich
void giaoDichUI(QuanLyDocGia& qlDocGia, ListDauSach& ds) {
    const char* subGiaoDich[] = { "Muon sach", "Tra sach", "Bao mat sach", "Xem sach dang muon", "<- Quay lai" };
    int indexPhai = 0;

    while (true) {
        veKhungGiaoDien();
        veMenuList(menuTrai, 6, 3, 2, 6, false);
        xoaVung(28, 4, 50, 18);
        gotoxy(40, 4); setColor(33); cout << "--- GIAO DICH ---";

        int chonPhai = chonMenu(subGiaoDich, 5, 35, 7, indexPhai);
        if (chonPhai == 4 || chonPhai == -1) return;

        // Vong lap chon doc gia: ESC tai day moi quay ve menu Giao dich
        while (true) {
            system("cls");
            int ma = chonTuBangDocGia(qlDocGia);
            if (ma == -1) break; // ESC -> ve menu Giao dich

            DocGia* dg = timDocGia(qlDocGia.root, ma);
            if (dg == nullptr) {
                cout << "Loi: Khong tim thay doc gia!\n";
                cout << "\n\nNhan phim bat ky de quay lai...";
                _getch();
                continue; // quay lai chon doc gia
            }

            showCursor(false);

            if (chonPhai == 3) { // Xem sach dang muon
                system("cls");
                setColor(33); cout << "=== DOC GIA: " << dg->ho << " " << dg->ten << " ===\n"; resetColor();
                xemSachDangMuon(dg, ds);
                cout << "------------------------------------------\n";
                cout << "\nNhan phim bat ky de quay lai...";
                _getch();
                continue;
            }

            if (chonPhai == 0) { // Muon sach
                if (dg->trangThaiThe == 0) {
                    system("cls");
                    setColor(33); cout << "=== DOC GIA: " << dg->ho << " " << dg->ten << " ===\n"; resetColor();
                    xemSachDangMuon(dg, ds);
                    cout << "------------------------------------------\n";
                    cout << "The dang bi KHOA!\n";
                    cout << "\n\nNhan phim bat ky de tiep tuc...";
                    _getch();
                    continue;
                }
                if (dg->soSachDangMuon >= 3) {
                    system("cls");
                    setColor(33); cout << "=== DOC GIA: " << dg->ho << " " << dg->ten << " ===\n"; resetColor();
                    xemSachDangMuon(dg, ds);
                    cout << "------------------------------------------\n";
                    cout << "Da muon toi da 3 cuon!\n";
                    cout << "\n\nNhan phim bat ky de tiep tuc...";
                    _getch();
                    continue;
                }

                if (coSachQuaHan(dg)) {
                    system("cls");
                    setColor(33); cout << "=== DOC GIA: " << dg->ho << " " << dg->ten << " ===\n"; resetColor();
                    xemSachDangMuon(dg, ds);
                    cout << "------------------------------------------\n";
                    setColor(31); cout << "Doc gia dang co sach muon QUA HAN! Khong the muon them.\n"; resetColor();
                    cout << "\n\nNhan phim bat ky de tiep tuc...";
                    _getch();
                    continue;
                }

                // Vong lap chon dau sach: ESC tai day quay ve chon doc gia
                bool xongMuon = false;
                while (!xongMuon) {
                    DauSach* dauSachChon = chonTuBangDauSach(ds, dg);
                    if (dauSachChon == nullptr) break; // ESC -> ve chon doc gia

                    // Vong lap chon cuon sach: ESC tai day quay ve chon dau sach
                    while (true) {
                        Sach* cuonSachChon = chonCuonSachTuDauSach(dauSachChon, dg, &ds);
                        if (cuonSachChon == nullptr) break; // ESC -> ve chon dau sach

                        system("cls");
                        setColor(33); cout << "=== DOC GIA: " << dg->ho << " " << dg->ten << " ===\n"; resetColor();
                        muonSach(dg, ds, cuonSachChon->maSach);
                        cout << "\n\nNhan phim bat ky de tiep tuc...";
                        _getch();
                        xongMuon = true; // thanh cong -> thoat ca 2 vong, ve chon doc gia
                        break;
                    }
                }
                continue;
            }

            if (chonPhai == 1) { // Tra sach
                if (dg->soSachDangMuon == 0) {
                    system("cls");
                    setColor(33); cout << "=== DOC GIA: " << dg->ho << " " << dg->ten << " ===\n"; resetColor();
                    cout << "Doc gia nay khong muon sach nao de tra.\n";
                    cout << "\n\nNhan phim bat ky de tiep tuc...";
                    _getch();
                    continue;
                }

                // Vong lap chon sach can tra: ESC tai day quay ve chon doc gia
                while (true) {
                    const char* maSachTra = chonSachDangMuonUI(dg, ds);
                    if (maSachTra == nullptr) break; // ESC -> ve chon doc gia

                    system("cls");
                    setColor(33); cout << "=== DOC GIA: " << dg->ho << " " << dg->ten << " ===\n"; resetColor();
                    traSach(dg, ds, maSachTra, layNgayHienTai());
                    cout << "\n\nNhan phim bat ky de tiep tuc...";
                    _getch();
                    break; // thanh cong -> ve chon doc gia
                }
                continue;
            }

            if (chonPhai == 2) { // Bao mat sach
                if (dg->soSachDangMuon == 0) {
                    system("cls");
                    setColor(33); cout << "=== DOC GIA: " << dg->ho << " " << dg->ten << " ===\n"; resetColor();
                    cout << "Doc gia nay khong muon sach nao de bao mat.\n";
                    cout << "\n\nNhan phim bat ky de tiep tuc...";
                    _getch();
                    continue;
                }

                while (true) {
                    const char* maSachMat = chonSachDangMuonUI(dg, ds);
                    if (maSachMat == nullptr) break; // ESC -> ve chon doc gia

                    system("cls");
                    setColor(33); cout << "=== DOC GIA: " << dg->ho << " " << dg->ten << " ===\n"; resetColor();
                    baoMatSach(dg, ds, maSachMat);
                    cout << "\n\nNhan phim bat ky de tiep tuc...";
                    _getch();
                    break;
                }
                continue;
            }
        }
    }
}

void thongKeUI(QuanLyDocGia& qlDocGia, ListDauSach& ds) {
    int luaChon = 0;

    while (true) {
        system("cls");
        showCursor(false);
        setColor(33);
        cout << "\n======================== THONG KE ========================\n";
        resetColor();

        if (luaChon == 0) lietKeDocGiaQuaHan(qlDocGia);
        else if (luaChon == 1) inTop10SachMuonNhieu(ds);
        else if (luaChon == 2) thongKeSoLuongTheoTheLoai(ds);

        cout << "\n==========================================================\n";
        
        for (int i = 0; i < 3; i++) {
            if (i == luaChon) { setColor(47); cout << " >> "; }
            else { cout << "    "; }

            if (i == 0) cout << "1. Doc gia qua han";
            else if (i == 1) cout << "2. Top 10 sach muon";
            else if (i == 2) cout << "3. Theo the loai";
            
            if (i == luaChon) resetColor();
        }
        
        setColor(36);
        cout << "\n\n   (Mui ten Trai/Phai hoac Phim 1, 2, 3 de chuyen | ESC: Quay lai)\n";
        resetColor();

        int key = _getch();
        if (key == 224 || key == 0) {
            key = _getch();
            if (key == 75) luaChon = (luaChon - 1 + 3) % 3; // LEFT  
            else if (key == 77) luaChon = (luaChon + 1) % 3; // RIGHT
        } else if (key == '1') {
            luaChon = 0;
        } else if (key == '2') {
            luaChon = 1;
        } else if (key == '3') {
            luaChon = 2;
        } else if (key == 27) { // ESC
            return;
        }
    }
}

// --- CHUONG TRINH CHINH ---
void runMenu() {
    setupConsole();
    QuanLyDocGia qlDocGia;
    ListDauSach ds; ds.n = 0;
    
    loadDauSach("Input_file/DauSach.txt", ds);
    loadDocGia("Input_file/DocGia.txt", qlDocGia);
    khoiTaoKhoMaThe();

    int indexTrai = 0;
    while (true) {
        veKhungGiaoDien(); // Ve lai toan bo khung de xoa sach UI cua trang truoc

        if (indexTrai == 0) {
            int sm = 0, qh = 0;
            demThongKeTongQuan(qlDocGia.root, sm, qh);
            hienThiTongQuan(ds.n, qlDocGia.soLuongDocGia, sm, qh);
        }

        // PHIM TAT BI MAT DE TAO DU LIEU GIA
        if (indexTrai == 0) {
            setColor(36);
            gotoxy(30, 20); cout << "(Nhan 'G' de tao 50 doc gia mau)";
            resetColor();
        }

        int chonChinh = chonMenu(menuTrai, 6, 2, 6, indexTrai);
        if (chonChinh == 5 || chonChinh == -1) {
            system("cls");
            saveDocGia("Input_file/DocGia.txt", qlDocGia.root);
            saveDauSach("Input_file/DauSach.txt", ds);
            luuKhoMaThe();
            cout << "Da luu du lieu. Tam biet!\n";
            
            break;
        }

        if (chonChinh == -2) { // Ma bi mat cho phim 'G'
            system("cls");
            taoDuLieuGia(qlDocGia, ds, 50);
            cout << "\nNhan phim bat ky de tiep tuc...";
            _getch();
            continue;
        }
        switch (chonChinh) {
            case 0: continue; // Tong quan
            case 1: quanLyDocGiaUI(qlDocGia); indexTrai = 0; break;
            case 2: quanLySachUI(ds); indexTrai = 0; break;
            case 3: giaoDichUI(qlDocGia, ds); indexTrai = 0; break;
            case 4: thongKeUI(qlDocGia, ds); indexTrai = 0; break;
        }
    }
}
