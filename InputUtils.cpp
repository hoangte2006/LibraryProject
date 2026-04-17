#include "InputUtils.h"
#include <iostream>
#include <limits>

using namespace std;

#include "Structs.h"

bool kiemTraNgayHopLe(const Ngay& ngay) {
	if (ngay.thang < 1 || ngay.thang > 12) return false;
	if (ngay.ngay < 1 || ngay.ngay > 31) return false;
	if (ngay.nam < 1900 || ngay.nam > 2100) return false;
	const int ngayTrongThang[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	int maxNgay = ngayTrongThang[ngay.thang];
	if (ngay.thang == 2) {
		bool laNamNhuan = (ngay.nam % 4 == 0 && ngay.nam % 100 != 0) || (ngay.nam % 400 == 0);
		if (laNamNhuan) maxNgay = 29; 
	}
	return ngay.ngay <= maxNgay;
}

int nhapSoNguyenHopLe(const char* thongBao) {
    int so;
    while (true) {
        cout << thongBao;
        if (cin >> so) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xoa bo nho dem sau khi nhap so
            return so;
        } else {
            if (cin.eof()) {
                exit(0);
            }
            cout << "Loi: Vui long chi nhap so nguyen!" << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xoa bo nho dem de cho lan nhap tiep theo
        }
    }
}
