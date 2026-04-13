#include <cstring>
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
// Sắp xếp danh sách đầu sách tăng dần theo tên sách (bubble sort)
void sapXepDauSachTangDan(ListDauSach &ds) {
    for (int i = 0; i < ds.n - 1; ++i) {
        for (int j = i + 1; j < ds.n; ++j) {
            if (_stricmp(ds.nodes[i]->tenSach, ds.nodes[j]->tenSach) > 0)                DauSach* tmp = ds.nodes[i];
                ds.nodes[i] = ds.nodes[j];
                ds.nodes[j] = tmp;
            }
        }
    }
    cout << "Da sap xep danh sach dau sach tang dan theo ten!\n";
}
int menu() {
    cout << "\n===== MENU =====\n";
    cout << "1. Quan ly doc gia\n";
    cout << "2. Quan ly sach\n";
    cout << "3. Giao dich\n";
    cout << "4. Thong ke\n";
    cout << "0. Thoat\n";
    cout << "Chon: ";
    
    int choice;
    if (!(cin >> choice)) { // Kiểm tra nếu người dùng nhập chữ thay vì số
        cin.clear(); // Xóa lỗi nhập 
        cin.ignore(1000, '\n'); // Xóa bỏ ký tự không hợp lệ còn lại trong bộ đệm
        cout << "Lua chon khong hop le! Vui long nhap so tu 0 den 4.\n";    
        return -1;
    }
    cin.ignore(1000, '\n'); // Xóa bỏ ký tự newline còn lại trong bộ đệm sau khi đọc số
    return choice;
}

void runMenu()
{
    TREE_DocGia root = nullptr;
    ListDauSach ds;
    ds.n = 0;

    loadDauSach("Input_file/DauSach.txt", ds);
    sapXepDauSachTangDan(ds); // Đảm bảo danh sách tăng dần theo tên sách sau khi load
    loadDocGia("Input_file/DocGia.txt", root);
    capNhatTrangThaiSachDangMuon(root, ds);
    capNhatSoLuotMuonCuaDauSach(root, ds);
    khoiTaoKhoMaThe(root); // Khoi tao Stack id sau khi da load data
    sapXepDauSachTangDan(ds); // Sap xep lai sau khi cap nhat so luot muon de hien thi top 10 chinh xac

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
            cout << "Chon: ";
            cin >> c; cin.ignore(1000,'\n');

            switch (c) {
        case 1: nhapDocGia(root); break;

        case 2: {
            int ma;
            cout << "Nhap ma: ";
            cin >> ma; cin.ignore(1000,'\n');
            char confirm;
            cout << "Ban co chac chan muon xoa doc gia co ma " << ma << "? (y/n): ";
            cin >> confirm; cin.ignore(1000, '\n');
            if (confirm == 'y' || confirm == 'Y') {
                bool xoaOK = xoaDocGia(root, ma);
                if (xoaOK) cout << "Xoa doc gia thanh cong!\n";
                else cout << "Khong the xoa doc gia nay!\n";
            } else {
                cout << "Da huy xoa doc gia.\n";
            }
            break;
        }

            case 3: {
                int ma;
                cout << "Nhap ma: ";
                cin >> ma; cin.ignore(1000,'\n');
                hieuChinhDocGia(root, ma);
                break;
            }

            case 4: {
                int ma;
                cout << "Nhap ma: ";
                cin >> ma; cin.ignore(1000, '\n');
                khoaMoThe(root, ma);
                break;
            }

            case 5: {
                int opt;
                cout << "1. Theo ma the\n2. Theo ten\nChon: ";
                cin >> opt; cin.ignore();

                if (opt == 1) inDanhSachDocGia(root);
                else inTheoTen(root);
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
            cout << "Chon: ";
            cin >> c; cin.ignore(1000,'\n');

            switch (c) {
            case 1: themDauSach(ds); break;

            case 2: hieuChinhDauSach(ds); break;

            case 3: {
                char isbn[20];
                cout << "Nhap ISBN can xoa: ";
                cin.ignore();
                cin.getline(isbn, 20);
                char confirm;
                cout << "Ban co chac chan muon xoa dau sach co ISBN " << isbn << "? (y/n): ";
                cin >> confirm; cin.ignore(1000, '\n');
                if (confirm == 'y' || confirm == 'Y') {
                    bool xoaOK = xoaDauSach(ds, isbn);
                    if (xoaOK) cout << "Xoa dau sach thanh cong!\n";
                    else cout << "Khong the xoa dau sach nay!\n";
                } else {
                    cout << "Da huy xoa dau sach.\n";
                }
                break;
            }

            case 4: {
                char isbn[20];
                cout << "Nhap ISBN: ";
                cin.getline(isbn,20);

                DauSach* d = timTheoISBN(ds,isbn);
                if (d) themCuonSach(d);
                else cout << "Khong tim thay!\n";
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
            cout << "Chon: ";
            cin >> c; cin.ignore(1000,'\n');

            switch (c) {

            case 1: {
                int ma;
                char ms[25];

                cout << "Ma doc gia: ";
                cin >> ma; cin.ignore(1000,'\n');

                cout << "Ma sach: ";
                cin.getline(ms,25);

                if (muonSach(root, ds, ma, ms)) {
                    saveDocGia("Input_file/DocGia.txt", root);
                    saveDauSach("Input_file/DauSach.txt", ds);
                } else {
                    cout << "Giao dich that bai, khong luu file!\n";
                }
                break;
            }

            case 2: {
                int ma;
                char ms[25];

                cout << "Ma doc gia: ";
                cin >> ma; cin.ignore(1000,'\n');

                cout << "Ma sach: ";
                cin.getline(ms,25);

                if (traSach(root, ds, ma, ms, layNgayHienTai())) {
                    saveDocGia("Input_file/DocGia.txt", root);
                    saveDauSach("Input_file/DauSach.txt", ds);
                } else {
                    cout << "Giao dich that bai, khong luu file!\n";
                }

                break;
            }

            case 3: {
                int ma;
                char ms[25];

                cout << "Ma doc gia: ";
                cin >> ma; cin.ignore(1000,'\n');

                cout << "Ma sach: ";
                cin.getline(ms,25);

                if (baoMatSach(root, ds, ma, ms)) {
                    saveDocGia("Input_file/DocGia.txt", root);
                    saveDauSach("Input_file/DauSach.txt", ds);
                } else {
                    cout << "Giao dich that bai, khong luu file!\n";
                }
                
                break;
            }

            case 4: {
                int ma;
                cout << "Nhap ma doc gia: ";
                cin >> ma; cin.ignore(1000, '\n');

                DocGia* dg = timDocGia(root, ma);
                if (dg) xemSachDangMuon(dg);
                else cout << "Khong tim thay!\n";
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
            cout << "Chon: ";
            cin >> c; cin.ignore(1000,'\n');

            switch (c) {
            case 1:
                lietKeDocGiaQuaHan(root);
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
            saveDocGia("Input_file/DocGia.txt", root);
            saveDauSach("Input_file/DauSach.txt", ds);
            cout << "Da luu du lieu!\n";
            cout << "Tam biet!\n";
            break;

        default:
            cout << "Sai lua chon!\n";
        }

    } while (choice != 0);

    giaiPhongCay(root);
    giaiPhongDanhSachDauSach(ds);
}
