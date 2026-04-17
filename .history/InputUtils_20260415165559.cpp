#include "InputUtils.h"
#include <iostream>

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

  int nhapSoNguyenHopLe(const char* thongBao) {                                            │
│ 22       int so;                                                                              │
│ 23 -     cout << thongBao;                                                                    │
│ 24 -     while (!(cin >> so)) {                                                               │
│ 25 -         cout << "Sai kieu du lieu! Vui long chi nhap so: ";                              │
│ 26 -         cin.clear();                                                                     │
│ 27 -         cin.ignore(1000, '\n');                                                          │
│ 23 +     while (true) {                                                                       │
│ 24 +         cout << thongBao;                                                                │
│ 25 +         if (cin >> so) {                                                                 │
│ 26 +             cin.ignore(10000, '\n'); // Dọn dẹp bộ đệm để tránh trôi lệnh cho getline    │
│    sau này                                                                                    │
│ 27 +             return so;                                                                   │
│ 28 +         } else {                                                                         │
│ 29 +             cout << "Loi: Vui long chi nhap so nguyen!" << endl;                         │
│ 30 +             cin.clear();                                                                 │
│ 31 +             cin.ignore(10000, '\n');                                                     │
│ 32 +         }                                                                                │
│ 33       }                                                                                    │
│ 29 -     return so;                                                                           │
│ 34   }                                                                                        │
                      
