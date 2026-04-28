#include "MuonTra.h"
#include "Docgia.h"
#include "Sach.h"
#include <ctime>
#include <iostream>
#include <cstring>

using namespace std;

// Ham layNgayHienTai de lay ngay thang nam hien tai tu he thong
Ngay layNgayHienTai() {
    time_t t = time(nullptr);
    struct tm* now = localtime(&t);
    Ngay ngayHT;
    ngayHT.ngay = now->tm_mday;
    ngayHT.thang = now->tm_mon + 1;
    ngayHT.nam = now->tm_year + 1900;
    return ngayHT; 
} 

// Ham nay chuyen ve dang so giay cho de tinh
time_t chuyenNgaySangTimeT(const Ngay& ngay) {
    struct tm tmNgay;
    tmNgay.tm_sec = 0;
    tmNgay.tm_min = 0;
    tmNgay.tm_hour = 0;
    tmNgay.tm_mday = ngay.ngay;
    tmNgay.tm_mon = ngay.thang - 1;
    tmNgay.tm_year = ngay.nam - 1900;
    tmNgay.tm_wday = 0; 
    tmNgay.tm_yday = 0; 
    tmNgay.tm_isdst = -1; 
    return mktime(&tmNgay); 
}

// difftime cho an toan hon khi tinh toan so ngay giua 2 ngay
int tinhSoNgay(const Ngay& ngayMuon, const Ngay& ngayHienTai) {
    time_t timeMuon = chuyenNgaySangTimeT(ngayMuon);
    time_t timeHienTai = chuyenNgaySangTimeT(ngayHienTai);
    double seconds = difftime(timeHienTai, timeMuon);  
    
    return (int)(seconds / (60 * 60 * 24));
}

// Ham muonSach 
bool muonSach(DocGia* docGia, ListDauSach &ds, const char* maSach) {
    Ngay ngayDungLucMuon = layNgayHienTai();

    if (docGia == nullptr) return false;

    DauSach* dauSach = nullptr;
    Sach* sachMuon = timSachTheoMa(ds, maSach, dauSach);

    if (sachMuon == nullptr) {
        cout << "Khong tim thay cuon sach co ma: " << maSach << "\n";
        return false;
    }

    if (sachMuon->trangThai == 2) {
        cout << "Cuon sach nay da bi thanh ly, khong the muon!\n";
        return false;
    } else if (sachMuon->trangThai == 1) {
        cout << "Cuon sach nay dang duoc nguoi khac muon!\n";
        return false;
    }

    // Kiem tra muon cung dau sach
    bool daMuonSachNay = false;
    char isbnCanMuon[20];
    tachMaISBN(maSach, isbnCanMuon);

    MuonTra* mt = docGia->dsMuonTra.pHead;
    while (mt != nullptr) {
        if (mt->trangThai == 0) { 
            char isbnDangMuon[20];
            tachMaISBN(mt->maSach, isbnDangMuon);
            if (strcmp(isbnDangMuon, isbnCanMuon) == 0) daMuonSachNay = true;
        }
        mt = mt->pNext;
    }

    if (daMuonSachNay) {
        cout << "Doc gia dang muon 1 cuon sach thuoc cung DAU SACH nay roi! Khong the muon tiep.\n";
        return false;
    }   

    MuonTra* muonTra = new MuonTra; 
    strncpy(muonTra->maSach, maSach, sizeof(muonTra->maSach)-1);
    muonTra->maSach[sizeof(muonTra->maSach)-1] = '\0'; 
    muonTra->ngayMuon = ngayDungLucMuon;
    muonTra->trangThai = 0; 
    muonTra->pNext = nullptr; 

    addTailMuonTra(docGia->dsMuonTra, muonTra);
    sachMuon->trangThai = 1;
    docGia->soSachDangMuon++; // Cap nhat O(1)

    cout << "Muon sach thanh cong!\n";
    return true;
}

bool traSach(DocGia* docGia, ListDauSach &ds, const char* maSach, Ngay ngayTra) {
    if (docGia == nullptr) return false;

    MuonTra* muonTraCanTra = nullptr;
    bool vanConLoi = false;
    MuonTra* tempMT = docGia->dsMuonTra.pHead;

    while (tempMT != nullptr) {
        if (strcmp(tempMT->maSach, maSach) == 0 && tempMT->trangThai == 0) { 
            muonTraCanTra = tempMT; // Tim thay sach can tra, luu lai
        } else {
    
            if (tempMT->trangThai == 2 || (tempMT->trangThai == 0 && tinhSoNgay(tempMT->ngayMuon, ngayTra) > HAN_MUON)) {
                vanConLoi = true;
            }
        }
        tempMT = tempMT->pNext;
    }

    if (muonTraCanTra == nullptr) {
        cout << "Khong tim thay thong tin muon sach cho ma nay!\n";
        return false;
    }

    DauSach* dauSach = nullptr;
    Sach* sachTra = timSachTheoMa(ds, maSach, dauSach); // 
    if (sachTra == nullptr) { cout << "Loi du lieu: Khong tim thay cuon sach trong danh muc!\n"; return false; }

    muonTraCanTra->trangThai = 1;
    muonTraCanTra->ngayTra = ngayTra;
    sachTra->trangThai = 0;
    docGia->soSachDangMuon--; // Cap nhat O(1)

    // Tu dong mo khoa the
    if (docGia->trangThaiThe == 0 && !vanConLoi) docGia->trangThaiThe = 1;

    cout << "Tra sach thanh cong!\n";
    return true;
}

bool baoMatSach(DocGia* docGia, ListDauSach &ds, const char* maSach) {
    if (docGia == nullptr) return false;

    MuonTra* muonTra = docGia->dsMuonTra.pHead;
    while (muonTra != nullptr) {
        if (strcmp(muonTra->maSach, maSach) == 0 && muonTra->trangThai == 0) {
            DauSach* dauSach = nullptr;
            Sach* sach = timSachTheoMa(ds, maSach, dauSach);
            if (sach) sach->trangThai = 2; 
            
            muonTra->trangThai = 2;
            docGia->trangThaiThe = 0; // Khoa the
            docGia->soSachDangMuon--; // Sach mat thi khong tinh la dang muon nua
            
            cout << "Da ghi nhan sach bi mat! The da bi khoa.\n";
            return true;
        }
        muonTra = muonTra->pNext;
    }
    cout << "Khong tim thay thong tin muon sach cho ma nay!\n";
    return false;
}

bool coSachQuaHan(const DocGia* docGia) {
    if (docGia == nullptr) return false;

    Ngay ngayHienTai = layNgayHienTai();
    MuonTra* temp = docGia->dsMuonTra.pHead;

    while (temp != nullptr) {
        int soNgay = tinhSoNgay(temp->ngayMuon, ngayHienTai);

        if (temp->trangThai == 0 && soNgay > HAN_MUON) {
            return true;
        }
        temp = temp->pNext;
    }
    return false;
}

void xemSachDangMuon(const DocGia* docGia, const ListDauSach& ds) {
    MuonTra* temp = docGia->dsMuonTra.pHead;
    bool coSach = false;

    while (temp != nullptr) {
        if (temp->trangThai == 0) { // Dang muon
            coSach = true;
            DauSach* dauSach = nullptr;
            Sach* sach = timSachTheoMa(ds, temp->maSach, dauSach);
            
            cout << "- Ma sach: " << temp->maSach;
            if (dauSach != nullptr) {
                cout << " | Ten sach: " << dauSach->tenSach;
            } else {
                cout << " | Ten sach: (Khong ro - da bi xoa khoi he thong?)";
            }
            cout << " | Ngay muon: " << temp->ngayMuon.ngay << "/" << temp->ngayMuon.thang << "/" << temp->ngayMuon.nam << endl;
        }
        temp = temp->pNext;
    }
    
    if (!coSach) {
         cout << "  (Hien tai khong muon cuon sach nao)\n";
    }
}
