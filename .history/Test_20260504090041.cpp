#include <iostream>
#include <conio.h>
#include <cctype>
#include <vector>
#include <cstdint>
#include <cstring>
#include <ctime>
#include "InputUtils.h"
#include "Docgia.h"
#include "Sach.h"
#include "MuonTra.h"
using namespace std;

void testNhapLieu()
{
    const int GIOI_HAN = 11;
    char ma[GIOI_HAN];
    nhapMa("Test nhap ma: ", ma, GIOI_HAN);
    cout << "=> Ma vua nhap: [" << ma << "]" << endl;

    const int GIOI_HAN_TEN = 50;
    char ten[GIOI_HAN_TEN];
    nhapTen("Test nhap ten: ", ten, GIOI_HAN_TEN);
    cout << "=> Ten vua nhap: [" << ten << "]" << endl;
}

void taoDuLieuGia(QuanLyDocGia& ql, ListDauSach& ds, int soLuong) {
    if (ds.n == 0) {
        cout << "Khong the tao du lieu gia vi kho sach rong.\n";
        return;
    }

    const char* hoArr[] = {"NGUYEN", "TRAN", "LE", "PHAM", "HOANG", "HUYNH", "PHAN", "VO", "DANG", "BUI", "DO", "HO", "NGO", "DUONG", "LY"};
    const char* tenNamArr[] = {"AN", "BACH", "BAO", "BINH", "CUONG", "DAT", "DUY", "GIANG", "HAI", "HIEU", "HUNG", "HUY", "KHANH", "KHOA", "KIEN", "LAM", "LONG", "MINH", "NAM", "NGHIA", "PHONG", "PHUC", "QUAN", "QUANG", "QUOC", "SON", "TAN", "THANG", "THANH", "TRUNG", "TUAN", "VIET"};
    const char* tenNuArr[] = {"ANH", "BICH", "CHAU", "CHI", "DIEM", "DUNG", "GIANG", "HA", "HANH", "HOA", "HONG", "HUONG", "KHANH", "LAN", "LINH", "LY", "MAI", "MY", "NGA", "NGOC", "NHI", "NHU", "OANH", "PHUONG", "QUYNH", "THAO", "THU", "THUY", "TRANG", "TRAM", "TUYET", "VY"};

    int soHo = sizeof(hoArr) / sizeof(hoArr[0]);
    int soTenNam = sizeof(tenNamArr) / sizeof(tenNamArr[0]);
    int soTenNu = sizeof(tenNuArr) / sizeof(tenNuArr[0]);

    uint32_t seed = (uint32_t)time(nullptr);

    // 1. Thu thap tat ca sach co the muon
    vector<Sach*> sachSanCo;
    for (int i = 0; i < ds.n; i++) {
        Sach* s = ds.nodes[i]->dsSach.pHead;
        while (s != nullptr) {
            if (s->trangThai == 0) {
                sachSanCo.push_back(s);
            }
            s = s->pNext;
        }
    }

    if (sachSanCo.empty()) {
        cout << "Khong con sach nao trong kho de cho muon.\n";
        return;
    }

    int count = 0;
    for (int i = 0; i < soLuong; i++) {
        int maThe = taoMaTheMoi();
        int maThe = taoMaTheMoi(ql);
        if (maThe == -1) {
            cout << "Kho ma the da can kiet!\n";
            break;
        }

        DocGia* dg = taoDocGia(maThe);

        // Random gioi tinh va ten
        bool laNam = (rand() % 2 == 0);
        strcpy(dg->giotinh, laNam ? "NAM" : "NU");
        strcpy(dg->ho, hoArr[rand() % soHo]);
        if (laNam) {
            strcpy(dg->ten, tenNamArr[rand() % soTenNam]);
        } else {
            strcpy(dg->ten, tenNuArr[rand() % soTenNu]);
        }

        // Mac dinh the hoat dong, chi khoa khi co vi pham (qua han hoac mat sach)
        dg->trangThaiThe = 1;

        themDocGia(ql, dg);

        // Random so sach muon (0-3)
        int soSachMuon = rand() % 4;

        // Sinh mang offset ngay lui lai va sap xep giam dan
        // de dam bao ngay muon cac sach khong bi lon xon
        int offsets[3] = {0, 0, 0};
        for (int j = 0; j < soSachMuon; j++) {
            bool isQuaHan = (rand() % 4 == 0); // 25% ty le qua han
            offsets[j] = isQuaHan ? (rand() % 20 + 8) : (rand() % 8);
        }
        for (int j = 0; j < soSachMuon - 1 && soSachMuon > 1; j++) {
            for (int k = j + 1; k < soSachMuon; k++) {
                if (offsets[j] < offsets[k]) {
                    int temp = offsets[j];
                    offsets[j] = offsets[k];
                    offsets[k] = temp;
                }
            }
        }

        bool coMatSach = false;

        for (int j = 0; j < soSachMuon; j++) {
            if (sachSanCo.empty()) break;

            // Chon mot cuon sach ngau nhien tu danh sach san co
            int sachIdx = rand() % sachSanCo.size();
            Sach* sachChon = sachSanCo[sachIdx];

            // Kiem tra xem doc gia da muon dau sach nay chua
            char isbnCanMuon[20];
            tachMaISBN(sachChon->maSach, isbnCanMuon);
            bool daMuonDauSachNay = false;
            MuonTra* mtCheck = dg->dsMuonTra.pHead;
            while(mtCheck != nullptr) {
                char isbnDangMuon[20];
                tachMaISBN(mtCheck->maSach, isbnDangMuon);
                if(strcmp(isbnDangMuon, isbnCanMuon) == 0) {
                    daMuonDauSachNay = true;
                    break;
                }
                mtCheck = mtCheck->pNext;
            }

            if (daMuonDauSachNay) continue; // Bo qua neu da muon dau sach nay

            // Tinh toan thoi gian mượn thẳng hàng
            time_t now = time(nullptr);
            now -= (time_t)offsets[j] * 24 * 60 * 60;
            struct tm* tmMuon = localtime(&now);
            Ngay ngayMuon = {tmMuon->tm_mday, tmMuon->tm_mon + 1, tmMuon->tm_year + 1900};
            
            muonSach(dg, ds, sachChon->maSach); // Muon sach that
            dg->dsMuonTra.pTail->ngayMuon = ngayMuon; // Ghi de lai ngay muon gia

            // Gia lap 10% doc gia lam mat sach
            if (rand() % 10 == 0) {
                baoMatSach(dg, ds, sachChon->maSach);
                dg->dsMuonTra.pTail->ngayMuon = ngayMuon; // Khoi phuc lai ngay muon gia lap 
                coMatSach = true;
            }

            // Loai sach da muon khoi danh sach san co
            sachSanCo.erase(sachSanCo.begin() + sachIdx);
        }

        // In ly do khoa the ra man hinh de test de dang hon
        if (coMatSach) {
            dg->trangThaiThe = 0;
            setColor(31); cout << "   -> [KHOA THE] " << dg->maThe << " - " << dg->ho << " " << dg->ten << " (Ly do: Lam MAT SACH)\n"; resetColor();
        }
        
        count++;
    }
    cout << "\nDa tao thanh cong " << count << " doc gia gia.\n";
}