#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
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
#include <con

using namespace std;

// =========================================================================
// HÀM VẼ GIAO DIỆN MENU BẰNG PHÍM MŨI TÊN (KHÔNG CẦN GÕ SỐ)
// =========================================================================
int interactiveMenu(const string& title, const string options[], const int returnValues[], int numOptions) {
    int selected = 0; // Vị trí đang đứng

    while (true) {
        system("cls"); // Xóa màn hình để vẽ lại menu mới

        // Vẽ Header
        cout << "\n";
        cout << "  =======================================================\n";
        cout << "                " << title << "\n";
        cout << "  =======================================================\n\n";

        // Vẽ các lựa chọn
        for (int i = 0; i < numOptions; i++) {
            if (i == selected) {
                // Đang trỏ chuột vào dòng này
                cout << "         >> [ * ] " << options[i] << " <<\n"; 
            } else {
                // Dòng bình thường
                cout << "            [   ] " << options[i] << "\n";
            }
        }

        // Vẽ Footer hướng dẫn
        cout << "\n  =======================================================\n";
        cout << "   (Dung phim MUI TEN LEN/XUONG de chon, ENTER de xac nhan)\n\n";

        // Bắt phím từ người dùng (Sử dụng hàm getKey() trong InputUtils.h)
        int key = getKey();

        if (key == KEY_UP) {
            selected = (selected - 1 + numOptions) % numOptions; // Cuộn lên, tự vòng lại cuối
        } 
        else if (key == KEY_DOWN) {
            selected = (selected + 1) % numOptions; // Cuộn xuống, tự vòng lên đầu
        } 
        else if (key == KEY_ENTER) {
            return returnValues[selected]; // Trả về mã chức năng tương ứng
        }
    }
}

// =========================================================================
// LUỒNG CHẠY CHÍNH TÍCH HỢP UI
// =========================================================================
void runMenu()
{
    QuanLyDocGia qlDocGia;
    ListDauSach ds;
    ds.n = 0;

    // Load du lieu ban dau
    loadDauSach("Input_file/DauSach.txt", ds);
    loadDocGia("Input_file/DocGia.txt", qlDocGia);

    capNhatTrangThaiSachDangMuon(qlDocGia.root, ds);
    capNhatSoLuotMuonCuaDauSach(qlDocGia.root, ds);

    cout << "Da load du lieu thanh cong!\n";

    khoiTaoKhoMaThe(qlDocGia.root);

    // Cấu hình mảng Menu Chính
    const string mainOptions[] = {
        "1. Quan ly doc gia",
        "2. Quan ly sach",
        "3. Giao dich (Muon/Tra)",
        "4. Thong ke",
        "0. Luu & Thoat he thong"
    };
    const int mainValues[] = {1, 2, 3, 4, 0};

    bool isRunning = true;
    while (isRunning) {
        int choice = interactiveMenu("HE THONG QUAN LY THU VIEN", mainOptions, mainValues, 5);
        system("cls");

        switch (choice) {

        // ================= DOC GIA =================
        case 1:
        {
            const string dgOptions[] = {
                "1. Them doc gia", "2. Xoa doc gia", "3. Hieu chinh", 
                "4. Khoa / Mo the", "5. In danh sach", "0. Quay lai"
            };
            const int dgValues[] = {1, 2, 3, 4, 5, 0};
            
            int c = interactiveMenu("QUAN LY DOC GIA", dgOptions, dgValues, 6);
            system("cls");
            if (c == 0) break; // Quay lai menu chinh

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
                const string inOptions[] = {"1. In theo ma the", "2. In theo ten (A-Z)"};
                const int inValues[] = {1, 2};
                int opt = interactiveMenu("IN DANH SACH DOC GIA", inOptions, inValues, 2);
                system("cls");

                if (opt == 1) inDanhSachDocGia(qlDocGia.root);
                else inTheoTen(qlDocGia);
                break;
            }
            }
            
            cout << "\nNhan ENTER hoac phim bat ky de tiep tuc...";
            getch_raw();
            break;
        }

        // ================= SACH =================
        case 2:
        {
            const string sachOptions[] = {
                "1. Them dau sach", "2. Hieu chinh dau sach", "3. Xoa dau sach", 
                "4. Them cuon sach", "5. Tra cuu theo ten", "6. In danh sach", 
                "7. In theo the loai", "0. Quay lai"
            };
            const int sachValues[] = {1, 2, 3, 4, 5, 6, 7, 0};
            
            int c = interactiveMenu("QUAN LY SACH", sachOptions, sachValues, 8);
            system("cls");
            if (c == 0) break;

            switch (c) {
            case 1: themDauSach(ds); break;

            case 2: hieuChinhDauSach(ds); break;

            case 3: xoaDauSachUI(ds); break;

            case 4: {
                char isbn[20];
                cout << "Nhap ISBN: ";
                cin.getline(isbn,20);
                //chuanHoaISBN(isbn); // Ham nay chua ro o dau nen neu loi ni mo ra hoac sua lai nha

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

            cout << "\nNhan ENTER hoac phim bat ky de tiep tuc...";
            getch_raw();
            break;
        }

        // ================= GIAO DICH =================
        case 3:
        {
            const string gdOptions[] = {
                "1. Muon sach", "2. Tra sach", "3. Bao mat sach", 
                "4. Xem sach dang muon", "0. Quay lai"
            };
            const int gdValues[] = {1, 2, 3, 4, 0};
            
            int c = interactiveMenu("GIAO DICH MUON / TRA", gdOptions, gdValues, 5);
            system("cls");
            if (c == 0) break;

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

                if (muonSach(dg, ds, ms)) { // Neu muonSach can root, truyen qlDocGia.root vao
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

                if (traSach(dg, ds, ms, layNgayHienTai())) { // Truyen qlDocGia.root neu can
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

                if (baoMatSach(dg, ds, ms)) { // Truyen qlDocGia.root neu can
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
            
            cout << "\nNhan ENTER hoac phim bat ky de tiep tuc...";
            getch_raw();
            break;
        }

        // ================= THONG KE =================
        case 4: {
            const string tkOptions[] = {
                "1. Doc gia qua han", "2. Top 10 sach muon nhieu", 
                "3. Thong ke theo the loai", "0. Quay lai"
            };
            const int tkValues[] = {1, 2, 3, 0};
            
            int c = interactiveMenu("BAO CAO THONG KE", tkOptions, tkValues, 4);
            system("cls");
            if (c == 0) break;

            switch (c) {
            case 1:
                lietKeDocGiaQuaHan(qlDocGia.root); // Truyen root theo dung ham trong file cua ban
                break;

            case 2:
                inTop10SachMuonNhieu(ds);
                break;

            case 3:
                thongKeSoLuongTheoTheLoai(ds);
                break;
            }
            
            cout << "\nNhan ENTER hoac phim bat ky de tiep tuc...";
            getch_raw();
            break;
        }
        
        case 0:
            cout << "Dang sao luu du lieu...\n";
            saveDocGia("Input_file/DocGia.txt", qlDocGia.root);
            saveDauSach("Input_file/DauSach.txt", ds);
            luuKhoMaThe();
            cout << "Da luu du lieu!\nTam biet!\n";
            isRunning = false;
            break;

        }

    }

    giaiPhongCay(qlDocGia.root);
    giaiPhongDanhSachDauSach(ds);
}