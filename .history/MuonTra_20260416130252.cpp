#include "MuonTra.h"
#include "Docgia.h"
#include "Sach.h"
#include <ctime>

using namespace std;

// Ham layNgayHienTai de lay ngay thang nam hien tai tu he thong
// struct tm* now = localtime(&t); la ham localtime de chuyen doi thoi gian hien tai tu dang time_t (so giay tinh tu 1/1/1970) sang dang struct tm, trong do struct tm la mot cau truc du lieu chua thong tin ve ngay thang nam, gio phut giay, va cac thong tin lien quan den thoi gian khac. Ham localtime se tra ve con tro den struct tm chua thong tin ve ngay thang nam hien tai duoc lay tu he thong, va chung ta co the truy cap vao cac truong ngay, thang, nam trong struct tm do de gan gia tri cho bien ngayHT trong ham layNgayHienTai.
Ngay layNgayHienTai() {
    time_t t = time(nullptr);
    struct tm* now = localtime(&t);
    Ngay ngayHT;
    ngayHT.ngay = now->tm_mday;
    ngayHT.thang = now->tm_mon + 1;
    ngayHT.nam = now->tm_year + 1900;
    return ngayHT; 
} 

// Ham nay chuyen ve dang so giay cho de tinh, nhu sau:
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


// difftime cho an toan hon khi tinh toan so ngay giua 2 ngay; va no so sanh ngay luon roi, ham soSanhNgay chua can dung den.
// phu hop voi he thong tinh ngay thang nam hien tai cua may tinh
// de tranh cac van de lien quan den tinh toan so ngay khi co su thay doi ve gio he thong (chuyen sang/ra khoi gio mua he, thay doi thoi gian he thong, v.v.) co the anh huong den ket qua tinh toan so ngay neu chi tinh toan theo cach tinh toan so ngay don gian bang cach nhan so ngay voi 24*60*60 giay/ngay ma khong su dung difftime.
int tinhSoNgay(const Ngay& ngayMuon, const Ngay& ngayHienTai) {
    time_t timeMuon = chuyenNgaySangTimeT(ngayMuon);
    time_t timeHienTai = chuyenNgaySangTimeT(ngayHienTai);
    double seconds = difftime(timeHienTai, timeMuon);  
    
    return (int)(seconds / (60 * 60 * 24));
}

// Ham muonSach 
bool muonSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach) {
    Ngay ngayDungLucMuon = layNgayHienTai();

    DocGia* docGia = timDocGia(root, maThe);
    if (docGia == nullptr) {
        cout << "Khong tim thay doc gia co ma the: " << maThe << "\n";
        return false;
    }

    if (docGia->trangThaiThe == 0) {
        cout << "The doc gia dang bi khoa! Khong the muon sach.\n";
        return false;
    }

    // o(1) khoi dem  
    if (docGia->soSachDangMuon >= MAX_SACH_MUON) {
        cout << "Doc gia da muon toi da 3 cuon! Khong the muon them.\n";
        return false;
    }

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

    // qua han va cung dau sach
    bool daMuonSachNay = false;
    bool biQuaHan = false;
    char isbnCanMuon[20];
    tachMaISBN(maSach, isbnCanMuon);

    MuonTra* mt = docGia->dsMuonTra.pHead;
    while (mt != nullptr) {
        if (mt->trangThai == 0) { 
            char isbnDangMuon[20];
            tachMaISBN(mt->maSach, isbnDangMuon);
            if (strcmp(isbnDangMuon, isbnCanMuon) == 0) daMuonSachNay = true;
            if (tinhSoNgay(mt->ngayMuon, ngayDungLucMuon) > HAN_MUON) biQuaHan = true;
        }
        mt = mt->pNext;
    }

    if (daMuonSachNay) {
        cout << "Doc gia dang muon 1 cuon sach thuoc cung DAU SACH nay roi! Khong the muon tiep.\n";
        return false;
    }   

    if (biQuaHan) {
        cout << "Ban dang co sach qua han! Vui long tra sach truoc khi muon sach moi!\n";
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
    dauSach->soLuotMuon++;
    docGia->soSachDangMuon++; // Cap nhat O(1)

    cout << "Muon sach thanh cong!\n";
    return true;
}

bool traSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach, Ngay ngayTra) {
    DocGia* docGia = timDocGia(root, maThe);
    if (docGia == nullptr) return false;

    MuonTra* muonTra = docGia->dsMuonTra.pHead;
    while (muonTra != nullptr) {
        if (strcmp(muonTra->maSach, maSach) == 0 && muonTra->trangThai == 0) break;
        muonTra = muonTra->pNext;
    }

    if (muonTra == nullptr) {
        cout << "Khong tim thay thong tin muon sach!\n";
        return false;
    }

    DauSach* dauSach = nullptr;
    Sach* sachTra = timSachTheoMa(ds, maSach, dauSach);
    if (sachTra == nullptr) return false;

    muonTra->trangThai = 1;
    muonTra->ngayTra = ngayTra;
    sachTra->trangThai = 0;
    docGia->soSachDangMuon--; // Cap nhat O(1)

    // Tu dong mo khoa the
    if (docGia->trangThaiThe == 0) {
        bool vanConLoi = false;
        MuonTra* tempMT = docGia->dsMuonTra.pHead;
        while (tempMT != nullptr) {
            if (tempMT->trangThai == 2 || (tempMT->trangThai == 0 && tinhSoNgay(tempMT->ngayMuon, ngayTra) > HAN_MUON)) {
                vanConLoi = true;
                break;
            }
            tempMT = tempMT->pNext;
        }
        if (!vanConLoi) docGia->trangThaiThe = 1;
    }

    cout << "Tra sach thanh cong!\n";
    return true;
}

bool baoMatSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach) {
    DocGia* docGia = timDocGia(root, maThe);
    if (docGia == nullptr) return false;

    MuonTra* muonTra = docGia->dsMuonTra.pHead;
    while (muonTra != nullptr) {
        if (strcmp(muonTra->maSach, maSach) == 0 && muonTra->trangThai == 0) {
            DauSach* dauSach = nullptr;
            Sach* sach = timSachTheoMa(ds, maSach, dauSach);
            if (sach) sach->trangThai = 2;
            
            muonTra->trangThai = 2;
            docGia->trangThaiThe = 0; // Khoa the
            docGia->soSachDangMuon--; // Sach mat thi khong tinh la dang muon nua (bi khoa the roi)
            
            cout << "Da ghi nhan sach bi mat! The da bi khoa.\n";
            return true;
        }
        muonTra = muonTra->pNext;
    }
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
