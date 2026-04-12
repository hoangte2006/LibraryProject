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

// Ham muonSach co cac buoc nhu:
bool muonSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach) {
    Ngay ngayMuon = layNgayHienTai();

    DocGia* docGia = timDocGia(root, maThe);
    if (docGia == nullptr) {
        cout << "Khong tim thay doc gia co ma the: " << maThe << "\n";
        return false;
    }

    if (docGia->trangThaiThe == 0) {
        cout << "The doc gia dang bi khoa! Khong the muon sach.\n";
        return false;
    }

    DauSach* dauSach = nullptr;
    Sach* sachMuon = timSachTheoMa(ds, maSach, dauSach);

    if (sachMuon == nullptr) {
        cout << "Khong tim thay cuon sach co ma: " << maSach << "\n";
        return false;
    }

    if (sachMuon->trangThai != 0) {
        cout << "Cuon sach khong co san de muon!\n";
        return false;
    }
    // --------------------------------------------------------------------------

    int soSachDangMuon = 0;
    bool daMuonSachNay = false;
    bool biQuaHan = false;

    MuonTra* mt = docGia->dsMuonTra.pHead;
    while (mt != nullptr) {
        if (mt->trangThai == 0) { 
            soSachDangMuon++;
            
            if (strcmp(mt->maSach, maSach) == 0) {
                daMuonSachNay = true;
            }

            if (tinhSoNgay(mt->ngayMuon, ngayMuon) > HAN_MUON) { 
                biQuaHan = true;
            }
        }
        mt = mt->pNext;
    }

    if (daMuonSachNay) {
        cout << "Doc gia da muon sach nay roi!\n";
        return false;
    }   

    if (soSachDangMuon >= MAX_SACH_MUON) {
        cout << "Doc gia da muon toi da 3 cuon! Khong the muon them.\n";
        return false;
    }       

    if (biQuaHan) {
        docGia->trangThaiThe = 0; 
        cout << "Ban dang co sach qua han! Vui long tra sach truoc khi muon sach moi!\n";
        return false;
    }
    // ----------------------------------------------------------------------

    MuonTra* muonTra = new MuonTra; 
    strncpy(muonTra->maSach, maSach, sizeof(muonTra->maSach)-1);

    // tam thoi dung cai nay, sap toi lam nhapma rang buoc se giup
    muonTra->maSach[sizeof(muonTra->maSach)-1] = '\0'; 

    muonTra->ngayMuon = ngayMuon;
    muonTra->trangThai = 0; // 0 da muon
    muonTra->pNext = nullptr; 

    addTailMuonTra(docGia->dsMuonTra, muonTra);
    sachMuon->trangThai = 1;
    dauSach->soLuotMuon++;

    cout << "Muon sach thanh cong!\n";
    return true;
}

bool traSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach, Ngay ngayTra) {
    DocGia* docGia = timDocGia(root, maThe);
    if (docGia == nullptr) {
        cout << "Khong tim thay doc gia co ma the: " << maThe << "\n";
        return false;
    }

    DauSach* dauSach = nullptr;
    Sach* sachTra = timSachTheoMa(ds, maSach, dauSach);

    if (sachTra == nullptr) {
        cout << "Khong tim thay sach trong he thong!\n";
        return false;
    }

    if (sachTra->trangThai != 1) {
        cout << "Sach khong o trang thai dang muon!\n";
        return false;
    }
    // ---------------------------------

    if (docGia->dsMuonTra.pHead == nullptr) {
        cout << "Doc gia chua muon sach nao!\n";
        return false;
    }

    MuonTra* muonTra = docGia->dsMuonTra.pHead;

    while (muonTra != nullptr) {
        if (strcmp(muonTra->maSach, maSach) == 0 && muonTra->trangThai == 0) { 

            if (!soSanhNgay(muonTra->ngayMuon, ngayTra)) {
                cout << "Ngay tra khong hop le!\n";
                return false;
            }

            muonTra->trangThai = 1;
            muonTra->ngayTra = ngayTra;
            sachTra->trangThai = 0;

            cout << "Tra sach thanh cong!\n";
            return true;
        }
        muonTra = muonTra->pNext;
    }
    cout << "Khong tim thay thong tin muon sach cho ma sach: " << maSach << " va ma the: " << maThe << "\n";
    return false;
}

// Ham bao mat sach khi doc gia lam mat sach dang muon.
// ham se cap nhat trang thai cua cuon sach do thanh 2 (lam mat).
// va cap nhat trang thai cua phieu muon do thanh 2 (mat sach).
// Ham se tra ve true neu da ghi nhan duoc viec sach bi mat.
// va tra ve false neu khong tim thay thong tin phu hop de ghi nhan viec sach bi mat.
bool baoMatSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach)
{
    DauSach* dauSach = nullptr;
    Sach* sach = timSachTheoMa(ds, maSach, dauSach);    
    
    if (sach == nullptr) {
        cout << "Khong tim thay sach\n";
        return false;
    }

    if (sach->trangThai != 1) {
        cout << "Sach khong phai o trang thai dang muon!\n";
        return false;
    }

    DocGia* docGia = timDocGia(root, maThe);

    if (docGia == nullptr) {
        cout << "Khong tim thay doc gia\n";
        return false;
    }

    MuonTra* muonTra = docGia->dsMuonTra.pHead;

    while (muonTra != nullptr) {
        if (strcmp(muonTra->maSach, maSach) == 0 &&
            muonTra->trangThai == 0)
        { 
            sach->trangThai = 2; 
            muonTra->trangThai = 2; 
            // dauSach->dsSach.tongSoSach--; mat sach != xoa sach   

            cout << "Da ghi nhan sach bi mat!\n";
            return true;
        }

        muonTra = muonTra->pNext;
    }

    cout << "Doc gia khong muon sach nay hoac khong tim thay!\n";
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

void xemSachDangMuon(const DocGia* docGia) {
    MuonTra* temp = docGia->dsMuonTra.pHead;

    while (temp != nullptr) {
        if (temp->trangThai == 0) {
            cout << "Ma sach: " << temp->maSach << " | Ngay muon: " 
                << temp->ngayMuon.ngay << "/" << temp->ngayMuon.thang << "/" << temp->ngayMuon.nam << endl;
        }
        temp = temp->pNext;
    }
}
