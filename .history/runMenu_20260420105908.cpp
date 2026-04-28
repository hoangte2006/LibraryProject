#include <iostream>
#include <conio.h>
#include <string>
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

using namespace std;

// --- CAC HAM DO HOA (Giu nguyen de dam bao giao dien dep) ---
void gotoxy(int x, int y) {
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Cursor_an_Pos = { (short)x, (short)y };
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void setColor(int color) { cout << "\033[1;" << color << "m"; }
void resetColor() { cout << "\033[0m"; }
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
        cout << "Danh sach doc gia rong!\n";
        return -1;
    }

    DocGia** arr = new DocGia*[ql.soLuongDocGia];
    int n = 0;
    BSTtoArray(ql.root, arr, n);

    int luaChon = 0;
    int trang = 0;
    const int ITEM_PER_PAGE = 10;
    int tongTrang = (n > 0) ? ((n - 1) / ITEM_PER_PAGE + 1) : 1;

    while (true) {
        system("cls");
        cout << "--- CHON DOC GIA (Trang " << trang + 1 << "/" << tongTrang << ") ---\n";
        cout << "   ┌" << string(12, '-') << "┬" << string(27, '-') << "┬" << string(12, '-') << "┬" << string(17, '-') << "┐" << endl;
        cout << "   │ " << left << setw(10) << "Ma The" << " │ " << left << setw(25) << "Ho Ten" << " │ " << left << setw(10) << "Gioi Tinh" << " │ " << left << setw(15) << "Trang Thai" << " │" << endl;
        cout << "   ├" << string(12, '-') << "┼" << string(27, '-') << "┼" << string(12, '-') << "┼" << string(17, '-') << "┤" << endl;

        int start = trang * ITEM_PER_PAGE;
        int end = min(start + ITEM_PER_PAGE, n);

        for (int i = start; i < end; i++) {
            if (i == luaChon) { setColor(47); cout << ">> "; } 
            else { cout << "   "; }

            string hoTen = string(arr[i]->ho) + " " + string(arr[i]->ten);
            if (hoTen.length() > 25) hoTen = hoTen.substr(0, 22) + "...";

            cout << "│ " << left << setw(10) << arr[i]->maThe << " │ " << left << setw(25) << hoTen << " │ " << left << setw(10) << arr[i]->giotinh << " │ " << left << setw(15) << (arr[i]->trangThaiThe == 1 ? "Hoat dong" : "Khoa") << " │";
            
            if (i == luaChon) resetColor();
            cout << endl;
        }

        cout << "   └" << string(12, '-') << "┴" << string(27, '-') << "┴" << string(12, '-') << "┴" << string(17, '-') << "┘" << endl;
        cout << "   (Mui ten: Len/Xuong, PgUp/PgDn: Chuyen trang, Enter: Chon, ESC: Thoat)\n";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) luaChon = max(0, luaChon - 1); // UP
            else if (key == 80) luaChon = min(n - 1, luaChon + 1); // DOWN
            else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE); // PGUP
            else if (key == 81) luaChon = min(n - 1, luaChon + 10); // PGDN
            trang = luaChon / ITEM_PER_PAGE;
        } else if (key == 13) {
            delete[] arr;
            return arr[luaChon]->maThe;
        } else if (key == 27) {
            delete[] arr;
            return -1;
        }
    }
}

DauSach* chonTuBangDauSach(ListDauSach& ds) {
    if (ds.n == 0) {
        cout << "Danh sach dau sach rong!\n";
        return nullptr;
    }

    int luaChon = 0;
    int trang = 0;
    const int ITEM_PER_PAGE = 10;
    int tongTrang = (ds.n > 0) ? ((ds.n - 1) / ITEM_PER_PAGE + 1) : 1;

    while (true) {
        system("cls");
        cout << "--- CHON DAU SACH (Trang " << trang + 1 << "/" << tongTrang << ") ---\n";
        cout << "   ┌" << string(17, '-') << "┬" << string(32, '-') << "┬" << string(22, '-') << "┬" << string(17, '-') << "┐" << endl;
        cout << "   │ " << left << setw(15) << "ISBN" << " │ " << left << setw(30) << "Ten Sach" << " │ " << left << setw(20) << "Tac Gia" << " │ " << left << setw(15) << "The Loai" << " │" << endl;
        cout << "   ├" << string(17, '-') << "┼" << string(32, '-') << "┼" << string(22, '-') << "┼" << string(17, '-') << "┤" << endl;

        int start = trang * ITEM_PER_PAGE;
        int end = min(start + ITEM_PER_PAGE, ds.n);

        for (int i = start; i < end; i++) {
            if (i == luaChon) { setColor(47); cout << ">> "; } 
            else { cout << "   "; }

            string tenSach = ds.nodes[i]->tenSach;
            if (tenSach.length() > 30) tenSach = tenSach.substr(0, 27) + "...";
            string tacGia = ds.nodes[i]->tacGia;
            if (tacGia.length() > 20) tacGia = tacGia.substr(0, 17) + "...";

            cout << "│ " << left << setw(15) << ds.nodes[i]->ISBN << " │ " << left << setw(30) << tenSach << " │ " << left << setw(20) << tacGia << " │ " << left << setw(15) << ds.nodes[i]->theLoai << " │";
            
            if (i == luaChon) resetColor();
            cout << endl;
        }
        cout << "   └" << string(17, '-') << "┴" << string(32, '-') << "┴" << string(22, '-') << "┴" << string(17, '-') << "┘" << endl;
        cout << "   (Mui ten: Len/Xuong, PgUp/PgDn: Chuyen trang, Enter: Chon, ESC: Thoat)\n";

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) luaChon = max(0, luaChon - 1);
            else if (key == 80) luaChon = min(ds.n - 1, luaChon + 1);
            else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE);
            else if (key == 81) luaChon = min(ds.n - 1, luaChon + ITEM_PER_PAGE);
            trang = luaChon / ITEM_PER_PAGE;
        } else if (key == 13) {
            return ds.nodes[luaChon];
        } else if (key == 27) {
            return nullptr;
        }
    }
}

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

        int status;
        switch (currentField) {
            case 0:
                status = nhapChuoiForm(winX + 13, winY + 3, ho, 50, false, true);
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

        if (status == 
        ) currentField = (currentField - 1 + 3) % 3;
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
                    int maThe = taoMaTheMoi(ql.root);
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

void quanLySachUI(ListDauSach& ds) {
    int luaChon = 0;
    const int ITEM_PER_PAGE = 15;
    char searchKeyword[100] = "";
    bool searchMode = false;

    while (true) {
        DauSach** displayArr = new DauSach*[ds.n];
        int n = 0;

        if (searchMode && strlen(searchKeyword) > 0) {
            string keywordLower = searchKeyword;
            transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), [](unsigned char c){ return tolower(c); });
            for (int i = 0; i < ds.n; i++) {
                if (strstr(ds.nodes[i]->tenSachSearch, keywordLower.c_str()) != nullptr ||
                    strstr(ds.nodes[i]->tacGiaSearch, keywordLower.c_str()) != nullptr ||
                    strstr(ds.nodes[i]->ISBN, keywordLower.c_str()) != nullptr) {
                    displayArr[n++] = ds.nodes[i];
                }
            }
        } else {
            for (int i = 0; i < ds.n; i++) {
                displayArr[n++] = ds.nodes[i];
            }
        }

        if (n == 0) {
            system("cls"); showCursor(false);
            setColor(33); cout << "--- QUAN LY DAU SACH ---\n\n"; resetColor();
            if (searchMode) cout << "Khong tim thay dau sach nao khop voi '" << searchKeyword << "'!\n\n";
            else cout << "Danh sach dau sach rong!\n\n";
            setColor(36); cout << "   (T)hem | (F)im | ESC: Quay lai\n"; resetColor();
            
            int key = _getch();
            if (key == 't' || key == 'T') {
                // formNhapDauSach(ds, nullptr, true);
                cout << "Chuc nang them moi se duoc them o day"; _getch();
            } else if (key == 'f' || key == 'F') {
                system("cls"); showCursor(true);
                if (nhapChuoiTuDo("Tim kiem (ISBN/Ten/Tac gia): ", searchKeyword, 100)) {
                    searchMode = (strlen(searchKeyword) > 0);
                    luaChon = 0;
                }
            } else if (key == 27) { delete[] displayArr; return; }
            delete[] displayArr;
            continue;
        }
        
        if (luaChon >= n) luaChon = max(0, n - 1);
        int tongTrang = (n > 0) ? ((n - 1) / ITEM_PER_PAGE + 1) : 1;
        int trang = luaChon / ITEM_PER_PAGE;

        system("cls"); showCursor(false);
        setColor(33); 
        if(searchMode) cout << "--- TIM KIEM: '" << searchKeyword << "' (" << n << " ket qua) - Trang " << trang + 1 << "/" << tongTrang << " ---\n\n";
        else cout << "--- QUAN LY DAU SACH (Trang " << trang + 1 << "/" << tongTrang << ") ---\n\n";
        resetColor();
        cout << "   ┌" << string(17, '-') << "┬" << string(32, '-') << "┬" << string(22, '-') << "┬" << string(17, '-') << "┐" << endl;
        cout << "   │ " << left << setw(15) << "ISBN" << " │ " << left << setw(30) << "Ten Sach" << " │ " << left << setw(20) << "Tac Gia" << " │ " << left << setw(15) << "The Loai" << " │" << endl;
        cout << "   ├" << string(17, '-') << "┼" << string(32, '-') << "┼" << string(22, '-') << "┼" << string(17, '-') << "┤" << endl;

        int start = trang * ITEM_PER_PAGE;
        int end = min(start + ITEM_PER_PAGE, n);

        for (int i = start; i < end; i++) {
            if (i == luaChon) { setColor(47); cout << ">> "; } 
            else { cout << "   "; }

            string tenSach = displayArr[i]->tenSach; if (tenSach.length() > 30) tenSach = tenSach.substr(0, 27) + "...";
            string tacGia = displayArr[i]->tacGia; if (tacGia.length() > 20) tacGia = tacGia.substr(0, 17) + "...";

            cout << "│ " << left << setw(15) << displayArr[i]->ISBN << " │ " << left << setw(30) << tenSach << " │ " << left << setw(20) << tacGia << " │ " << left << setw(15) << displayArr[i]->theLoai << " │";
            
            if (i == luaChon) resetColor();
            cout << endl;
        }
        cout << "   └" << string(17, '-') << "┴" << string(32, '-') << "┴" << string(22, '-') << "┴" << string(17, '-') << "┘" << endl;
        setColor(36);
        cout << "\n   (T)hem | (S)ua | (X)oa/Del | (C)uon sach | (F)im | (I)n DS | Mui ten | ESC: Quay lai\n";
        resetColor();

        int key = _getch();
        bool actionTaken = false;

        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 72) luaChon = max(0, luaChon - 1);
            else if (key == 80) luaChon = min(n - 1, luaChon + 1);
            else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE);
            else if (key == 81) luaChon = min(n - 1, luaChon + ITEM_PER_PAGE);
            else if (key == 83) { // DELETE key
                system("cls"); showCursor(true);
                cout << "Ban co chac chan muon xoa dau sach ISBN: " << displayArr[luaChon]->ISBN << " khong? (Y/N): ";
                char confirm = _getch(); cout << endl;
                if (toupper(confirm) == 'Y') xoaDauSach(ds, displayArr[luaChon]->ISBN); else cout << "Da huy thao tac xoa.\n";
                actionTaken = true;
            }
        } 
        else if (key == 27) { delete[] displayArr; return; }
        // Other keys like T, S, X, C, F, I would be handled here
        
        if (actionTaken) { cout << "\nNhan phim bat ky de tiep tuc..."; _getch(); }
        
        delete[] displayArr;
    }
}

// --- GIAO DIEN CHUYEN NGHIỆP ---
void quanLyDocGiaUI(QuanLyDocGia& ql) {
    int luaChon = 0;
    const int ITEM_PER_PAGE = 15;
    char searchKeyword[100] = "";
    bool searchMode = false;

    while (true) {
        // 1. Chuan bi du lieu de hien thi
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

        // 2. Hien thi khi danh sach rong
        if (n == 0) {
            system("cls");
            showCursor(false);
            setColor(33); cout << "--- QUAN LY DOC GIA ---\n\n";
            resetColor();
            if (searchMode) cout << "Khong tim thay doc gia nao khop voi '" << searchKeyword << "'!\n\n";
            else cout << "Danh sach doc gia rong!\n\n";
            setColor(36);
            cout << "   (T)hem | (F)im | ESC: Quay lai\n";
            resetColor();
            
            int key = _getch();
            if (key == 't' || key == 'T') {
                system("cls");
                showCursor(true);
                nhapDocGia(ql);
                cout << "\nNhan phim bat ky de tiep tuc...";
                _getch();
            } else if (key == 'f' || key == 'F') {
                system("cls"); showCursor(true);
                if (nhapChuoiTuDo("Tim kiem (theo Ten/Ma The, de trong de xem tat ca): ", searchKeyword, 100)) {
                    searchMode = (strlen(searchKeyword) > 0);
                    luaChon = 0;
                }
            } else if (key == 27) {
                delete[] displayArr;
                return; // Thoat khoi giao dien quan ly
            }
            delete[] displayArr;
            continue;
        }
        
        // 3. Hien thi danh sach
        if (luaChon >= n) {
            luaChon = max(0, n - 1);
        }

        int tongTrang = (n > 0) ? ((n - 1) / ITEM_PER_PAGE + 1) : 1;
        int trang = luaChon / ITEM_PER_PAGE;

        system("cls");
        showCursor(false);
        setColor(33); 
        if(searchMode) cout << "--- TIM KIEM: '" << searchKeyword << "' (" << n << " ket qua) - Trang " << trang + 1 << "/" << tongTrang << " ---\n\n";
        else cout << "--- QUAN LY DOC GIA (Trang " << trang + 1 << "/" << tongTrang << ") ---\n\n";
        resetColor();
        cout << "   ┌" << string(8, '-') << "┬" << string(22, '-') << "┬" << string(12, '-') << "┬" << string(12, '-') << "┬" << string(17, '-') << "┐" << endl;
        cout << "   │ " << left << setw(6) << "Ma The" << " │ " << left << setw(20) << "Ho" << " │ " << left << setw(10) << "Ten" << " │ " << left << setw(10) << "Gioi Tinh" << " │ " << left << setw(15) << "Trang Thai" << " │" << endl;
        cout << "   ├" << string(8, '-') << "┼" << string(22, '-') << "┼" << string(12, '-') << "┼" << string(12, '-') << "┼" << string(17, '-') << "┤" << endl;

        int start = trang * ITEM_PER_PAGE;
        int end = min(start + ITEM_PER_PAGE, n);

        for (int i = start; i < end; i++) {
            if (i == luaChon) { setColor(47); cout << ">> "; } 
            else { cout << "   "; }

            string ho = displayArr[i]->ho;
            if (ho.length() > 20) ho = ho.substr(0, 17) + "...";

            cout << "│ " << left << setw(6) << displayArr[i]->maThe << " │ " << left << setw(20) << ho << " │ " << left << setw(10) << displayArr[i]->ten << " │ " << left << setw(10) << displayArr[i]->giotinh << " │ " << left << setw(15) << (displayArr[i]->trangThaiThe == 1 ? "Hoat dong" : "Khoa") << " │";
            
            if (i == luaChon) resetColor();
            cout << endl;
        }

        cout << "   └" << string(8, '-') << "┴" << string(22, '-') << "┴" << string(12, '-') << "┴" << string(12, '-') << "┴" << string(17, '-') << "┘" << endl;
        setColor(36);
        cout << "\n   (T)hem | (S)ua | (X)oa/Del | (K)hoa/Mo | (F)im | (I)n DS | Mui ten: Di chuyen | ESC: Quay lai\n";
        resetColor();

        // 4. Xu ly phim bam
        int key = _getch();
        bool actionTaken = false;

        if (key == 0 || key == 224) {
            key = _getch();
            if (key == 72) luaChon = max(0, luaChon - 1);
            else if (key == 80) luaChon = min(n - 1, luaChon + 1);
            else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE);
            else if (key == 81) luaChon = min(n - 1, luaChon + ITEM_PER_PAGE);
            else if (key == 83) { // DELETE key
                system("cls"); showCursor(true);
                cout << "Ban co chac chan muon xoa doc gia co ma the " << displayArr[luaChon]->maThe << " khong? (Y/N): ";
                char confirm = _getch(); cout << endl;
                if (toupper(confirm) == 'Y') xoaDocGia(ql, displayArr[luaChon]->maThe); else cout << "Da huy thao tac xoa.\n";
                actionTaken = true;
            }
        } 
        else if (key == 27) { delete[] displayArr; return; }
        else if (key == 't' || key == 'T') {
            int winX = 28, winY = 4, winW = 50, winH = 12;
            showCursor(true);
            xoaVung(winX, winY, winW, winH); 
            setColor(33);
            for (int i = 0; i < winW; i++) { gotoxy(winX + i, winY); cout << "─"; gotoxy(winX + i, winY + winH - 1); cout << "─"; }
            for (int i = 0; i < winH; i++) { gotoxy(winX, winY + i); cout << "│"; gotoxy(winX + winW - 1, winY + i); cout << "│"; }
            gotoxy(winX, winY); cout << "┌"; gotoxy(winX + winW - 1, winY); cout << "┐"; gotoxy(winX, winY + winH - 1); cout << "└"; gotoxy(winX + winW - 1, winY + winH - 1); cout << "┘";
            gotoxy(winX + 2, winY); cout << "[ THEM DOC GIA MOI ]";

            resetColor();
            int maThe = taoMaTheMoi(ql.root);
            DocGia* dg = taoDocGia(maThe);

            gotoxy(winX + 2, winY + 2); cout << "Ma the (tu dong): " << maThe;

            gotoxy(winX + 2, winY + 3);
            if (!nhapTen("Ho: ", dg->ho, 50)) { delete dg; actionTaken = true; continue; } 

            gotoxy(winX + 2, winY + 4);
            if (!nhapTen("Ten: ", dg->ten, 20)) { delete dg; actionTaken = true; continue; }

            int chonGT;
            do {
                gotoxy(winX + 2, winY + 5); cout << string(winW - 4, ' '); // Xoa dong cu
                gotoxy(winX + 2, winY + 5); cout << "Gioi tinh (1. NAM | 2. NU): ";
                chonGT = nhapSoNguyenHopLe("");
                if (chonGT == 1) strcpy(dg->giotinh, "NAM");
                else if (chonGT == 2) strcpy(dg->giotinh, "NU");
                else { gotoxy(winX + 2, winY + 6); setColor(31); cout << "Loi: Vui long chon 1 hoac 2!"; resetColor(); }
            } while (chonGT != 1 && chonGT != 2);
            
            if (themDocGia(ql, dg)) { gotoxy(winX + 2, winY + 7); setColor(32); cout << "Them doc gia thanh cong!"; }
            else { gotoxy(winX + 2, winY + 7); setColor(31); cout << "Them that bai! Ma the co the da ton tai."; }
            resetColor();
        else if (key == 't' || key == 'T') { 
            formNhapDocGia(ql, nullptr, true);
            actionTaken = true;
        }
        else if (key == 's' || key == 'S') {
            DocGia* dg = displayArr[luaChon];
            int winX = 28, winY = 4, winW = 50, winH = 12;
            showCursor(true);
            xoaVung(winX, winY, winW, winH);
            setColor(33);
            for (int i = 0; i < winW; i++) { gotoxy(winX + i, winY); cout << "─"; gotoxy(winX + i, winY + winH - 1); cout << "─"; }
            for (int i = 0; i < winH; i++) { gotoxy(winX, winY + i); cout << "│"; gotoxy(winX + winW - 1, winY + i); cout << "│"; }
            gotoxy(winX, winY); cout << "┌"; gotoxy(winX + winW - 1, winY); cout << "┐"; gotoxy(winX, winY + winH - 1); cout << "└"; gotoxy(winX + winW - 1, winY + winH - 1); cout << "┘";
            gotoxy(winX + 2, winY); cout << "[ HIEU CHINH DOC GIA ]";
            
            resetColor();
            gotoxy(winX + 2, winY + 2); cout << "Ma the: " << dg->maThe;

            char hoMoi[51], tenMoi[21];
            gotoxy(winX + 2, winY + 3); if (!nhapTen("Ho: ", hoMoi, 50, dg->ho)) { actionTaken = true; continue; }
            gotoxy(winX + 2, winY + 4); if (!nhapTen("Ten: ", tenMoi, 20, dg->ten)) { actionTaken = true; continue; }
            
            int chonGT;
            do {
                gotoxy(winX + 2, winY + 5); cout << string(winW - 4, ' ');
                gotoxy(winX + 2, winY + 5); cout << "Gioi tinh (1. NAM | 2. NU): ";
                chonGT = nhapSoNguyenHopLe("");
                if (chonGT != 1 && chonGT != 2) { gotoxy(winX + 2, winY + 6); setColor(31); cout << "Loi: Vui long chon 1 hoac 2!"; resetColor(); }
            } while (chonGT != 1 && chonGT != 2);

            strcpy(dg->ho, hoMoi); strcpy(dg->ten, tenMoi);
            if (chonGT == 1) strcpy(dg->giotinh, "NAM"); else strcpy(dg->giotinh, "NU");
            gotoxy(winX + 2, winY + 7); setColor(32); cout << "Hieu chinh thanh cong!"; resetColor();
        else if (key == 's' || key == 'S') { 
            formNhapDocGia(ql, displayArr[luaChon], false);
            actionTaken = true;
        }
        else if (key == 'x' || key == 'X') { system("cls"); showCursor(true); cout << "Ban co chac chan muon xoa doc gia co ma the " << displayArr[luaChon]->maThe << " khong? (Y/N): "; char confirm = _getch(); cout << endl; if (toupper(confirm) == 'Y') xoaDocGia(ql, displayArr[luaChon]->maThe); else cout << "Da huy thao tac xoa.\n"; actionTaken = true; }
        else if (key == 'k' || key == 'K') { system("cls"); showCursor(true); khoaMoThe(ql.root, displayArr[luaChon]->maThe); actionTaken = true; }
        else if (key == 'f' || key == 'F') { 
            system("cls"); showCursor(true);
            if (nhapChuoiTuDo("Tim kiem (theo Ten/Ma The, de trong de xem tat ca): ", searchKeyword, 100)) {
                searchMode = (strlen(searchKeyword) > 0);
                luaChon = 0; // Reset lua chon ve dau danh sach khi tim kiem moi
            }
            // Neu huy (nhan ESC), khong lam gi ca, tu khoa tim kiem van giu nguyen
        }
        else if (key == 'i' || key == 'I') { const char* subInDocGia[] = { "In theo Ma The (tang dan)", "In theo Ten (A-Z)", "<- Quay lai" }; int indexIn = 0; system("cls"); gotoxy(10, 4); setColor(33); cout << "--- CHON KIEU IN ---"; int chonIn = chonMenu(subInDocGia, 3, 10, 7, indexIn); if (chonIn != 2 && chonIn != -1) { system("cls"); showCursor(true); if (chonIn == 0) inDanhSachDocGia(ql.root); else if (chonIn == 1) inTheoTen(ql); actionTaken = true; } }
        
        if (actionTaken) { 
            showCursor(false);
            cout << "\nNhan phim bat ky de tiep tuc..."; _getch(); 
        }
        
        delete[] displayArr;
    }
}

// --- HAM CHAY CHUONG TRINH CHINH ---
void runMenu() {
    setupConsole();
    QuanLyDocGia qlDocGia;
    ListDauSach ds; ds.n = 0;
    
    loadDauSach("Input_file/DauSach.txt", ds);
    loadDocGia("Input_file/DocGia.txt", qlDocGia);
    khoiTaoKhoMaThe(qlDocGia.root);

    const char* menuTrai[] = { "Tong quan", "Quan ly doc gia", "Quan ly sach", "Giao dich", "Thong ke", "Thoat" };
    int indexTrai = 0;

    const char* subDocGia[] = { "Them doc gia", "Xoa doc gia", "Hieu chinh", "Khoa/Mo the", "In danh sach", "<- Quay lai" };
    const char* subSach[] = { "Them dau sach", "Hieu chinh", "Xoa dau sach", "Them cuon sach", "Tra cuu theo Ten Sach", "Tra cuu theo Tac Gia", "In danh sach", "<- Quay lai" };
    const char* subGiaoDich[] = { "Muon sach", "Tra sach", "Bao mat sach", "Xem sach dang muon", "<- Quay lai" };
    const char* subThongKe[] = { "Doc gia qua han", "Top 10 sach", "Thong ke the loai", "<- Quay lai" };

    bool isRunning = true;
    while (isRunning) {
        veKhungGiaoDien();
        if (indexTrai == 0) {
            int sm = 0, qh = 0;
            demThongKeTongQuan(qlDocGia.root, sm, qh);
            hienThiTongQuan(ds.n, qlDocGia.soLuongDocGia, sm, qh);
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
        if (chonChinh == 0) continue; 

        bool inSub = true;
        int indexPhai = 0;
        while (inSub) {
            veMenuList(menuTrai, 6, indexTrai, 2, 6, false);
            xoaVung(28, 4, 50, 18);
            int chonPhai = -1;

            if (indexTrai == 1) { // --- QUAN LY DOC GIA ---
                quanLyDocGiaUI(qlDocGia);
                inSub = false; // Thoat khoi vong lap menu con
                continue;      // Quay ve vong lap menu chinh, bo qua phan "Nhan phim bat ky"
            } 
            else if (indexTrai == 2) { // --- QUAN LY SACH ---
                gotoxy(40, 4); setColor(33); cout << "--- QUAN LY SACH ---";
                chonPhai = chonMenu(subSach, 8, 35, 7, indexPhai);
                if (chonPhai == 7 || chonPhai == -1) { inSub = false; continue; }
                
                if (chonPhai == 0) { system("cls"); showCursor(true); themDauSach(ds); }
                else if (chonPhai == 4) { 
                    system("cls"); showCursor(true); 
                    char key[100]; 
                    if (nhapChuoiTuDo("Nhap tu khoa TEN SACH can tim: ", key, 100)) {
                        traCuuTheoTenNangCao(ds, key); 
                    }
                }
                else if (chonPhai == 5) { 
                    system("cls"); showCursor(true); 
                    char key[100]; 
                    if (nhapChuoiTuDo("Nhap tu khoa TEN TAC GIA can tim: ", key, 100)) {
                        traCuuTheoTacGia(ds, key); 
                    }
                }
                else if (chonPhai == 6) { system("cls"); showCursor(true); inTheoTheLoai_TrongDoTenTangDan(ds); }
                else {
                    DauSach* d = chonTuBangDauSach(ds);
                    if (d != nullptr) {
                        if (chonPhai == 1) { system("cls"); showCursor(true); hieuChinhDauSach(ds); } // Hieu chinh van can nhap ISBN
                        else if (chonPhai == 2) {
                            system("cls"); showCursor(true);
                            cout << "Ban co chac chan muon xoa dau sach ISBN: " << d->ISBN << " khong? (Y/N): ";
                            char confirm; cin >> confirm; cin.ignore(1000, '\n');
                            if (toupper(confirm) == 'Y') xoaDauSach(ds, d->ISBN); else cout << "Huy xoa dau sach.\n";
                        }
                        else if (chonPhai == 3) { system("cls"); showCursor(true); themCuonSach(d); }
                    }
                }
                quanLySachUI(ds);
                inSub = false;
                continue;
            }
            else if (indexTrai == 3) { // --- GIAO DICH ---
                gotoxy(40, 4); setColor(33); cout << "--- GIAO DICH ---";
                chonPhai = chonMenu(subGiaoDich, 5, 35, 7, indexPhai);
                if (chonPhai == 4 || chonPhai == -1) { inSub = false; continue; }

                system("cls"); showCursor(true);
                cout << "--- TRA CUU NHANH DOC GIA ---\n";
                inDanhSachDocGia(qlDocGia.root);
                int ma = nhapSoNguyenHopLe("\nNhap ma the doc gia de giao dich: ");
                DocGia* dg = timDocGia(qlDocGia.root, ma);
                if (!dg) cout << "Loi: Khong tim thay doc gia!\n";
                else {
                    system("cls");
                    cout << "=== DOC GIA: " << dg->ho << " " << dg->ten << " ===\n";
                    xemSachDangMuon(dg, ds); // Hien thi ngay danh sach dang muon
                    cout << "------------------------------------------\n";

                    if (chonPhai == 0) { // Muon sach
                        if (dg->trangThaiThe == 0) cout << "The dang bi KHOA!\n";
                        else if (dg->soSachDangMuon >= 3) cout << "Da muon toi da 3 cuon!\n";
                        else {
                            cout << "\n--- CAC DAU SACH CO THE MUON ---\n";
                            inDanhSachDauSach(ds);
                            char ms[25]; 
                            if (nhapMa("\nNhap MA SACH (VD: ISBN_0001): ", ms, 25)) {
                                muonSach(dg, ds, ms);
                            } else {
                                cout << "Da huy muon sach.\n";
                            }
                        }
                    }
                    else if (chonPhai == 1) { // Tra sach
                        if (dg->soSachDangMuon == 0) cout << "Doc gia nay dau co muon sach dau ma tra ní!\n";
                        else {
                            char ms[25]; 
                            if (nhapMa("\nNhap ma sach can TRA: ", ms, 25)) {
                                traSach(dg, ds, ms, layNgayHienTai());
                            } else {
                                cout << "Da huy tra sach.\n";
                            }
                        }
                    }
                    else if (chonPhai == 2) { // Bao mat
                        char ms[25]; 
                        if (nhapMa("\nNhap ma sach bao MAT: ", ms, 25)) {
                            baoMatSach(dg, ds, ms);
                        } else {
                            cout << "Da huy bao mat.\n";
                        }
                    }
                }
            }
            else if (indexTrai == 4) { // --- THONG KE ---
                gotoxy(40, 4); setColor(33); cout << "--- THONG KE ---";
                chonPhai = chonMenu(subThongKe, 4, 35, 7, indexPhai);
                if (chonPhai == 3 || chonPhai == -1) { inSub = false; continue; }

                system("cls"); showCursor(true);
                if (chonPhai == 0) lietKeDocGiaQuaHan(qlDocGia);
                else if (chonPhai == 1) inTop10SachMuonNhieu(ds);
                else if (chonPhai == 2) thongKeSoLuongTheoTheLoai(ds);
            }

            if (inSub) { // Chi hien thi khi van con o menu con
                cout << "\n\nNhan phim bat ky de quay lai menu...";
                _getch(); showCursor(false);
                veKhungGiaoDien(); 
            }
        }
    }
}