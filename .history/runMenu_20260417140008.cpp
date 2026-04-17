#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
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
#include <windows.h>
#include <fstream>


using namespace std;

// =========================================================================
// 1. BỘ CÔNG CỤ VẼ GIAO DIỆN TÁCH BIỆT (KHÔNG LÀM RỐI LOGIC)
// =========================================================================
void gotoxy(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
}

// Ham de quy dem so sach dang muon va so doc gia qua han
void xoaVung(int x, int y, int width, int height) {
    for (int i = 0; i < height; i++) {
        gotoxy(x, y + i);
        for (int j = 0; j < width; j++) cout << " ";
    }
}

void demThongKeTongQuan(TREE_DocGia root, int& soSachDangMuon, int& soDocGiaQuaHan) {
    if (root == nullptr) return;
    demThongKeTongQuan(root->pLeft, soSachDangMuon, soDocGiaQuaHan);
    
    soSachDangMuon += root->soSachDangMuon;
    if (coSachQuaHan(root)) {
        soDocGiaQuaHan++;
    }
    
    demThongKeTongQuan(root->pRight, soSachDangMuon, soDocGiaQuaHan);
}

void hienThiTongQuan(const QuanLyDocGia& ql, const ListDauSach& ds) {
    int tongDauSach = ds.n;
    int tongDocGia = ql.soLuongDocGia;
    int soSachDangMuon = 0;
    int soDocGiaQuaHan = 0;

    demThongKeTongQuan(ql.root, soSachDangMuon, soDocGiaQuaHan);

    gotoxy(28, 4); cout << "\033[1;33m--- TONG QUAN HE THONG ---\033[0m";

    // Box 1: Tong Dau Sach
    gotoxy(30, 7); cout << "\033[1;36m╔══════════════════════╗\033[0m";
    gotoxy(30, 8); cout << "\033[1;36m║\033[0m     Tong Dau Sach    \033[1;36m║\033[0m";
    gotoxy(30, 9); cout << "\033[1;36m║\033[0m                      \033[1;36m║\033[0m";
    gotoxy(30, 10); cout << "\033[1;36m╚══════════════════════╝\033[0m";
    gotoxy(38, 9); cout << "\033[1;37m" << tongDauSach << "\033[0m";

    // Box 2: Tong Doc Gia
    gotoxy(55, 7); cout << "\033[1;36m╔══════════════════════╗\033[0m";
    gotoxy(55, 8); cout << "\033[1;36m║\033[0m     Tong Doc Gia     \033[1;36m║\033[0m";
    gotoxy(55, 9); cout << "\033[1;36m║\033[0m                      \033[1;36m║\033[0m";
    gotoxy(55, 10); cout << "\033[1;36m╚══════════════════════╝\033[0m";
    gotoxy(63, 9); cout << "\033[1;37m" << tongDocGia << "\033[0m";

    // Box 3: Dang Muon
    gotoxy(30, 13); cout << "\033[1;33m╔══════════════════════╗\033[0m";
    gotoxy(30, 14); cout << "\033[1;33m║\033[0m   Sach Dang Muon     \033[1;33m║\033[0m";
    gotoxy(30, 15); cout << "\033[1;33m║\033[0m                      \033[1;33m║\033[0m";
    gotoxy(30, 16); cout << "\033[1;33m╚══════════════════════╝\033[0m";
    gotoxy(38, 15); cout << "\033[1;37m" << soSachDangMuon << "\033[0m";

    // Box 4: Qua Han
    gotoxy(55, 13); cout << "\033[1;31m╔══════════════════════╗\033[0m";
    gotoxy(55, 14); cout << "\033[1;31m║\033[0m   Doc Gia Qua Han    \033[1;31m║\033[0m";
    gotoxy(55, 15); cout << "\033[1;31m║\033[0m                      \033[1;31m║\033[0m";
    gotoxy(55, 16); cout << "\033[1;31m╚══════════════════════╝\033[0m";
    gotoxy(63, 15); cout << "\033[1;37m" << soDocGiaQuaHan << "\033[0m";
}

void gotoxy(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
}

void xoaVung(int x, int y, int width, int height) {
    for (int i = 0; i < height; i++) {
        gotoxy(x, y + i);
        for (int j = 0; j < width; j++) cout << " ";
    }
}

void veKhungGiaoDien() {
    system("cls");
    cout << "\033[1;36m"; // Mau xanh lo
    cout << "\033[1;36m";
    cout << "================================================================================\n";
    cout << "||                     HE THONG QUAN LY THU VIEN CAO CAP                      ||\n";
    cout << "================================================================================\n";
    for (int i = 4; i <= 20; i++) {
        gotoxy(25, i);
        cout << "||";
    }
    gotoxy(1, 21);
    cout << "================================================================================\n";
    cout << "\033[0m"; 
    cout << "\033[0m";
}

// Hàm quản lý thanh sáng (highlight) di chuyển bằng mũi tên
int chonMenuThongMinh(const vector<string>& menu, int toaDoX, int toaDoY, int &luaChonHienTai, bool isFocus) {
    int key;
    while (true) {
        for (size_t i = 0; i < menu.size(); i++) {
            gotoxy(toaDoX, toaDoY + i * 2);
            if (i == (size_t)luaChonHienTai && isFocus) {
                cout << "\033[1;32m> " << menu[i] << " \033[0m"; // Xanh la: dang chon
                cout << "\033[1;32m> " << menu[i] << " \033[0m";
            } else if (i == (size_t)luaChonHienTai && !isFocus) {
                cout << "\033[1;33m  " << menu[i] << " \033[0m"; // Vang: muc dang hoat dong ben trai
                cout << "\033[1;33m  " << menu[i] << " \033[0m";
            } else {
                cout << "  " << menu[i] << "   ";
            }
        }
        if (!isFocus) return -1;

        if (!isFocus) return -1; // Neu khong focus thi in ra roi thoat de code chay tiep

        key = _getch();
        if (key == 224) { 
            key = _getch();
            if (key == 72) luaChonHienTai = (luaChonHienTai - 1 + menu.size()) % menu.size(); 
            if (key == 80) luaChonHienTai = (luaChonHienTai + 1) % menu.size();        
        } 
        else if (key == 13) return luaChonHienTai; // Enter
        else if (key == 27) return menu.size() - 1; // ESC mac dinh la Quay lai (Muc cuoi cung)
        else if (key == 13) return luaChonHienTai;
        else if (key == 27) return menu.size() - 1;
    }
}

// =========================================================================
// 2. LOGIC ĐIỀU HƯỚNG CHÍNH
// =========================================================================

void runMenu()
{
    QuanLyDocGia qlDocGia;
    ListDauSach ds;
    ds.n = 0;

    loadDauSach("Input_file/DauSach.txt", ds);
    loadDocGia("Input_file/DocGia.txt", qlDocGia);

    capNhatTrangThaiSachDangMuon(qlDocGia.root, ds);
    capNhatSoLuotMuonCuaDauSach(qlDocGia.root, ds);

    khoiTaoKhoMaThe(qlDocGia.root);

    // Mảng cấu hình các Menu (Dùng mảng để code không bị dài dòng)
    const vector<string> menuTrai = { "Tong quan", "Quan ly doc gia", "Quan ly sach", "Giao dich", "Thong ke", "Thoat chuong trinh" };
    int indexTrai = 0;

    const vector<string> menuDocGia = { "Them doc gia", "Xoa doc gia", "Hieu chinh", "Khoa / Mo the", "In danh sach", "<- Quay lai" };
    const vector<string> menuSach = { "Them dau sach", "Hieu chinh", "Xoa dau sach", "Them cuon sach", "Tra cuu ten", "In danh sach", "In the loai", "<- Quay lai" };
    const vector<string> menuGiaoDich = { "Muon sach", "Tra sach", "Bao mat sach", "Xem sach dang muon", "<- Quay lai" };
    const vector<string> menuThongKe = { "Doc gia qua han", "Top 10 sach muon nhieu", "Thong ke the loai", "<- Quay lai" };

    veKhungGiaoDien();

    bool isRunning = true;
    while (isRunning) {
        
        // --- BƯỚC 1: ĐIỀU KHIỂN MENU BÊN TRÁI ---
        xoaVung(28, 4, 50, 16); // Xóa menu phụ cũ bên phải
        xoaVung(28, 4, 50, 16);
        if (indexTrai == 0) {
            hienThiTongQuan(qlDocGia, ds);
        } else {
            gotoxy(28, 4); cout << "\033[1;30m(Dung mui ten Up/Down de chon, Enter de xac nhan)\033[0m";
        }
        
        chonMenuThongMinh(menuTrai, 3, 6, indexTrai, true);

        if (indexTrai == 5) { // Thoát
        if (indexTrai == 5) {
            system("cls");
            saveDocGia("Input_file/DocGia.txt", qlDocGia.root);
            saveDauSach("Input_file/DauSach.txt", ds);
            luuKhoMaThe();
            cout << "\n\033[1;32mDa luu du lieu! Tam biet!\033[0m\n\n";
            break;
        }

        // Neu chon "Tong quan", chi hien thi, khong vao menu con
        if (indexTrai == 0) {
            continue;
        }

        // --- BƯỚC 2: ĐIỀU KHIỂN MENU BÊN PHẢI ---
        int indexPhai = 0;
        bool inSubMenu = true;

        while (inSubMenu) {
            veKhungGiaoDien();
            // Vẽ lại menu trái nhưng tô màu vàng để biết đang ở mục nào
            chonMenuThongMinh(menuTrai, 3, 6, indexTrai, false); 
            
            int chonPhai = -1;

            // Xử lý Giao diện Menu Phụ tương ứng
            if (indexTrai == 1) {
                gotoxy(28, 4); cout << "\033[1;33m--- QUAN LY DOC GIA ---\033[0m";
                chonPhai = chonMenuThongMinh(menuDocGia, 28, 6, indexPhai, true);
            } 
            else if (indexTrai == 2) {
                gotoxy(28, 4); cout << "\033[1;33m--- QUAN LY SACH ---\033[0m";
                chonPhai = chonMenuThongMinh(menuSach, 28, 6, indexPhai, true);
            }
            else if (indexTrai == 3) {
                gotoxy(28, 4); cout << "\033[1;33m--- GIAO DICH ---\033[0m";
                chonPhai = chonMenuThongMinh(menuGiaoDich, 28, 6, indexPhai, true);
            }
            else if (indexTrai == 4) {
                gotoxy(28, 4); cout << "\033[1;33m--- THONG KE ---\033[0m";
                chonPhai = chonMenuThongMinh(menuThongKe, 28, 6, indexPhai, true);
            }

            // Kiểm tra nếu chọn "Quay lại" hoặc ấn phím ESC (Giá trị trả về là vị trí cuối hoặc -1)
            if ( (indexTrai == 1 && chonPhai == 5) || 
                 (indexTrai == 2 && chonPhai == 7) || 
                 (indexTrai == 3 && chonPhai == 4) || 
                 (indexTrai == 4 && chonPhai == 3) || chonPhai == -1 ) {
                inSubMenu = false; 
                continue; // Phá vòng lặp menu phụ, trở về focus menu bên trái
                continue;
            }

            // --- BƯỚC 3: THỰC THI NGHIỆP VỤ ---
            // Bí quyết: Xóa sạch màn hình trước khi chạy nghiệp vụ để input không bị đè vào khung vẽ
            system("cls"); 
            cout << "\n\033[1;36m===== CHUC NANG DANG CHAY =====\033[0m\n\n";

            if (indexTrai == 1) { // Quan ly Doc Gia
            if (indexTrai == 1) {
                switch (chonPhai) {
                    case 0: nhapDocGia(qlDocGia); break;
                    case 1: {
                        int ma = nhapSoNguyenHopLe("Nhap ma the: ");
                        DocGia* dg = timDocGia(qlDocGia.root, ma);
                        if (dg == nullptr) cout << "Khong tim thay doc gia co ma the: " << ma << "\n";
                        else {
                            cout << "Ban co chac chan muon xoa doc gia mang ten " << dg->ho << " " << dg->ten << " khong? (Y/N): ";
                            char confirm; cin >> confirm; cin.ignore(1000,'\n');
                            if (toupper(confirm) == 'Y') xoaDocGia(qlDocGia, ma);
                            else cout << "Huy xoa doc gia.\n";
                        }
                        break;
                    }
                    case 2: hieuChinhDocGia(qlDocGia.root, nhapSoNguyenHopLe("Nhap ma the: ")); break;
                    case 3: khoaMoThe(qlDocGia.root, nhapSoNguyenHopLe("Nhap ma the: ")); break;
                    case 4: {
                        cout << "1. Theo ma the\n2. Theo ten\n";
                        int opt = nhapSoNguyenHopLe("Chon: ");
                        if (opt == 1) inDanhSachDocGia(qlDocGia.root); else inTheoTen(qlDocGia);
                        break;
                    }
                }
            } 
            else if (indexTrai == 2) { // Quan Ly Sach
            else if (indexTrai == 2) {
                switch (chonPhai) {
                    case 0: themDauSach(ds); break;
                    case 1: hieuChinhDauSach(ds); break;
                    case 2: xoaDauSachUI(ds); break;
                    case 3: {
                        char isbn[20]; cout << "Nhap ISBN: "; cin.getline(isbn,20); chuanHoaISBN(isbn);
                        DauSach* d = timTheoISBN(ds,isbn);
                        if (d) themCuonSach(d); else cout << "Khong tim thay dau sach voi ISBN nay!\n";
                        break;
                    }
                    case 4: {
                        char ten[100]; cout << "Nhap ten: "; cin.getline(ten,100); traCuuTheoTenNangCao(ds,ten); break;
                    }
                    case 5: inDanhSachDauSach(ds); break;
                    case 6: inTheoTheLoai_TrongDoTenTangDan(ds); break;
                }
            }
            else if (indexTrai == 3) { // Giao Dich
            else if (indexTrai == 3) {
                switch (chonPhai) {
                    case 0: {
                        int ma = nhapSoNguyenHopLe("Ma doc gia: ");
                        DocGia* dg = timDocGia(qlDocGia.root, ma);
                        if (dg == nullptr) { cout << "Khong tim thay doc gia co ma the: " << ma << "\n"; break; }
                        if (dg->trangThaiThe == 0) { cout << "The doc gia dang bi khoa! Khong the muon sach.\n"; break; }
                        if (dg->soSachDangMuon >= MAX_SACH_MUON) { cout << "Doc gia da muon toi da 3 cuon! Khong the muon them.\n"; break; }
                        if (coSachQuaHan(dg)) { cout << "Ban dang co sach qua han! Vui long tra sach truoc khi muon sach moi!\n"; break; }
                        if (dg->soSachDangMuon > 0) { cout << "Danh sach cac cuon sach doc gia nay dang muon:\n"; xemSachDangMuon(dg, ds); }
                        char ms[25]; cout << "Ma sach can muon: "; cin.getline(ms,25);
                        if (muonSach(dg, ds, ms)) { saveDocGia("Input_file/DocGia.txt", qlDocGia.root); saveDauSach("Input_file/DauSach.txt", ds); } 
                        else cout << "Giao dich that bai, khong luu file!\n";
                        break;
                    }
                    case 1: {
                        int ma = nhapSoNguyenHopLe("Ma doc gia: ");
                        DocGia* dg = timDocGia(qlDocGia.root, ma);
                        if (dg == nullptr) { cout << "Khong tim thay doc gia co ma the: " << ma << "\n"; break; }
                        if (dg->soSachDangMuon == 0) { cout << "Doc gia nay hien khong muon sach nao!\n"; break; }
                        cout << "Danh sach cac cuon sach doc gia nay dang muon:\n"; xemSachDangMuon(dg, ds);
                        char ms[25]; cout << "Ma sach can tra: "; cin.getline(ms,25);
                        if (traSach(dg, ds, ms, layNgayHienTai())) { saveDocGia("Input_file/DocGia.txt", qlDocGia.root); saveDauSach("Input_file/DauSach.txt", ds); } 
                        else cout << "Giao dich that bai, khong luu file!\n";
                        break;
                    }
                    case 2: {
                        int ma = nhapSoNguyenHopLe("Ma doc gia: ");
                        DocGia* dg = timDocGia(qlDocGia.root, ma);
                        if (dg == nullptr) { cout << "Khong tim thay doc gia co ma the: " << ma << "\n"; break; }
                        if (dg->soSachDangMuon == 0) { cout << "Doc gia nay hien khong muon sach nao!\n"; break; }
                        cout << "Danh sach cac cuon sach doc gia nay dang muon:\n"; xemSachDangMuon(dg, ds);
                        char ms[25]; cout << "Ma sach bi mat: "; cin.getline(ms,25);
                        if (baoMatSach(dg, ds, ms)) { saveDocGia("Input_file/DocGia.txt", qlDocGia.root); saveDauSach("Input_file/DauSach.txt", ds); } 
                        else cout << "Giao dich that bai, khong luu file!\n";
                        break;
                    }
                    case 3: {
                        int ma = nhapSoNguyenHopLe("Nhap ma doc gia: ");
                        DocGia* dg = timDocGia(qlDocGia.root, ma);
                        if (dg) { cout << "Danh sach cac cuon sach doc gia " << dg->ho << " " << dg->ten << " dang muon:\n"; xemSachDangMuon(dg, ds); } 
                        else cout << "Khong tim thay doc gia!\n";
                        break;
                    }
                }
            }
            else if (indexTrai == 4) { // Thong Ke
            else if (indexTrai == 4) {
                switch (chonPhai) {
                    case 0: lietKeDocGiaQuaHan(qlDocGia); break;
                    case 1: inTop10SachMuonNhieu(ds); break;
                    case 2: thongKeSoLuongTheoTheLoai(ds); break;
                }
            }
            
            // --- BƯỚC 4: TẠM DỪNG VÀ PHỤC HỒI GIAO DIỆN ---
            cout << "\n\n\033[1;32mHoan tat! An phim bat ky de quay ve Menu...\033[0m";
            _getch();
            // Khi vòng lặp chạy lại, khung giao diện sẽ được vẽ mới tự động
        }
    }

    giaiPhongCay(qlDocGia.root);
    giaiPhongDanhSachDauSach(ds);
}
