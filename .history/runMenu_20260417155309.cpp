#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
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
    const char* subSach[] = { "Them dau sach", "Hieu chinh", "Xoa dau sach", "Them cuon sach", "Tra cuu ten", "In danh sach", "<- Quay lai" };
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
                gotoxy(40, 4); setColor(33); cout << "--- QUAN LY DOC GIA ---";
                chonPhai = chonMenu(subDocGia, 6, 35, 7, indexPhai);
                if (chonPhai == 5 || chonPhai == -1) { inSub = false; continue; }
                
                system("cls"); showCursor(true);
                if (chonPhai == 0) nhapDocGia(qlDocGia);
                else {
                    // Cac chuc nang can nhap Ma The: Luon hien danh sach truoc
                    cout << "--- DANH SACH DOC GIA HIEN CO ---\n";
                    inDanhSachDocGia(qlDocGia.root); 
                    cout << "---------------------------------\n";
                    int ma = nhapSoNguyenHopLe("Nhap ma the thuc hien: ");
                    if (chonPhai == 1) xoaDocGia(qlDocGia, ma);
                    else if (chonPhai == 2) hieuChinhDocGia(qlDocGia.root, ma);
                    else if (chonPhai == 3) khoaMoThe(qlDocGia.root, ma);
                    else if (chonPhai == 4) inTheoTen(qlDocGia);
                }
            } 
            else if (indexTrai == 2) { // --- QUAN LY SACH ---
                gotoxy(40, 4); setColor(33); cout << "--- QUAN LY SACH ---";
                chonPhai = chonMenu(subSach, 7, 35, 7, indexPhai);
                if (chonPhai == 6 || chonPhai == -1) { inSub = false; continue; }

                system("cls"); showCursor(true);
                if (chonPhai == 0) themDauSach(ds);
                else {
                    // Luon hien danh sach dau sach de nguoi dung thay ISBN
                    inDanhSachDauSach(ds);
                    cout << "---------------------------------\n";
                    if (chonPhai == 1) hieuChinhDauSach(ds);
                    else if (chonPhai == 2) xoaDauSachUI(ds);
                    else if (chonPhai == 3) {
                        char isbn[20]; nhapMa("Nhap ISBN de them cuon sach: ", isbn, 20);
                        DauSach* d = timTheoISBN(ds, isbn);
                        if (d) themCuonSach(d); else cout << "ISBN khong ton tai!\n";
                    }
                    else if (chonPhai == 4) {
                        char key[100]; nhapChuoiTuDo("Nhap ten sach can tim: ", key, 100);
                        traCuuTheoTenNangCao(ds, key);
                    }
                    else if (chonPhai == 5) inTheoTheLoai_TrongDoTenTangDan(ds);
                }
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
                            char ms[25]; nhapMa("\nNhap MA SACH (VD: ISBN_0001): ", ms, 25);
                            muonSach(dg, ds, ms);
                        }
                    }
                    else if (chonPhai == 1) { // Tra sach
                        if (dg->soSachDangMuon == 0) cout << "Doc gia nay dau co muon sach dau ma tra ní!\n";
                        else {
                            char ms[25]; nhapMa("\nNhap ma sach can TRA: ", ms, 25);
                            traSach(dg, ds, ms, layNgayHienTai());
                        }
                    }
                    else if (chonPhai == 2) { // Bao mat
                        char ms[25]; nhapMa("\nNhap ma sach bao MAT: ", ms, 25);
                        baoMatSach(dg, ds, ms);
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

            cout << "\n\nNhan phim bat ky de quay lai menu...";
            _getch(); showCursor(false);
            veKhungGiaoDien(); 
        }
    }
}