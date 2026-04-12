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
		bool laNamNhuan = (ngay.nam % 4 == 0 && ngay.nam % 100 != 0) || (ngay.nam % 400 == 0); // ví dụ năm 2000 là năm nhuận, năm 1900 không phải năm nhuận, phét tính năm nhuận theo quy tắc: chia hết cho 4 nhưng không chia hết cho 100, hoặc chia hết cho 400. ví dụ năm 2000 là năm nhuận, năm 1900 không phải năm nhuận.vì năm 1900 chia hết cho 100 nhưng không chia hết cho 400, nên không phải là năm nhuận.
		if (laNamNhuan) maxNgay = 29;
	}
	return ngay.ngay <= maxNgay;
}

