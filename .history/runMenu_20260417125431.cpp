#include <iostream>
#include "InputUtils.h"
#include "Structs.h"
#include "Docgia.h"
#include "Sach.h"
#include "Giaodich.h"
#include "Thongke.h"
#include "File.h"
#include "Test.h"
#include "runMenu.h"
#include "MuonTra.h"

using namespace std;


#ifdef _WIN32
#include <windows.h>
void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004;
    SetConsoleMode(hOut, dwMode);
}
#else
void enableANSI() {}
#endif

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define WHITE   "\033[97m"
#define BLUE_BG "\033[44m"
#define BLACK_BG "\033[40m"
#define GREEN_BG "\033[42m"
#define RED_BG "\033[41m"
#define YELLOW_BG "\033[43m"

#include <vector>
#include <string>

using std::vector;
using std::string;

void printSidebar(const vector<string>& items, int selected) {
    std::cout << BLUE_BG << WHITE;
    std::cout << "┌───────────────────────┐\n";
    std::cout << "│      LibManager       │\n";
    std::cout << "├───────────────────────┤\n";
    for (size_t i = 0; i < items.size(); ++i) {
        if ((int)i == selected)
            std::cout << YELLOW_BG << BLACK_BG;
        else
            std::cout << BLUE_BG << WHITE;
        printf("│ %-20s │\n", items[i].c_str());
    }
    std::cout << BLUE_BG << WHITE;
    std::cout << "└───────────────────────┘\n";
    std::cout << RESET;
}

void printHeader(const string& title) {
    std::cout << BOLD << CYAN;
    std::cout << "╔════════════════════════════════════════════════════╗\n";
    printf("║ %-48s ║\n", title.c_str());
    std::cout << "╚════════════════════════════════════════════════════╝\n";
    std::cout << RESET;
}

void printDashboard() {
    std::cout << " Tổng Đầu Sách: " << BOLD << "1204" << RESET
         << " | Tổng Độc Giả: " << BOLD << "850" << RESET
         << " | Đang Mượn: " << BOLD << "142" << RESET
         << " | Quá Hạn: " << BOLD << "12" << RESET << "\n";
    std::cout << "────────────────────────────────────────────────────\n";
    std::cout << " Giao Dịch Gần Đây:\n";
    std::cout << "  Độc Giả         | Tên Sách                | Ngày Mượn  | Trạng Thái\n";
    std::cout << "────────────────────────────────────────────────────\n";
    std::cout << "  Nguyễn Văn A    | Clean Code              | 15/04/2026 | Đang mượn\n";
    std::cout << "  Trần Thị B      | Effective C++           | 10/04/2026 | Đã trả\n";
    std::cout << "  Lê Hoàng C      | Cấu Trúc Dữ Liệu        | 01/04/2026 | Quá hạn\n";
}

int menu(const vector<string>& sidebar, int& selected) {
    system("cls");
    printSidebar(sidebar, selected);
    printHeader("BẢNG ĐIỀU KHIỂN");
    printDashboard();
    std::cout << "\nDùng phím số để chọn mục, hoặc phím mũi tên (giả lập):\n";
    for (size_t i = 0; i < sidebar.size(); ++i) {
        std::cout << "  " << (i+1) << ". " << sidebar[i] << (selected == (int)i ? "  <==" : "") << "\n";
    }
    std::cout << "  0. Thoát\n";
    int ch = nhapSoNguyenHopLe("Chọn: ");
    if (ch >= 1 && ch <= (int)sidebar.size()) selected = ch-1;
    return ch;
}

void runMenu()
{
    QuanLyDocGia qlDocGia;
    ListDauSach ds;
    ds.n = 0;

    loadDauSach("Input_file/DauSach.txt", ds);
    loadDocGia("Input_file/DocGia.txt", qlDocGia);

    capNhatTrangThaiSachDangMuon(qlDocGia.root, ds);
    capNhatSoLuotMuonCuaDauSach(qlDocGia.root, ds);

    cout << "Da load du lieu thanh cong!\n";

    khoiTaoKhoMaThe(qlDocGia.root);


    enableANSI();
    vector<string> sidebar = {
        "Quản Lý Độc Giả",
        "Quản Lý Sách",
        "Giao Dịch",
        "Thống Kê"
    };
    int selected = 0;
    int choice;
    do {
        choice = menu(sidebar, selected);

        switch (choice) {

        // ================= DOC GIA =================
        case 1:
        {
            int c;
            cout << "\n--- QUAN LY DOC GIA ---\n";
            cout << "1. Them doc gia\n";
            cout << "2. Xoa doc gia\n";
            cout << "3. Hieu chinh\n";
            cout << "4. Khoa / Mo the\n";
            cout << "5. In danh sach\n";
            c = nhapSoNguyenHopLe("Chon: ");

            switch (c) {
            case 1: nhapDocGia(qlDocGia); break;

            case 2: {
                int ma = nhapSoNguyenHopLe("Nhap ma the: ");
                
                DocGia* dg = timDocGia(qlDocGia.root, ma);
                if (dg == nullptr) {
                    cout << "Khong tim thay doc gia co ma the: " << ma << "\n";
                    break;
                }

                cout << "Ban co chac chan muon xoa doc gia mang ten " << dg->ho << " " << dg->ten << " khong? (Y/N): ";
                char confirm;
                cin >> confirm; cin.ignore(1000,'\n');
                if (toupper(confirm) == 'Y') {
                    xoaDocGia(qlDocGia, ma);
                } else {
                    cout << "Huy xoa doc gia.\n";
                }
                break;
            }

            case 3: {
                int ma = nhapSoNguyenHopLe("Nhap ma the: ");
                hieuChinhDocGia(qlDocGia.root, ma);
                break;
            }

            case 4: {
                int ma = nhapSoNguyenHopLe("Nhap ma the: ");
                khoaMoThe(qlDocGia.root, ma);
                break;
            }

            case 5: {
                cout << "1. Theo ma the\n2. Theo ten\n";
                int opt = nhapSoNguyenHopLe("Chon: ");

                if (opt == 1) inDanhSachDocGia(qlDocGia.root);
                else inTheoTen(qlDocGia);
                break;
            }
            }
            break;
        }

        // ================= SACH =================
        case 2:
        {
            int c;
            cout << "\n--- QUAN LY SACH ---\n";
            cout << "1. Them dau sach\n";
            cout << "2. Hieu chinh dau sach\n";
            cout << "3. Xoa dau sach\n";
            cout << "4. Them cuon sach\n";
            cout << "5. Tra cuu theo ten\n";
            cout << "6. In danh sach\n";
            cout << "7. In theo the loai\n";
            c = nhapSoNguyenHopLe("Chon: ");

            switch (c) {
            case 1: themDauSach(ds); break;

            case 2: hieuChinhDauSach(ds); break;

            case 3: xoaDauSachUI(ds); break;

            case 4: {
                char isbn[20];
                cout << "Nhap ISBN: ";
                cin.getline(isbn,20);
                chuanHoaISBN(isbn);

                DauSach* d = timTheoISBN(ds,isbn);
                if (d) themCuonSach(d);
                else cout << "Khong tim thay dau sach voi ISBN nay!\n";
                break;
            }

            case 5: {
                char ten[100];
                cout << "Nhap ten: ";
                cin.getline(ten,100);
                traCuuTheoTenNangCao(ds,ten);
                break;
            }

            case 6:
                inDanhSachDauSach(ds);
                break;

            case 7:
                inTheoTheLoai_TrongDoTenTangDan(ds);
                break;
            }
            break;
        }
        // ================= GIAO DICH =================
        case 3:
        {
            int c;
            cout << "\n--- GIAO DICH ---\n";
            cout << "1. Muon sach\n";
            cout << "2. Tra sach\n";
            cout << "3. Bao mat\n";
            cout << "4. Xem sach dang muon\n";
            c = nhapSoNguyenHopLe("Chon: ");

            switch (c) {

            case 1: {
                int ma = nhapSoNguyenHopLe("Ma doc gia: ");

                DocGia* dg = timDocGia(qlDocGia.root, ma);
                if (dg == nullptr) {
                    cout << "Khong tim thay doc gia co ma the: " << ma << "\n";
                    break;
                }

                if (dg->trangThaiThe == 0) {
                    cout << "The doc gia dang bi khoa! Khong the muon sach.\n";
                    break;
                }

                if (dg->soSachDangMuon >= MAX_SACH_MUON) {
                    cout << "Doc gia da muon toi da 3 cuon! Khong the muon them.\n";
                    break;
                }

                if (coSachQuaHan(dg)) {
                    cout << "Ban dang co sach qua han! Vui long tra sach truoc khi muon sach moi!\n";
                    break;
                }

                if (dg->soSachDangMuon > 0) {
                    cout << "Danh sach cac cuon sach doc gia nay dang muon:\n";
                    xemSachDangMuon(dg, ds);
                }

                char ms[25];
                cout << "Ma sach can muon: ";
                cin.getline(ms,25);

                if (muonSach(dg, ds, ms)) {
                    saveDocGia("Input_file/DocGia.txt", qlDocGia.root);
                    saveDauSach("Input_file/DauSach.txt", ds);
                } else {
                    cout << "Giao dich that bai, khong luu file!\n";
                }
                break;
            }

            case 2: {
                int ma = nhapSoNguyenHopLe("Ma doc gia: ");

                DocGia* dg = timDocGia(qlDocGia.root, ma);
                if (dg == nullptr) {
                    cout << "Khong tim thay doc gia co ma the: " << ma << "\n";
                    break;
                }
                
                if (dg->soSachDangMuon == 0) {
                    cout << "Doc gia nay hien khong muon sach nao!\n";
                    break;
                }

                cout << "Danh sach cac cuon sach doc gia nay dang muon:\n";
                xemSachDangMuon(dg, ds);

                char ms[25];
                cout << "Ma sach can tra: ";
                cin.getline(ms,25);

                if (traSach(dg, ds, ms, layNgayHienTai())) {
                    saveDocGia("Input_file/DocGia.txt", qlDocGia.root);
                    saveDauSach("Input_file/DauSach.txt", ds);
                } else {
                    cout << "Giao dich that bai, khong luu file!\n";
                }

                break;
            }

            case 3: {
                int ma = nhapSoNguyenHopLe("Ma doc gia: ");

                DocGia* dg = timDocGia(qlDocGia.root, ma);
                if (dg == nullptr) {
                    cout << "Khong tim thay doc gia co ma the: " << ma << "\n";
                    break;
                }
                
                if (dg->soSachDangMuon == 0) {
                    cout << "Doc gia nay hien khong muon sach nao!\n";
                    break;
                }

                cout << "Danh sach cac cuon sach doc gia nay dang muon:\n";
                xemSachDangMuon(dg, ds);

                char ms[25];
                cout << "Ma sach bi mat: ";
                cin.getline(ms,25);

                if (baoMatSach(dg, ds, ms)) {
                    saveDocGia("Input_file/DocGia.txt", qlDocGia.root);
                    saveDauSach("Input_file/DauSach.txt", ds);
                } else {
                    cout << "Giao dich that bai, khong luu file!\n";
                }
                
                break;
            }

            case 4: {
                int ma = nhapSoNguyenHopLe("Nhap ma doc gia: ");

                DocGia* dg = timDocGia(qlDocGia.root, ma);
                if (dg) {
                    cout << "Danh sach cac cuon sach doc gia " << dg->ho << " " << dg->ten << " dang muon:\n";
                    xemSachDangMuon(dg, ds);
                } else {
                    cout << "Khong tim thay doc gia!\n";
                }
                break;
            }
            }
            break;
        }

        // ================= THONG KE =================
        case 4: {
            int c;
            cout << "\n--- THONG KE ---\n";
            cout << "1. Doc gia qua han\n";
            cout << "2. Top 10 sach muon nhieu\n";
            cout << "3. Thong ke theo the loai\n";
            c = nhapSoNguyenHopLe("Chon: ");

            switch (c) {
            case 1:
                lietKeDocGiaQuaHan(qlDocGia);
                break;

            case 2:
                inTop10SachMuonNhieu(ds);
                break;

            case 3:
                thongKeSoLuongTheoTheLoai(ds);
                break;
            }
            break;
        }
        case 0:
            saveDocGia("Input_file/DocGia.txt", qlDocGia.root);
            saveDauSach("Input_file/DauSach.txt", ds);
            luuKhoMaThe();
            cout << "Da luu du lieu!\n";
            cout << "Tam biet!\n";
            break;

        default:
            cout << "Sai lua chon!\n";
        }

    } while (choice != 0);

    giaiPhongCay(qlDocGia.root);
    giaiPhongDanhSachDauSach(ds);
}
