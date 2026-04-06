#include <iostream>
#include "MuonTra.h"
#include "Docgia.h"
#include "Sach.h"

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

// Ham nay chuyen ve dang so giay cho de tinh;
time_t chuyenNgaySangTimeT(const Ngay& ngay) {
    struct tm tmNgay = {0}; // Dat cac truong ngay, thang, nam trong struct tm ve 0 de dam bao tinh chinh xac khi chuyen doi sang dang time_t sau nay, tranh cac van de lien quan den gia tri rac trong struct tm khi chuyen doi.
    tmNgay.tm_mday = ngay.ngay; // gan ngay cho truong tm_mday trong struct tm
    tmNgay.tm_mon = ngay.thang - 1;  // gan thang cho truong tm_mon trong struct tm, can tru di 1 vi thang trong struct tm duoc tinh tu 0 (thang 1 la 0, thang 2 la 1, ..., thang 12 la 11)
    tmNgay.tm_year = ngay.nam - 1900;  // gan nam cho truong tm_year trong struct tm, can tru di 1900 vi nam trong struct tm duoc tinh tu 1900 (nam 1900 la 0, nam 1901 la 1, ..., nam 2024 la 124)
    return mktime(&tmNgay); // mktime de tinh tong giay ve time_t;
}

int tinhSoNgay(const Ngay& ngayMuon, const Ngay& ngayHienTai) {
    return ;
}