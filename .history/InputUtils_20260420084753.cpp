#include "InputUtils.h"
#include <iostream>
#include <limits>
#include <conio.h>
#include <cctype>
#include <cstring>

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

// --- Cac ham nhap lieu nang cao (bat tung ky tu) ---

static void veLaiDongNhapLieu(const char* thongBao, const char* buffer, int len, int vt)
{
    cout << "\r" << thongBao;

    for (int i = 0; i < len; i++)
        cout << buffer[i];

    cout << "   "; // Xóa các ký tự dư thừa nếu có

    cout << "\r" << thongBao;

    for (int i = 0; i < vt; i++)
        cout << buffer[i];

    cout.flush(); 
}

void nhapMa(const char* thongBao, char* result, int maxLen)
bool nhapMa(const char* thongBao, char* result, int maxLen)
{
    int len = 0;
    int vt = 0;
    result[0] = '\0';

    cout << thongBao;

    while (true)
    {
        int c = _getch();

        if (c == 27) // ESC
        {
            cout << endl;
            return false;
        }

        if (c == 13) // ENTER
        {
            if (len > 0)
            {
                result[len] = '\0';
                cout << endl;
                return;
                return true;
            }
            cout << "\a"; // Beep neu chuoi rong
            continue;
        }

        if (c == 0 || c == 224) // Phim chuc nang
        {
            int key = _getch();
            if (key == 75 && vt > 0) vt--; // LEFT
            else if (key == 77 && vt < len) vt++; // RIGHT
            else if (key == 83 && vt < len) // DELETE
            {
                for (int i = vt; i < len - 1; i++) result[i] = result[i + 1];
                len--;
                result[len] = '\0';
            }
            veLaiDongNhapLieu(thongBao, result, len, vt);
            continue;
        }

        if (c == 8) // BACKSPACE
        {
            if (vt > 0)
            {
                for (int i = vt - 1; i < len - 1; i++) result[i] = result[i + 1];
                vt--;
                len--;
                result[len] = '\0';
            }
            veLaiDongNhapLieu(thongBao, result, len, vt);
            continue;
        }

        // Chi nhan ky tu so, chu cai, '-' va '_'
        if ((isalnum(c) || c == '-' || c == '_') && len < maxLen - 1) 
        {
            if (isalpha(c)) c = toupper(c);

            for (int i = len; i > vt; i--) result[i] = result[i - 1];

            result[vt] = (char)c;
            vt++;
            len++;
            veLaiDongNhapLieu(thongBao, result, len, vt);
        }
        else
        {
            cout << "\a"; // Beep khi nhap sai hoac day
        }
    }
}

// ham nay cho phep nhap ten hoac ho va ten, chi cho phep ky tu chu cai va khoang trang
// tu dau moi tu viet hoa, khong cho phep 2 khoang trang lien tiep, khong cho phep khoang trang o dau va cuoi
// va co the di chuyen con tro de sua loi khi dang nhap. Neu nhap sai thi se co am thanh va khong chap nhan ky tu do.
void nhapTen(const char* thongBao, char* result, int maxLen) {
bool nhapTen(const char* thongBao, char* result, int maxLen) {
    int len = 0;
    int vt = 0;
    result[0] = '\0';

    cout << thongBao;

    while (true)
    {
        int c = _getch();

        if (c == 27) // ESC
        {
            cout << endl;
            return false;
        }

        if (c == 13) // ENTER
        {
            while (len > 0 && result[len - 1] == ' ') len--; // Trim trailing space
            result[len] = '\0';
            if (len > 0) {
                 cout << endl;
                 return;
                 return true;
            }
            cout << "\a"; // Beep neu chuoi rong
            veLaiDongNhapLieu(thongBao, result, len, vt);
        }
        else if (c == 0 || c == 224) // Phim chuc nang
        {
            int key = _getch();
            if (key == 75 && vt > 0) vt--;
            else if (key == 77 && vt < len) vt++;
            else if (key == 83 && vt < len) {
                for (int i = vt; i < len - 1; i++) result[i] = result[i + 1];
                len--;
                result[len] = '\0';
            }
            veLaiDongNhapLieu(thongBao, result, len, vt);
        }
        else if (c == 8) // BACKSPACE
        {
            if (vt > 0) {
                for (int i = vt - 1; i < len - 1; i++) result[i] = result[i + 1];
                vt--;
                len--;
                result[len] = '\0';
            }
            veLaiDongNhapLieu(thongBao, result, len, vt);
        }
        else if ((isalpha(c) || (isspace(c) && vt > 0 && result[vt-1] != ' ')) && len < maxLen - 1)
        {
            for (int i = len; i > vt; i--) result[i] = result[i - 1];
            char charToInsert = (char)c;
            if (isalpha(charToInsert)) {
                // Yeu cau moi: Tu dong IN HOA toan bo ten
                charToInsert = toupper(charToInsert);
            }
            result[vt] = charToInsert;
            vt++;
            len++;
            veLaiDongNhapLieu(thongBao, result, len, vt);
        } else {
            cout << "\a";
        }
    }
}

void nhapChuoiTuDo(const char* thongBao, char* result, int maxLen) {
bool nhapChuoiTuDo(const char* thongBao, char* result, int maxLen) {
    int len = 0;
    int vt = 0;
    result[0] = '\0';
    cout << thongBao;
    while (true) {
        int c = _getch();
        if (c == 27) { // ESC
            cout << endl;
            return false;
        }
        if (c == 13) { // ENTER
            while (len > 0 && result[len - 1] == ' ') len--; // Trim trailing space
            while (len > 0 && result[0] == ' ') { memmove(result, result + 1, len--); } // Trim leading space
            result[len] = '\0';
            cout << endl;
            return;
            return true;
        } else if (c == 0 || c == 224) { // Phim chuc nang
            int key = _getch();
            if (key == 75 && vt > 0) vt--;
            else if (key == 77 && vt < len) vt++;
            else if (key == 83 && vt < len) {
                for (int i = vt; i < len - 1; i++) result[i] = result[i + 1];
                len--;
                result[len] = '\0';
            }
        } else if (c == 8) { // BACKSPACE
            if (vt > 0) {
                for (int i = vt - 1; i < len - 1; i++) result[i] = result[i + 1];
                vt--;
                len--;
                result[len] = '\0';
            }
        } else if (isprint(c) && len < maxLen - 1) { // Cho phep ky tu in duoc
            for (int i = len; i > vt; i--) result[i] = result[i - 1];
            result[vt] = (char)c;
            vt++;
            len++;
        } else {
            cout << "\a";
        }
        veLaiDongNhapLieu(thongBao, result, len, vt);
    }
}
