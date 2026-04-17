#include <iostream>
#include <conio.h>
#include <string>
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

// =========================================================================
// 1. CÔNG CỤ VẼ GIAO DIỆN TERMINAL (TUI)
// =========================================================================

// Ham di chuyen con tro chuot den toa do (x, y) tren Console
void gotoxy(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
}

// Ham xoa mot vung chu nhat tren man hinh de chong bi de chu (chong troi)
void xoaVung(int x, int y, int width, int height) {
    for (int i = 0; i < height; i++) {
        gotoxy(x, y + i);
        for (int j = 0; j < width; j++) cout << " ";
    }
}

// Ham de quy tinh toan so lieu thong ke cho Dashboard
void demThongKeTongQuan(TREE_DocGia root, int& soSachDangMuon, int& soDocGiaQuaHan) {
    if (root == nullptr) return;
    demThongKeTongQuan(root->pLeft, soSachDangMuon, soDocGiaQuaHan);
    
    soSachDangMuon += root->soSachDangMuon;
    if (coSachQuaHan(root)) {
        soDocGiaQuaHan++;
    }
    
    demThongKeTongQuan(root->pRight, soSachDangMuon, soDocGiaQuaHan);
}

// Ham ve bang Dashboard tong quan o man hinh chinh
void hienThiTongQuan(const QuanLyDocGia& ql, const ListDauSach& ds) {
    int tongDauSach = ds.n;
    int tongDocGia = ql.soLuongDocGia;
    int soSachDangMuon = 0;
    int soDocGiaQuaHan = 0;

    demThongKeTongQuan(ql.root, soSachDangMuon, soDocGiaQuaHan);

    gotoxy(28, 4); cout << "\033[1;36m■ TONG QUAN HE THONG\033[0m";

    // Card 1
    gotoxy(30, 6); cout << "\033[1;34m+----------------------+\033[0m";
    gotoxy(30, 7); cout << "\033[1;34m|\033[0m    Tong Dau Sach     \033[1;34m|\033[0m";
    gotoxy(30, 8); cout << "\033[1;34m|\033[0m                      \033[1;34m|\033[0m";
    gotoxy(30, 9); cout << "\033[1;34m+----------------------+\033[0m";
    gotoxy(38, 8); cout << "\033[1;37m" << tongDauSach << "\033[0m";

    // Card 2
    gotoxy(55, 6); cout << "\033[1;32m+----------------------+\033[0m";
    gotoxy(55, 7); cout << "\033[1;32m|\033[0m    Tong Doc Gia      \033[1;32m|\033[0m";
    gotoxy(55, 8); cout << "\033[1;32m|\033[0m                      \033[1;32m|\033[0m";
    gotoxy(55, 9); cout << "\033[1;32m+----------------------+\033[0m";
    gotoxy(63, 8); cout << "\033[1;37m" << tongDocGia << "\033[0m";

    // Card 3
    gotoxy(30, 11); cout << "\033[1;33m+----------------------+\033[0m";
    gotoxy(30, 12); cout << "\033[1;33m|\033[0m  Sach Dang Muon      \033[1;33m|\033[0m";
    gotoxy(30, 13); cout << "\033[1;33m|\033[0m                      \033[1;33m|\033[0m";
    gotoxy(30, 14); cout << "\033[1;33m+----------------------+\033[0m";
    gotoxy(38, 13); cout << "\033[1;37m" << soSachDangMuon << "\033[0m";

    // Card 4
    gotoxy(55, 11); cout << "\033[1;31m+----------------------+\033[0m";
    gotoxy(55, 12); cout << "\033[1;31m|\033[0m  Doc Gia Qua Han     \033[1;31m|\033[0m";
    gotoxy(55, 13); cout << "\033[1;31m|\033[0m                      \033[1;31m|\033[0m";
    gotoxy(55, 14); cout << "\033[1;31m+----------------------+\033[0m";
    gotoxy(63, 13); cout << "\033[1;37m" << soDocGiaQuaHan << "\033[0m";
}

// Ham ve Khung vien chia doi man hinh (Trai/Phai)
void veKhungGiaoDien() {
    system("cls");
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
    cout << " [Phim Mui Ten]: Di chuyen     [Enter / Mui ten Phai]: Chon     [ESC]: Quay lai\n";
    cout << "\033[0m";
}

// Ham dieu khien Menu bang phim Mui ten (Khong xai vector, xai mang tinh string)
int chonMenuThongMinh(const string menu[], int size, int toaDoX, int toaDoY, int &luaChonHienTai, bool isFocus) {
    int key;
    while (true) {
        // Render menu
        for (int i = 0; i < size; i++) {
            gotoxy(toaDoX, toaDoY + i * 2);
            if (i == luaChonHienTai && isFocus) {
                cout << "\033[1;32m> " << menu[i] << " \033[0m"; // Mau xanh khi duoc chon
            } else if (i == luaChonHienTai && !isFocus) {
                cout << "\033[1;33m  " << menu[i] << " \033[0m"; // Mau vang mo khi bi mat focus
            } else {
                cout << "  " << menu[i] << "   "; // Xoa dau > cu
            }
        }
        
        // Neu chi de hien thi (Mat focus vao cot khac) thi thoat luon
        if (!isFocus) return -1;

        // Cho doc phim
        key = _getch();
        if (key == 224 || key == 0) { 
            key = _getch();
            if (key == 72) luaChonHienTai = (luaChonHienTai - 1 + size) % size; // Mui ten LEN
            if (key == 80) luaChonHienTai = (luaChonHienTai + 1) % size;        // Mui ten XUONG
            if (key == 77) return luaChonHienTai;                               // Mui ten PHAI (Chon)
            if (key == 75) return size - 1;                                     // Mui ten TRAI (Quay lai)
        } 
        else if (key == 13) return luaChonHienTai;       // Phim ENTER
        else if (key == 27) return size - 1;             // Phim ESC (Nut Quay Lai luon nam cuoi cung)
    }
}

// =========================================================================
// 2. LUỒNG CHẠY CHÍNH CUẢ CHƯƠNG TRÌNH
// =========================================================================
void runMenu()
{
    QuanLyDocGia qlDocGia;
    qlDocGia.root = nullptr;
    qlDocGia.soLuongDocGia = 0;

    ListDauSach ds;
    ds.n = 0;

    // Load du lieu ban dau
    loadDauSach("Input_file/DauSach.txt", ds);
    loadDocGia("Input_file/DocGia.txt", qlDocGia.root);

    capNhatTrangThaiSachDangMuon(qlDocGia.root, ds);
    capNhatSoLuotMuonCuaDauSach(qlDocGia.root, ds);
    khoiTaoKhoMaThe(qlDocGia.root);

    // KHOI TAO MANG MENU (Xai array tinh cho chuan mon DSA)
    const string menuTrai[] = { "Bang Dieu Khien", "Quan Ly Doc Gia", "Quan Ly Sach", "Giao Dich Muon Tra", "Bao Cao Thong Ke", "Thoat He Thong" };
    int sizeTrai = 6;
    int indexTrai = 0;

    const string menuDocGia[] = { "Them doc gia moi", "Xoa doc gia", "Hieu chinh thong tin", "Khoa / Mo the", "In danh sach doc gia", "<- Quay lai" };
    int sizeDocGia = 6;

    const string menuSach[] = { "Them dau sach", "Hieu chinh dau sach", "Xoa dau sach", "Them cuon sach", "Tra cuu sach (Ten)", "In cac dau sach", "In theo the loai", "<- Quay lai" };
    int sizeSach = 8;

    const string menuGiaoDich[] = { "Cho muon sach", "Nhan tra sach", "Bao mat sach", "Xem lich su muon", "<- Quay lai" };
    int sizeGiaoDich = 5;

    const string menuThongKe[] = { "Danh sach qua han", "Top 10 sach HOT", "Thong ke the loai", "<- Quay lai" };
    int sizeThongKe = 4;

    veKhungGiaoDien();
    bool isRunning = true;

    // Vong lap UI Chinh
    while (isRunning) {
        
        xoaVung(28, 4, 50, 16); // Lam sach cot ben Phai
        
        if (indexTrai == 0) {
            hienThiTongQuan(qlDocGia, ds);
        } else {
            gotoxy(28, 4); cout << "\033[1;30m(Dung phim mui ten Phai hoac Enter de chon chuc nang)\033[0m";
        }
        
        // Kich hoat menu Trai
        chonMenuThongMinh(menuTrai, sizeTrai, 3, 6, indexTrai, true);

        // Kiem tra chon THOAT
        if (indexTrai == 5) {
            system("cls");
            saveDocGia("Input_file/DocGia.txt", qlDocGia.root);
            saveDauSach("Input_file/DauSach.txt", ds);
            luuKhoMaThe();
            cout << "\n\033[1;32m[OK] Da sao luu toan bo he thong. Tam biet!\033[0m\n\n";
            break;
        }

        if (indexTrai == 0) continue; // Dang o Bang Dieu Khien thi loop tiep

        int indexPhai = 0;
        bool inSubMenu = true;

        // Vong lap menu Phai (Sub-menu)
        while (inSubMenu) {
            veKhungGiaoDien();
            
            // In lai menu trai nhung mo di (isFocus = false)
            chonMenuThongMinh(menuTrai, sizeTrai, 3, 6, indexTrai, false); 
            
            int chonPhai = -1;

            if (indexTrai == 1) {
                gotoxy(28, 4); cout << "\033[1;36m■ CHUC NANG: DOC GIA\033[0m";
                chonPhai = chonMenuThongMinh(menuDocGia, sizeDocGia, 28, 6, indexPhai, true);
            } 
            else if (indexTrai == 2) {
                gotoxy(28, 4); cout << "\033[1;36m■ CHUC NANG: DANH MUC SACH\033[0m";
                chonPhai = chonMenuThongMinh(menuSach, sizeSach, 28, 6, indexPhai, true);
            }
            else if (indexTrai == 3) {
                gotoxy(28, 4); cout << "\033[1;36m■ CHUC NANG: GIAO DICH\033[0m";
                chonPhai = chonMenuThongMinh(menuGiaoDich, sizeGiaoDich, 28, 6, indexPhai, true);
            }
            else if (indexTrai == 4) {
                gotoxy(28, 4); cout << "\033[1;36m■ CHUC NANG: THONG KE\033[0m";
                chonPhai = chonMenuThongMinh(menuThongKe, sizeThongKe, 28, 6, indexPhai, true);
            }

            // Kiem tra rut lui (Nut Quay Lai nam o cuoi cung cua mang hoac bam ESC/Mui ten Trai)
            if ( (indexTrai == 1 && chonPhai == sizeDocGia - 1) || 
                 (indexTrai == 2 && chonPhai == sizeSach - 1) || 
                 (indexTrai == 3 && chonPhai == sizeGiaoDich - 1) || 
                 (indexTrai == 4 && chonPhai == sizeThongKe - 1) || chonPhai == -1 ) {
                inSubMenu = false; 
                continue;
            }

            // --- THUC THI CHUC NANG ---
            system("cls"); // Xoa toan bo khung de hien thi thong tin text
            cout << "\n\033[1;33m===== HE THONG DANG XU LY =====\033[0m\n\n";

            if (indexTrai == 1) { // QUAN LY DOC GIA
                switch (chonPhai) {
                    case 0: nhapDocGia(qlDocGia.root); break;
                    case 1: {
                        int ma = nhapSoNguyenHopLe("Nhap ma the can xoa: ");
                        DocGia* dg = timDocGia(qlDocGia.root, ma);
                        if (dg == nullptr) cout << "Khong tim thay doc gia!\n";
                        else {
                            cout << "Ban co chac muon xoa " << dg->ho << " " << dg->ten << "? (Y/N): ";
                            char confirm; cin >> confirm; cin.ignore(1000,'\n');
                            if (toupper(confirm) == 'Y') xoaDocGia(qlDocGia.root, ma);
                        }
                        break;
                    }
                    case 2: hieuChinhDocGia(qlDocGia.root, nhapSoNguyenHopLe("Nhap ma the: ")); break;
                    case 3: khoaMoThe(qlDocGia.root, nhapSoNguyenHopLe("Nhap ma the: ")); break;
                    case 4: {
                        cout << "1. In theo ma the tang dan\n2. In theo ten (A-Z)\n";
                        int opt = nhapSoNguyenHopLe("Chon che do in: ");
                        if (opt == 1) inDanhSachDocGia(qlDocGia.root); 
                        else inTheoTen(qlDocGia.root);
                        break;
                    }
                }
            } 
            else if (indexTrai == 2) { // QUAN LY SACH
                switch (chonPhai) {
                    case 0: themDauSach(ds); break;
                    case 1: hieuChinhDauSach(ds); break;
                    case 2: xoaDauSachUI(ds); break;
                    case 3: {
                        char isbn[20]; cout << "Nhap ISBN cua Dau sach: "; cin.getline(isbn,20);
                        DauSach* d = timTheoISBN(ds,isbn);
                        if (d) themCuonSach(d); else cout << "Loi: Khong tim thay Dau sach voi ISBN nay!\n";
                        break;
                    }
                    case 4: {
                        char ten[100]; cout << "Nhap tu khoa ten sach: "; cin.getline(ten,100); 
                        traCuuTheoTenNangCao(ds, ten); 
                        break;
                    }
                    case 5: inDanhSachDauSach(ds); break;
                    case 6: inTheoTheLoai_TrongDoTenTangDan(ds); break;
                }
            }
            else if (indexTrai == 3) { // GIAO DICH
                switch (chonPhai) {
                    case 0: {
                        int ma = nhapSoNguyenHopLe("Ma doc gia: ");
                        DocGia* dg = timDocGia(qlDocGia.root, ma);
                        if (!dg) { cout << "Khong tim thay the thu vien!\n"; break; }
                        if (dg->trangThaiThe == 0) { cout << "The dang bi khoa, tu choi giao dich!\n"; break; }
                        if (dg->soSachDangMuon >= MAX_SACH_MUON) { cout << "Doc gia da muon toi da 3 cuon!\n"; break; }
                        if (coSachQuaHan(dg)) { cout << "Doc gia dang bi no sach qua han!\n"; break; }
                        
                        char ms[25]; cout << "Ma sach can muon: "; cin.getline(ms,25);
                        if (muonSach(qlDocGia.root, ds, ma, ms)) { 
                            saveDocGia("Input_file/DocGia.txt", qlDocGia.root); 
                            saveDauSach("Input_file/DauSach.txt", ds); 
                        }
                        break;
                    }
                    case 1: {
                        int ma = nhapSoNguyenHopLe("Ma doc gia: ");
                        DocGia* dg = timDocGia(qlDocGia.root, ma);
                        if (!dg) { cout << "Khong tim thay the thu vien!\n"; break; }
                        if (dg->soSachDangMuon == 0) { cout << "Doc gia nay khong muon sach nao!\n"; break; }
                        
                        cout << "\n[ Cac sach dang muon ]\n";
                        xemSachDangMuon(dg, ds);
                        
                        char ms[25]; cout << "Ma sach tra: "; cin.getline(ms,25);
                        if (traSach(qlDocGia.root, ds, ma, ms, layNgayHienTai())) { 
                            saveDocGia("Input_file/DocGia.txt", qlDocGia.root); 
                            saveDauSach("Input_file/DauSach.txt", ds); 
                        }
                        break;
                    }
                    case 2: {
                        int ma = nhapSoNguyenHopLe("Ma doc gia bao mat: ");
                        DocGia* dg = timDocGia(qlDocGia.root, ma);
                        if (!dg) { cout << "Khong tim thay the thu vien!\n"; break; }
                        if (dg->soSachDangMuon == 0) { cout << "Doc gia nay khong muon sach nao!\n"; break; }
                        
                        xemSachDangMuon(dg, ds);
                        char ms[25]; cout << "Ma sach bi mat: "; cin.getline(ms,25);
                        if (baoMatSach(qlDocGia.root, ds, ma, ms)) { 
                            saveDocGia("Input_file/DocGia.txt", qlDocGia.root); 
                            saveDauSach("Input_file/DauSach.txt", ds); 
                        }
                        break;
                    }
                    case 3: {
                        int ma = nhapSoNguyenHopLe("Nhap ma doc gia: ");
                        DocGia* dg = timDocGia(qlDocGia.root, ma);
                        if (dg) xemSachDangMuon(dg, ds); else cout << "Khong tim thay doc gia!\n";
                        break;
                    }
                }
            }
            else if (indexTrai == 4) { // THONG KE
                switch (chonPhai) {
                    case 0: lietKeDocGiaQuaHan(qlDocGia.root); break; // Truyen theo file Thongke.h cua ban
                    case 1: inTop10SachMuonNhieu(ds); break;
                    case 2: thongKeSoLuongTheoTheLoai(ds); break;
                }
            }
            
            // --- KET THUC CHUC NANG -> DO TIM PHIM BAM DE TRO VE ---
            cout << "\n\n\033[1;32mHoan tat! An phim bat ky de quay ve Bang dieu khien...\033[0m";
            _getch();
            veKhungGiaoDien(); // Ve lai khung Dashboard de tranh bi xoa mat
        }
    }

    giaiPhongCay(qlDocGia.root);
    giaiPhongDanhSachDauSach(ds);
}