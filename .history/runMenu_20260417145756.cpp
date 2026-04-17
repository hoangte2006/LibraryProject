#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>

// Luu y: Dam bao cac file .h nay ton tai trong thu muc project
#include "Structs.h"
#include "Docgia.h"
#include "Sach.h"
#include "Giaodich.h"
#include "Thongke.h"
#include "File.h"
#include "MuonTra.h"

using namespace std;

// --- 1. CAC HAM HO TRO LOGIC (Giai quyet loi 'not declared') ---

// Ham de quy dem so sach dang muon va doc gia qua han
void demThongKeTongQuan(TREE_DocGia root, int& soSachDangMuon, int& soDocGiaQuaHan) {
    if (root == nullptr) return;
    demThongKeTongQuan(root->pLeft, soSachDangMuon, soDocGiaQuaHan);
    
    soSachDangMuon += root->soSachDangMuon;
    if (coSachQuaHan(root)) { // Ham naу nam trong MuonTra.h hoac Docgia.h cua ban
        soDocGiaQuaHan++;
    }
    
    demThongKeTongQuan(root->pRight, soSachDangMuon, soDocGiaQuaHan);
}

// --- 2. CAC HAM DO HOA CO BAN ---

void gotoxy(int x, int y) {
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = { (short)x, (short)y };
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void setColor(int color) {
    cout << "\033[1;" << color << "m";
}

void resetColor() {
    cout << "\033[0m";
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

// --- 3. GIAO DIEN CHINH ---

void veKhungGiaoDien() {
    system("cls");
    setColor(36); 
    gotoxy(0, 0);
    cout << "==================================================================================";
    gotoxy(0, 1);
    cout << "||                     HE THONG QUAN LY THU VIEN CAO CAP                        ||";
    gotoxy(0, 2);
    cout << "==================================================================================";
    
    for (int i = 3; i <= 22; i++) {
        gotoxy(25, i); cout << "||";
    }
    
    gotoxy(0, 23);
    cout << "==================================================================================";
    resetColor();
}

void hienThiTongQuan(int tongDauSach, int tongDocGia, int dangMuon, int quaHan) {
    int x = 30, y = 6;
    xoaVung(28, 4, 50, 18);
    
    setColor(33); 
    gotoxy(x+10, 4); cout << "--- TONG QUAN HE THONG ---";
    
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
            if (isFocus) {
                setColor(32); 
                cout << " > " << options[i] << "   ";
            } else {
                setColor(33); 
                cout << "   " << options[i] << "   ";
            }
        } else {
            resetColor();
            cout << "   " << options[i] << "   ";
        }
        resetColor();
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

// --- 4. HAM CHAY CHUONG TRINH ---

void runMenu() {
    setupConsole();
    
    QuanLyDocGia qlDocGia;
    ListDauSach ds; ds.n = 0;
    
    // Load du lieu tu file
    loadDauSach("Input_file/DauSach.txt", ds);
    loadDocGia("Input_file/DocGia.txt", qlDocGia);

    const char* menuTrai[] = { "Tong quan", "Quan ly doc gia", "Quan ly sach", "Giao dich", "Thong ke", "Thoat" };
    int nTrai = 6;
    int indexTrai = 0;

    const char* subDocGia[] = { "Them doc gia", "Xoa doc gia", "Hieu chinh", "Khoa/Mo the", "In danh sach", "<- Quay lai" };
    const char* subSach[] = { "Them dau sach", "Hieu chinh", "Xoa dau sach", "Them cuon sach", "In danh sach", "<- Quay lai" };
    const char* subGiaoDich[] = { "Muon sach", "Tra sach", "Bao mat sach", "Xem sach dang muon", "<- Quay lai" };
    const char* subThongKe[] = { "Doc gia qua han", "Top 10 sach", "Thong ke the loai", "<- Quay lai" };

    bool isRunning = true;
    while (isRunning) {
        veKhungGiaoDien();
        
        if (indexTrai == 0) {
            // QUAN TRONG: Phai khoi tao sm va qh bang 0 truoc khi truyen vao ham
            int sm = 0; 
            int qh = 0;
            demThongKeTongQuan(qlDocGia.root, sm, qh);
            hienThiTongQuan(ds.n, qlDocGia.soLuongDocGia, sm, qh);
        } else {
            gotoxy(35, 12); setColor(30); 
            cout << "(Nhan Enter de vao menu con)"; resetColor();
        }

        int chonChinh = chonMenu(menuTrai, nTrai, 2, 6, indexTrai);

        if (chonChinh == 5 || chonChinh == -1) {
            system("cls");
            saveDocGia("Input_file/DocGia.txt", qlDocGia.root);
            saveDauSach("Input_file/DauSach.txt", ds);
            cout << "Da luu du lieu. Tam biet!\n";
            isRunning = false;
            break;
        }

        if (chonChinh == 0) continue; 

        bool inSub = true;
        int indexPhai = 0;
        
        while (inSub) {
            veMenuList(menuTrai, nTrai, indexTrai, 2, 6, false);
            xoaVung(28, 4, 50, 18);
            int chonPhai = -1;

            if (indexTrai == 1) {
                gotoxy(40, 4); setColor(33); cout << "--- QUAN LY DOC GIA ---";
                chonPhai = chonMenu(subDocGia, 6, 35, 7, indexPhai);
            } 
            else if (indexTrai == 2) {
                gotoxy(40, 4); setColor(33); cout << "--- QUAN LY SACH ---";
                chonPhai = chonMenu(subSach, 6, 35, 7, indexPhai);
            }
            else if (indexTrai == 3) {
                gotoxy(40, 4); setColor(33); cout << "--- GIAO DICH ---";
                chonPhai = chonMenu(subGiaoDich, 5, 35, 7, indexPhai);
            }
            else if (indexTrai == 4) {
                gotoxy(40, 4); setColor(33); cout << "--- THONG KE ---";
                chonPhai = chonMenu(subThongKe, 4, 35, 7, indexPhai);
            }

            // Neu chon nut "Quay lai" (nut cuoi cung cua moi menu con)
            if (chonPhai == -1 || 
               (indexTrai == 1 && chonPhai == 5) || 
               (indexTrai == 2 && chonPhai == 5) || 
               (indexTrai == 3 && chonPhai == 4) || 
               (indexTrai == 4 && chonPhai == 3)) {
                inSub = false;
                continue;
            }

            // Thuc thi chuc nang
            system("cls");
            showCursor(true);
            resetColor();
            
            // Link cac ham xu ly logic vao day
            if (indexTrai == 1) {
                switch(chonPhai) {
                    case 0: nhapDocGia(qlDocGia); break;
                    // Them cac case khac...
                }
            }
            
            cout << "\n\nNhan phim bat ky de quay lai...";
            _getch();
            showCursor(false);
            veKhungGiaoDien(); 
        }
    }

    giaiPhongCay(qlDocGia.root);
    giaiPhongDanhSachDauSach(ds);
}