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
        cout << "   ┌" << string(12, '─') << "┬" << string(27, '─') << "┬" << string(12, '─') << "┬" << string(17, '─') << "┐" << endl;
        cout << "   │ " << left << setw(10) << "Ma The" << " │ " << left << setw(25) << "Ho Ten" << " │ " << left << setw(10) << "Gioi Tinh" << " │ " << left << setw(15) << "Trang Thai" << " │" << endl;
        cout << "   ├" << string(12, '─') << "┼" << string(27, '─') << "┼" << string(12, '─') << "┼" << string(17, '─') << "┤" << endl;

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

        cout << "   └" << string(12, '─') << "┴" << string(27, '─') << "┴" << string(12, '─') << "┴" << string(17, '─') << "┘" << endl;
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
        cout << "   ┌" << string(17, '─') << "┬" << string(32, '─') << "┬" << string(22, '─') << "┬" << string(17, '─') << "┐" << endl;
        cout << "   │ " << left << setw(15) << "ISBN" << " │ " << left << setw(30) << "Ten Sach" << " │ " << left << setw(20) << "Tac Gia" << " │ " << left << setw(15) << "The Loai" << " │" << endl;
        cout << "   ├" << string(17, '─') << "┼" << string(32, '─') << "┼" << string(22, '─') << "┼" << string(17, '─') << "┤" << endl;

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
        cout << "   └" << string(17, '─') << "┴" << string(32, '─') << "┴" << string(22, '─') << "┴" << string(17, '─') << "┘" << endl;
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

// --- GIAO DIEN CHUYEN NGHIỆP ---
void quanLyDocGiaUI(QuanLyDocGia& ql) {
    int luaChon = 0;
    const int ITEM_PER_PAGE = 15;

    while (true) {
        // Refresh data on every loop to reflect changes (add/delete)
        if (ql.soLuongDocGia == 0) {
            system("cls");
            showCursor(false);
            setColor(33); cout << "--- QUAN LY DOC GIA ---\n\n";
            resetColor();
            cout << "Danh sach doc gia rong!\n\n";
            setColor(36);
            cout << "   (T)hem doc gia moi | ESC: Quay lai\n";
            resetColor();
            
            int key = _getch();
            if (key == 't' || key == 'T') {
                system("cls");
                showCursor(true);
                nhapDocGia(ql);
                cout << "\nNhan phim bat ky de tiep tuc...";
                _getch();
                continue; // Re-enter the loop
            } else if (key == 27) {
                return;
            }
            continue;
        }

        DocGia** arr = new DocGia*[ql.soLuongDocGia];
        int n = 0;
        BSTtoArray(ql.root, arr, n);
        
        if (luaChon >= n) {
            luaChon = max(0, n - 1);
        }

        int tongTrang = (n > 0) ? ((n - 1) / ITEM_PER_PAGE + 1) : 1;
        int trang = luaChon / ITEM_PER_PAGE;

        system("cls");
        showCursor(false);
        setColor(33); cout << "--- QUAN LY DOC GIA (Trang " << trang + 1 << "/" << tongTrang << ") ---\n\n";
        resetColor();
        cout << "   ┌" << string(12, '─') << "┬" << string(27, '─') << "┬" << string(12, '─') << "┬" << string(17, '─') << "┐" << endl;
        cout << "   │ " << left << setw(10) << "Ma The" << " │ " << left << setw(25) << "Ho Ten" << " │ " << left << setw(10) << "Gioi Tinh" << " │ " << left << setw(15) << "Trang Thai" << " │" << endl;
        cout << "   ├" << string(12, '─') << "┼" << string(27, '─') << "┼" << string(12, '─') << "┼" << string(17, '─') << "┤" << endl;

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

        cout << "   └" << string(12, '─') << "┴" << string(27, '─') << "┴" << string(12, '─') << "┴" << string(17, '─') << "┘" << endl;
        setColor(36);
        cout << "\n   (T)hem | (S)ua | (X)oa/Del | (K)hoa/Mo | (I)n DS | Mui ten: Di chuyen | ESC: Quay lai\n";
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
                cout << "Ban co chac chan muon xoa doc gia co ma the " << arr[luaChon]->maThe << " khong? (Y/N): ";
                char confirm = _getch(); cout << endl;
                if (toupper(confirm) == 'Y') xoaDocGia(ql, arr[luaChon]->maThe); else cout << "Da huy thao tac xoa.\n";
                actionTaken = true;
            }
        } 
        else if (key == 27) { delete[] arr; return; }
        else if (key == 't' || key == 'T') { system("cls"); showCursor(true); nhapDocGia(ql); actionTaken = true; }
        else if (key == 's' || key == 'S') { system("cls"); showCursor(true); hieuChinhDocGia(ql.root, arr[luaChon]->maThe); actionTaken = true; }
        else if (key == 'x' || key == 'X') { system("cls"); showCursor(true); cout << "Ban co chac chan muon xoa doc gia co ma the " << arr[luaChon]->maThe << " khong? (Y/N): "; char confirm = _getch(); cout << endl; if (toupper(confirm) == 'Y') xoaDocGia(ql, arr[luaChon]->maThe); else cout << "Da huy thao tac xoa.\n"; actionTaken = true; }
        else if (key == 'k' || key == 'K') { system("cls"); showCursor(true); khoaMoThe(ql.root, arr[luaChon]->maThe); actionTaken = true; }
        else if (key == 'i' || key == 'I') { const char* subInDocGia[] = { "In theo Ma The (tang dan)", "In theo Ten (A-Z)", "<- Quay lai" }; int indexIn = 0; system("cls"); gotoxy(10, 4); setColor(33); cout << "--- CHON KIEU IN ---"; int chonIn = chonMenu(subInDocGia, 3, 10, 7, indexIn); if (chonIn != 2 && chonIn != -1) { system("cls"); showCursor(true); if (chonIn == 0) inDanhSachDocGia(ql.root); else if (chonIn == 1) inTheoTen(ql); actionTaken = true; } }
        
        if (actionTaken) { cout << "\nNhan phim bat ky de tiep tuc..."; _getch(); }
        
        delete[] arr;
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
                
                if (chonPhai == 0) {
                    system("cls"); showCursor(true);
                    nhapDocGia(qlDocGia);
                }
                else if (chonPhai == 4) {
                    const char* subInDocGia[] = { "In theo Ma The (tang dan)", "In theo Ten (A-Z)", "<- Quay lai" };
                    int indexIn = 0;
                    xoaVung(28, 4, 50, 18);
                    gotoxy(40, 4); setColor(33); cout << "--- CHON KIEU IN ---";
                    int chonIn = chonMenu(subInDocGia, 3, 35, 7, indexIn);

                    if (chonIn == 2 || chonIn == -1) { // Quay lai hoac ESC
                        continue; // Bỏ qua phần "Nhan phim bat ky" và vẽ lại menu Quan Ly Doc Gia
                    }

                    system("cls"); showCursor(true);
                    if (chonIn == 0) {
                        inDanhSachDocGia(qlDocGia.root);
                    } else if (chonIn == 1) {
                        inTheoTen(qlDocGia);
                    }
                }
                else {
                    int ma = chonTuBangDocGia(qlDocGia);
                    if (ma != -1) {
                        system("cls"); showCursor(true);
                        if (chonPhai == 1) xoaDocGia(qlDocGia, ma);
                        else if (chonPhai == 2) hieuChinhDocGia(qlDocGia.root, ma);
                        else if (chonPhai == 3) khoaMoThe(qlDocGia.root, ma);
                    }
                }
                quanLyDocGiaUI(qlDocGia);
                inSub = false; // Thoat khoi vong lap menu con
                continue;      // Quay ve vong lap menu chinh, bo qua phan "Nhan phim bat ky"
            } 
            else if (indexTrai == 2) { // --- QUAN LY SACH ---
                gotoxy(40, 4); setColor(33); cout << "--- QUAN LY SACH ---";
                chonPhai = chonMenu(subSach, 7, 35, 7, indexPhai);
                if (chonPhai == 6 || chonPhai == -1) { inSub = false; continue; }
                
                if (chonPhai == 0) { system("cls"); showCursor(true); themDauSach(ds); }
                else if (chonPhai == 4) { system("cls"); showCursor(true); char key[100]; nhapChuoiTuDo("Nhap ten sach can tim: ", key, 100); traCuuTheoTenNangCao(ds, key); }
                else if (chonPhai == 5) { system("cls"); showCursor(true); inTheoTheLoai_TrongDoTenTangDan(ds); }
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
            if (inSub) { // Chi hien thi khi van con o menu con
                cout << "\n\nNhan phim bat ky de quay lai menu...";
                _getch(); showCursor(false);
                veKhungGiaoDien(); 
            }
        }
    }
}