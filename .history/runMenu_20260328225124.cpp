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

using namespace std;

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

    // ===== LOAD FILE =====
    loadDauSach("File/DauSach.txt", ds);
    cout << "Load DauSach thanh cong\n";

    loadDocGia("File/DocGia.txt", root);
    cout << "Load DocGia thanh cong\n";

    capNhatTrangThaiSachDangMuon(root, ds);
    // =====================

    int choice;
    do {
        choice = menu();

        switch (choice) {

        case 1:
        {
            int subChoice;
            cout << "\n--- QUAN LY DOC GIA ---\n";
            cout << "11. Them doc gia\n";
            cout << "12. Xoa doc gia\n";
            cout << "13. Hieu chinh thong tin\n";
            cout << "14. Khoa / Mo the\n";
            cout << "15. In danh sach doc gia\n";
            cout << "Chon: ";

            cin >> subChoice;
            cin.ignore(1000, '\n');

            if (subChoice == 11) {
                nhapDocGia(root);
            }
            else if (subChoice == 12) {
                int maThe;
                cout << "Nhap ma the can xoa: ";
                cin >> maThe;
                cin.ignore();

                xoaDocGia(root, maThe);
            }
            else if (subChoice == 13) {
                int maThe;
                cout << "Nhap ma the can sua: ";
                cin >> maThe;
                cin.ignore();

                hieuChinhDocGia(root, maThe);
            }
            else if (subChoice == 14) {
                int maThe;
                cout << "Nhap ma the: ";
                cin >> maThe;
                cin.ignore();

                khoaMoThe(root, maThe);
            }
            else if (subChoice == 15) {
                cout << "1. Theo ma the\n";
                cout << "2. Theo ten\n";
                int opt;
                cin >> opt;
                cin.ignore();

                if (opt == 1)
                    inDanhSachDocGia(root); // tu dong tang do duyet LNR
                else
                    inTheoTen(root);
            }
            
            break;
        }

        case 2:
        {   
            int subChoice;
            cout << "\n--- QUAN LY SACH ---\n";
            cout << "21. Them dau sach\n";
            cout << "22. Them cuon sach (nhap ISBN)\n";
            cout << "23. Tra cuu sach theo ten\n";
            cout << "24. In danh sach dau sach va cuon sach\n";
            cout << "Chon: ";
            cin >> subChoice;
            cin.ignore(1000, '\n'); 

            if (subChoice == 21) {
                themDauSach(ds); 
            } 
            else if (subChoice == 22) {
                char isbn[20]; // Tăng kích thước cho an toàn
                cout << "Nhap ISBN cua dau sach muon them cuon: ";
                cin.getline(isbn, 20);

                DauSach* dsFound = timTheoISBN(ds, isbn); 

                if (dsFound != nullptr) {
                    themCuonSach(dsFound); 
                } else {
                    cout << "Khong tim thay dau sach co ISBN nay!\n";
                }
            }
            else if (subChoice == 23) {
                char ten[100];
                cout << "Nhap ten sach can tim: ";
                cin.getline(ten, 100);

                traCuuTheoTenNangCao(ds, ten); 
            }
            else if (subChoice == 24) {
                if (ds.n == 0) cout << "Danh sach trong!\n";
                for(int i = 0; i < ds.n; i++) {
                    cout << "\n[" << i+1 << "] ISBN: " << ds.nodes[i]->ISBN 
                         << " - Ten: " << ds.nodes[i]->tenSach;
                    inDanhSachCuonSach(ds.nodes[i]); 
                }
            }
            break;
        }

        case 3:
        {
            int subChoice;
            cout << "\n--- GIAO DICH ---\n";
            cout << "31. Muon sach\n";
            cout << "32. Tra sach\n";
            cout << "33. Bao mat sach\n";
            cout << "Chon: ";

            cin >> subChoice;
            cin.ignore(1000, '\n');

            if (subChoice == 31)
            {
                int maThe;
                char maSach[25];
                Ngay ngay;

                cout << "Nhap ma the doc gia: ";
                cin >> maThe;
                cin.ignore(1000, '\n');

                cout << "Nhap ma sach: ";
                cin.getline(maSach, 25);

                cout << "Nhap ngay muon (d m y): ";
                Ngay ngay = layNgayHienTai();
                cin.ignore();

                muonSach(root, ds, maThe, maSach, ngay);
            }

            else if (subChoice == 32)
            {
                int maThe;
                char maSach[25];
                Ngay ngay;

                cout << "Nhap ma the doc gia: ";
                cin >> maThe;
                cin.ignore(1000, '\n');

                cout << "Nhap ma sach: ";
                cin.getline(maSach, 25);

                cout << "Nhap ngay tra (d m y): ";
                cin >> ngay.ngay >> ngay.thang >> ngay.nam;
                cin.ignore();

                traSach(root, ds, maThe, maSach, ngay);
            }

            else if (subChoice == 33)
            {
                int maThe;
                char maSach[25];

                cout << "Nhap ma the doc gia: ";
                cin >> maThe;
                cin.ignore(1000, '\n');

                cout << "Nhap ma sach: ";
                cin.getline(maSach, 25);
                cin.ignore();

                baoMatSach(root, ds, maThe, maSach);
            }

            break;
        }

        case 4:
            cout << "Chuc nang thong ke\n";
            break;
        case 0:
            cout << "Thoat chuong trinh\n";
            break;
        default:
            cout << "Lua chon khong hop le! Vui long chon lai.\n";
        }
    } while (choice != 0);
        giaiPhongCay(root);
        giaiPhongDanhSachDauSach(ds);

    // Đừng quên giải phóng bộ nhớ (Cleanup) trước khi thoát
    // giaiPhongDanh Sach(ds);
}
