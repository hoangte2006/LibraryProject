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

const char* menuTrai[] = { "Tong quan", "Quan ly doc gia", "Quan ly sach", "Giao dich", "Thong ke", "Thoat" };

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
        cout << "Danh sach doc gia rong!\nNhan phim bat ky de thoat.";
        _getch();
        return -1;
    }

    DocGia** fullArr = new DocGia*[ql.soLuongDocGia];
    int full_n = 0;
    BSTtoArray(ql.root, fullArr, full_n);

    char searchKeyword[100] = "";
    bool searchMode = false; 
    int luaChon = 0;
    const int ITEM_PER_PAGE = 10;

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

        system("cls");
        if (n == 0) {
            cout << "--- CHON DOC GIA ---\n";
            cout << "Khong tim thay doc gia nao khop voi '" << searchKeyword << "'!\n\n";
            cout << "(F) de tim kiem lai | ESC de thoat\n";
        } else {
            if (luaChon >= n) luaChon = max(0, n - 1);
            int tongTrang = (n > 0) ? ((n - 1) / ITEM_PER_PAGE + 1) : 1;
            int trang = luaChon / ITEM_PER_PAGE;

            cout << "--- CHON DOC GIA (Trang " << trang + 1 << "/" << tongTrang << ") ---\n";
            if (searchMode) cout << "Tim kiem: '" << searchKeyword << "'\n";
            cout << "   ┌" << string(12, '-') << "┬" << string(27, '-') << "┬" << string(12, '-') << "┬" << string(17, '-') << "┐" << endl;
            cout << "   │ " << left << setw(10) << "Ma The" << " │ " << left << setw(25) << "Ho Ten" << " │ " << left << setw(10) << "Gioi Tinh" << " │ " << left << setw(15) << "Trang Thai" << " │" << endl;
            cout << "   ├" << string(12, '-') << "┼" << string(27, '-') << "┼" << string(12, '-') << "┼" << string(17, '-') << "┤" << endl;

            int start = trang * ITEM_PER_PAGE;
            int end = min(start + ITEM_PER_PAGE, n);

            for (int i = start; i < end; i++) {
                if (i == luaChon) { setColor(47); cout << ">> "; } 
                else { cout << "   "; }

                string hoTen = string(displayArr[i]->ho) + " " + string(displayArr[i]->ten);
                if (hoTen.length() > 25) hoTen = hoTen.substr(0, 22) + "...";

                cout << "│ " << left << setw(10) << displayArr[i]->maThe << " │ " << left << setw(25) << hoTen << " │ " << left << setw(10) << displayArr[i]->giotinh << " │ " << left << setw(15) << (displayArr[i]->trangThaiThe == 1 ? "Hoat dong" : "Khoa") << " │";
                
                if (i == luaChon) resetColor();
                cout << endl;
            }

            cout << "   └" << string(12, '-') << "┴" << string(27, '-') << "┴" << string(12, '-') << "┴" << string(17, '-') << "┘" << endl;
            cout << "   (Mui ten: Len/Xuong, PgUp/PgDn, Enter: Chon, F: Tim, ESC: Thoat)\n";
        }

        int key = _getch();
        if (key == 'f' || key == 'F') {
            showCursor(true);
            gotoxy(0, 20); cout << string(80, ' ');
            gotoxy(0, 20);
            if (nhapChuoiTuDo("Tim kiem (Ma The/Ten, de trong de xem tat ca): ", searchKeyword, 100)) {
                searchMode = (strlen(searchKeyword) > 0);
                luaChon = 0;
            }
            showCursor(false);
        } else if (key == 224) {
            key = _getch();
            if (key == 72) luaChon = max(0, luaChon - 1); // UP
            else if (key == 80) luaChon = min(n - 1, luaChon + 1); // DOWN
            else if (key == 73) luaChon = max(0, luaChon - ITEM_PER_PAGE); // PGUP
            else if (key == 81) luaChon = min(n - 1, luaChon + 10); // PGDN
        } else if (key == 13) {
            if (n > 0) {
                int maThe = displayArr[luaChon]->maThe;
                delete[] displayArr;
                delete[] fullArr;
                return maThe;
            }
        } else if (key == 27) {
            delete[] displayArr;
            delete[] fullArr;
            return -1;
        }
        delete[] displayArr;
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

Sach* chonCuonSachTuDauSach(DauSach* dauSach) {
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
    
    while (true) {
        system("cls");
        int tongTrang = (n > 0) ? ((n - 1) / ITEM_PER_PAGE + 1) : 1;
        int trang = luaChon / ITEM_PER_PAGE;

        cout << "--- CHON CUON SACH DE MUON (Trang " << trang + 1 << "/" << tongTrang << ") ---\n";
        cout << "Dau sach: " << dauSach->tenSach << "\n\n";
        cout << "   ┌" << string(26, '-') << "┬" << string(22, '-') << "┐" << endl;
        cout << "   │ " << left << setw(24) << "Ma Sach" << " │ " << left << setw(20) << "Vi Tri" << " │" << endl;
        cout << "   ├" << string(26, '-') << "┼" << string(22, '-') << "┤" << endl;

        int start = trang * ITEM_PER_PAGE;
        int end = min(start + ITEM_PER_PAGE, n);

        for (int i = start; i < end; i++) {
            if (i == luaChon) { setColor(47); cout << ">> "; } 
            else { cout << "   "; }
            cout << "│ " << left << setw(24) << availableBooks[i]->maSach << " │ " << left << setw(20) << availableBooks[i]->viTri << " │";
            if (i == luaChon) resetColor();
            cout << endl;
        }
        cout << "   └" << string(26, '-') << "┴" << string(22, '-') << "┘" << endl;
        cout << "   (Mui ten: Chon, Enter: Xac nhan, ESC: Huy)\n";

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

void formNhapDauSach(ListDauSach& ds, DauSach* dsPtr, bool isThemMoi) {
    int winX = 15, winY = 2, winW = 60, winH = 14;
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

        gotoxy(winX + 17, winY + 3); cout << string(35, ' '); gotoxy(winX + 17, winY + 3); cout << isbn;
        gotoxy(winX + 17, winY + 4); cout << string(35, ' '); gotoxy(winX + 17, winY + 4); cout << tenSach;
        gotoxy(winX + 17, winY + 5); cout << string(35, ' '); gotoxy(winX + 17, winY + 5); cout << tacGia;
        gotoxy(winX + 17, winY + 6); cout << string(35, ' '); gotoxy(winX + 17, winY + 6); cout << theLoai;
        gotoxy(winX + 17, winY + 7); cout << string(35, ' '); gotoxy(winX + 17, winY + 7); cout << soTrang;
        gotoxy(winX + 17, winY + 8); cout << string(35, ' '); gotoxy(winX + 17, winY + 8); cout << namXB;

        int status;
        switch (currentField) {
            case 0: status = isThemMoi ? nhapChuoiForm(winX + 17, winY + 3, isbn, 20) : INPUT_DOWN; break;
            case 1: status = nhapChuoiForm(winX + 17, winY + 4, tenSach, 100, false, true); break;
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
                formNhapDauSach(ds, nullptr, true);
                showCursor(false);
                cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
            } else if (key == 'f' || key == 'F') {
                system("cls"); showCursor(true);
                if (nhapChuoiTuDo("Tim kiem (ISBN/Ten/Tac gia): ", searchKeyword, 100)) {
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
        cout << "\n   (T)-Thêm | (S)-Sửa | (X)-Xóa | (C)-Cuốn sách | (F)-Tìm | (I)-In DS | Mui ten | ESC: Quay lai\n";
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
        else if (key == 't' || key == 'T') { formNhapDauSach(ds, nullptr, true); actionTaken = true; }
        else if (key == 's' || key == 'S') { formNhapDauSach(ds, displayArr[luaChon], false); actionTaken = true; }
        else if (key == 'x' || key == 'X') { system("cls"); showCursor(true); cout << "Ban co chac chan muon xoa dau sach ISBN: " << displayArr[luaChon]->ISBN << " khong? (Y/N): "; char confirm = _getch(); cout << endl; if (toupper(confirm) == 'Y') xoaDauSach(ds, displayArr[luaChon]->ISBN); else cout << "Da huy thao tac xoa.\n"; actionTaken = true; }
        else if (key == 'c' || key == 'C') {
            system("cls");
            inDanhSachCuonSach(displayArr[luaChon]); 
            cout << "\n\n(T)-Thêm cuốn sách | ESC: Quay lai\n";
            int k = _getch();
            if (k == 't' || k == 'T') { system("cls"); themCuonSach(displayArr[luaChon]); }
            actionTaken = true;
        }
        else if (key == 'f' || key == 'F') { system("cls"); showCursor(true); if (nhapChuoiTuDo("Tim kiem (ISBN/Ten/Tac gia): ", searchKeyword, 100)) { searchMode = (strlen(searchKeyword) > 0); luaChon = 0; } }
        else if (key == 'i' || key == 'I') {
            system("cls"); showCursor(true); inTheoTheLoai_TrongDoTenTangDan(ds); actionTaken = true;
        }
        
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
            cout << "   (T)-Thêm | (F)-Tìm | ESC: Quay lai\n";
            resetColor();
            
            int key = _getch();
            if (key == 't' || key == 'T') {
                formNhapDocGia(ql, nullptr, true);
                showCursor(false);
                cout << "\nNhan phim bat ky de tiep tuc..."; _getch();
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
        cout << "\n   (T)-Thêm | (S)-Sửa | (X)-Xóa | (K)-Khóa/Mở | (F)-Tìm | (I)-In DS | Mui ten | ESC: Quay lai\n";
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
            formNhapDocGia(ql, nullptr, true);
            actionTaken = true;
        }
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

        system("cls");
        int ma = chonTuBangDocGia(qlDocGia);
        if (ma == -1) continue;

        DocGia* dg = timDocGia(qlDocGia.root, ma);
        if (dg) {
            showCursor(true);
            system("cls");
            cout << "=== DOC GIA: " << dg->ho << " " << dg->ten << " ===\n";
            xemSachDangMuon(dg, ds);
            cout << "------------------------------------------\n";

            if (chonPhai == 0) { // Muon sach
                if (dg->trangThaiThe == 0) cout << "The dang bi KHOA!\n";
                else if (dg->soSachDangMuon >= 3) cout << "Da muon toi da 3 cuon!\n";
                else {
                    DauSach* dauSachChon = chonTuBangDauSach(ds);
                    if (dauSachChon != nullptr) {
                        Sach* cuonSachChon = chonCuonSachTuDauSach(dauSachChon);
                        if (cuonSachChon != nullptr) {
                            muonSach(dg, ds, cuonSachChon->maSach);
                        } else {
                            cout << "Da huy chon cuon sach.\n";
                        }
                    } else {
                        cout << "Da huy chon dau sach.\n";
                    }
                }
            } else if (chonPhai == 1) { // Tra sach
                if (dg->soSachDangMuon == 0) cout << "Doc gia nay khong muon sach nao de tra.\n";
                else {
                    char ms[25]; cout << endl;
                    if (nhapMa("Nhap ma sach can TRA: ", ms, 25)) traSach(dg, ds, ms, layNgayHienTai());
                    else cout << "Da huy tra sach.\n";
                }
            } else if (chonPhai == 2) { // Bao mat
                char ms[25]; cout << endl;
                if (nhapMa("Nhap ma sach bao MAT: ", ms, 25)) baoMatSach(dg, ds, ms);
                else cout << "Da huy bao mat.\n";
            }
            cout << "\n\nNhan phim bat ky de quay lai...";
            _getch();
        } else {
            cout << "Loi: Khong tim thay doc gia!\n";
            cout << "\n\nNhan phim bat ky de quay lai...";
            _getch();
        }
    }
}

void thongKeUI(QuanLyDocGia& qlDocGia, ListDauSach& ds) {
    const char* subThongKe[] = { "Doc gia qua han", "Top 10 sach", "Thong ke the loai", "<- Quay lai" };
    int indexPhai = 0;

    while (true) {
        veKhungGiaoDien();
        veMenuList(menuTrai, 6, 4, 2, 6, false);
        xoaVung(28, 4, 50, 18);
        gotoxy(40, 4); setColor(33); cout << "--- THONG KE ---";

        int chonPhai = chonMenu(subThongKe, 4, 35, 7, indexPhai);
        if (chonPhai == 3 || chonPhai == -1) return;

        system("cls");
        showCursor(true);
        if (chonPhai == 0) lietKeDocGiaQuaHan(qlDocGia);
        else if (chonPhai == 1) inTop10SachMuonNhieu(ds);
        if (chonPhai == 0) lietKeDocGiaQuaHan(qlDocGia); 
        else if (chonPhai == 1) {
            capNhatSoLuotMuonCuaDauSach(qlDocGia.root, ds); // Tinh toan lai so luot muon hien tai
            inTop10SachMuonNhieu(ds);
        }
        else if (chonPhai == 2) thongKeSoLuongTheoTheLoai(ds);
        
        cout << "\n\nNhan phim bat ky de quay lai...";
        _getch();
        showCursor(false);
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

        switch (chonChinh) {
            case 0: continue; // Tong quan
            case 1: quanLyDocGiaUI(qlDocGia); break;
            case 2: quanLySachUI(ds); break;
            case 3: giaoDichUI(qlDocGia, ds); break;
            case 4: thongKeUI(qlDocGia, ds); break;
        }
    }
}