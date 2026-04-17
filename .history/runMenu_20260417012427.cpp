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

int menu() {
    cout << "\n===== MENU =====\n";
    cout << "1. Quan ly doc gia\n";
    cout << "2. Quan ly sach\n";
    cout << "3. Giao dich\n";
    cout << "4. Thong ke\n";
    cout << "0. Thoat\n";
    return nhapSoNguyenHopLe("Chon: ");
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

    int choice;
    do {
        choice = menu();

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
