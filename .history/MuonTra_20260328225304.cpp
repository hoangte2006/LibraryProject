#include "MuonTra.h"
#include "Docgia.h"
#include "Sach.h"
#include <ctime>

using namespace std;

// Ham layNgayHienTai de lay ngay thang nam hien tai tu he thong
// struct tm* now = localtime(&t); la ham localtime de chuyen doi thoi gian hien tai tu dang time_t (so giay tinh tu 1/1/1970) sang dang struct tm, trong do struct tm la mot cau truc du lieu chua thong tin ve ngay thang nam, gio phut giay, va cac thong tin lien quan den thoi gian khac. Ham localtime se tra ve con tro den struct tm chua thong tin ve ngay thang nam hien tai duoc lay tu he thong, va chung ta co the truy cap vao cac truong ngay, thang, nam trong struct tm do de gan gia tri cho bien ngayHT trong ham layNgayHienTai.
Ngay layNgayHienTai() {
    time_t t = time(nullptr); // lay tong gia tu 1/1/1970;
    struct tm* now = localtime(&t); // tinhs ra ngay thang nam hien tai.
    Ngay ngayHT;
    ngayHT.ngay = now->tm_mday; // tra ve ngay hien tai co dang so nguyen tu 1-31.
    ngayHT.thang = now->tm_mon + 1; // +1 vi thang trong struct tm tinh tu 0; tra ve dang so nguyen tu 1-12.
    ngayHT.nam = now->tm_year + 1900; // +1900 vi nam trong struct tm tinh tu 1900; tra ve dang so nguyen tu 1900 tro len. vd: nam 2024 se duoc tra ve la 124, nen can cong them 1900 de tra ve nam thuc te.
    return ngayHT;
}

// Ham nay chuyen ve dang so giay cho de tinh, nhu sau:
time_t chuyenNgaySangTimeT(const Ngay& ngay) {
    struct tm tmNgay = {0}; // Dat cac truong ngay, thang, nam trong struct tm ve 0 de dam bao tinh chinh xac khi chuyen doi sang dang time_t sau nay, tranh cac van de lien quan den gia tri rac trong struct tm khi chuyen doi.
    tmNgay.tm_mday = ngay.ngay; // gan ngay cho truong tm_mday trong struct tm
    tmNgay.tm_mon = ngay.thang - 1;  // gan thang cho truong tm_mon trong struct tm, can tru di 1 vi thang trong struct tm duoc tinh tu 0 (thang 1 la 0, thang 2 la 1, ..., thang 12 la 11)
    tmNgay.tm_year = ngay.nam - 1900;  // gan nam cho truong tm_year trong struct tm, can tru di 1900 vi nam trong struct tm duoc tinh tu 1900 (nam 1900 la 0, nam 1901 la 1, ..., nam 2024 la 124)
    return mktime(&tmNgay); // mktime de tinh tong giay ve time_t;
}

// difftime cho an toan hon khi tinh toan so ngay giua 2 ngay; va no so sanh ngay luon roi, ham soSanhNgay chua can dung den.
// phu hop voi he thong tinh ngay thang nam hien tai cua may tinh
// de tranh cac van de lien quan den tinh toan so ngay khi co su thay doi ve gio he thong (chuyen sang/ra khoi gio mua he, thay doi thoi gian he thong, v.v.) co the anh huong den ket qua tinh toan so ngay neu chi tinh toan theo cach tinh toan so ngay don gian bang cach nhan so ngay voi 24*60*60 giay/ngay ma khong su
int tinhSoNgay(const Ngay& ngayMuon, const Ngay& ngayHienTai) {
    time_t timeMuon = chuyenNgaySangTimeT(ngayMuon);
    time_t timeHienTai = chuyenNgaySangTimeT(ngayHienTai);
    double seconds = difftime(timeHienTai, timeMuon);  
    
    return (int)(seconds / (60 * 60 * 24));
}

// Ham muonSach co cac buoc nhu:
// 1. Tim Doc Gia theo ma the, neu khong tim thay thi thong bao va tra ve false.
// 2. Kiem tra trang thai the doc gia, neu da bi khoa thi thong bao va tra ve false.
// 3. Kiem tra xem doc gia da muon sach nay chua, neu da muon roi thi thong bao va tra ve false.
// 4. Kiem tra xem doc gia da muon qua 3 cuon sach chua, neu da muon qua 3 cuon thi thong bao khoa the va tra ve false.
// 5. Kiem tra xem doc gia co sach nao da qua han chua, neu co sach qua han thi thong bao va tra ve false.
// 6. Tim cuon sach theo maSach, neu khong tim thay thi thong bao va tra ve false.
// 7. Kiem tra trang thai cua cuon sach, neu cuon sach khong co san de muon thi thong bao va tra ve false.
// 8. Neu tat ca cac dieu kien tren duoc thoa man, thi tien hanh muon sach: tao moi mot muonTra moi de luu thong tin muon sach, them vao cuoi danh sach muon tra cua doc gia, cap nhat trang thai cua cuon sach sang da muon, tang luot muon cho dau sach, thong bao muon sach thanh cong va tra ve true.
bool muonSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach, Ngay ngayMuon) {

    DocGia* docGia = timDocGia(root, maThe);
    if (docGia == nullptr) {
        cout << "Khong tim thay doc gia co ma the: " << maThe << "\n";
        return false;
    }

    if (docGia->trangThaiThe == 0) {
        cout << "The doc gia da bi khoa. Khong the muon sach!\n";
        return false;
    }

    if (dangMuonSach(docGia, maSach)) {
        cout << "Doc gia da muon sach nay roi!\n";
        return false;
    }   

    if (demSachDangMuon(docGia) >= MAX_SACH_MUON) {
        cout << "Doc gia da muon qua 3 cuon! Khong the muon them!. Khoa the!\n";
        docGia->trangThaiThe = 0; // khoa the doc gia vi muon qua 3 cuon sach, vuot qua gioi han cho phep trong quy dinh cua thu vien, de bao ve tai san sach va khuyen khich doc gia tra sach dung han.
        return false;
    }

    if (coSachQuaHan(docGia, ngayMuon)) {
        cout << "Doc gia co sach qua han! Khong the muon sach!\n";
        return false;
    }

    // tim cuon sach theo maSach, va cung luc tim duoc dau sach chua cuon sach do de sau nay tang luot muon khi muon sach thanh cong.
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
    
    // Toa moi mot muonTra moi de luu thong tin muon sach.
    MuonTra* muonTra = new MuonTra; 

    //strcpy(muonTra->maSach, maSach); De an toan hon khi copy maSach vao muonTra->maSach, tranh truong hop maSach co do dai vuot qua kich thuoc cua muonTra->maSach (25), neu do dai vuot qua thi strncpy se cat bo phan vuot qua va khong copy vao muonTra->maSach
    // de tranh loi ghi de bo nho va cac van de lien quan den tinh an toan khi su dung sau nay. Ham strncpy se copy toi da sizeof(muonTra->maSach)-1 ky tu tu maSach vao muonTra->maSach, va sau do gan ky tu ket thuc chuoi '\0' o cuoi de dam bao tinh an toan khi su dung sau nay.
    
    strncpy(muonTra->maSach, maSach, sizeof(muonTra->maSach)-1);
    // tam thoi dung cai nay, sap toi lam nhapma rang buoc se giup
    muonTra->maSach[sizeof(muonTra->maSach)-1] = '\0'; // van de cua strncpy la no khong tu dong them ky tu ket thuc chuoi '\0' neu do dai chuoi nguon vuot qua kich thuoc cua chuoi dich, nen can phai gan ky tu '\0' o cuoi de dam bao tinh an toan khi su dung sau nay.

    muonTra->ngayMuon = ngayMuon;
    muonTra->trangThai = 0; // danh dau da muon la 0.
    muonTra->pNext = nullptr; //

    // thêm vào cuối danh sách
    addTailMuonTra(docGia->dsMuonTra, muonTra);

    // cap nhat trang thai sach sau khi muon
    sachMuon->trangThai = 1;

    // tang luot muon
    dauSach->soLuotMuon++;

    cout << "Muon sach thanh cong!\n";
    return true;
}

bool traSach(TREE_DocGia root, ListDauSach &ds, int maThe, const char* maSach, Ngay ngayTra) {
    // Tim Doc Gia 
    DocGia* docGia = timDocGia(root, maThe);
    if (docGia == nullptr) {
        cout << "Khong tim thay doc gia co ma the: " << maThe << "\n";
        return false;
    }

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
            
        // chỉ cập nhật khi chắc chắn tìm được sách
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

    if (!dangMuonSach(docGia, maSach)) {
        cout << "Doc gia khong muon sach nay!\n";
        return false;
    }

    MuonTra* muonTra = docGia->dsMuonTra.pHead;

    while (muonTra != nullptr) {
        if (strcmp(muonTra->maSach, maSach) == 0 &&
            muonTra->trangThai == 0)
        { 
            sach->trangThai = 2; // thanh ly
            muonTra->trangThai = 2; // mat sach
            // dauSach->dsSach.tongSoSach--; mat sach != xoa sach   

            cout << "Da ghi nhan sach bi mat!\n";
            return true;
        }

        muonTra = muonTra->pNext;
    }

    cout << "Khong tim thay phieu muon\n";
    return false;
}

bool coSachQuaHan(const DocGia* docGia, const Ngay& ngayHienTai) {
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

void xemSachDangMuon(const DocGia* docGia)