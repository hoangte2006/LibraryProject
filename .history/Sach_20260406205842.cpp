#include "Sach.h"
#include <cctype>
#include <iostream>
#include <cstring>
#include <limits>
#include "Docgia.h"
#include <ctime>

using namespace std;

//
// -- Code Hoang Te -- 
//


int timViTriChen(const ListDauSach &ds, const char* tenSach) {
    int left = 0;
    int right = ds.n - 1;

    while (left <= right) {
        int mid = left + ((right - left) >> 1); 
        int cmp = strcmp(ds.nodes[mid]->tenSach, tenSach);

        if (cmp == 0)
            return mid;
        else if (cmp < 0 ) 
            left = mid + 1;
        else if (cmp > 0) 
            right = mid - 1;
    }
    return left; 
}

DauSach* timTheoISBN(const ListDauSach &ds, const char* ISBN) {
    for (int i = 0; i < ds.n; i++) {
        if (strcmp(ds.nodes[i]->ISBN, ISBN) == 0) 
            return ds.nodes[i];
    }
    return nullptr;
}

void themDauSach(ListDauSach &ds) {    
    if (ds.n >= MAX_DAUSACH) {
        cout << "Danh sach dau sach da day!\n";
        return;
    }

    DauSach* p = new DauSach; 

    cout << "Nhap ISBN: ";
    cin.getline(p->ISBN, 20); 

    if (timTheoISBN(ds, p->ISBN) != nullptr) {
        cout << "ISBN da ton tai! Them dau sach that bai.\n";
        delete p; 
        return;
    }

    cout << "Nhap ten sach: ";
    cin.getline(p->tenSach, 100);

    cout << "Nhap tac gia: ";
    cin.getline(p->tacGia, 50);

    cout << "Nhap the loai: ";
    cin.getline(p->theLoai, 30);

    cout << "Nhap so trang: ";
    cin >> p->soTrang;

    cout << "Nhap nam xuat ban: ";
    cin >> p->namXuatBan;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    p->soLuotMuon = 0;
    p->dsSach.pHead = p->dsSach.pTail = nullptr;
    p->dsSach.tongSoSach = 0;
    p->sttXoa.top = -1; 

    // may truong hop 0,1,2 ngon lanh
    int pos = timViTriChen(ds, p->tenSach); 

    for (int i = ds.n; i > pos; i--) {
        ds.nodes[i] = ds.nodes[i - 1];
    }
    ds.nodes[pos] = p;
    ds.n++;

    cout << "Them dau sach thanh cong" << endl;
}

void inDanhSachDauSach(const ListDauSach &ds) {
    if (ds.n == 0) {
        cout << "Danh sach nay rong! \n";
        return;
    }

    for (int i = 0; i < ds.n; i++) {
        cout << i + 1 << ". "
             << ds.nodes[i]->tenSach
             << " | ISBN: " << ds.nodes[i]->ISBN
             << " | So cuon: " << ds.nodes[i]->dsSach.tongSoSach
             << endl;    }
}

void addTailSach(ListSach &list, Sach* newSach) {
    newSach->pNext = nullptr;
    // ds rong 
    if (list.pHead == nullptr) {
        list.pHead = list.pTail = newSach;
    } else { 
        list.pTail->pNext = newSach;
        list.pTail = newSach;
    }

    list.tongSoSach++;
}

void addTailMuonTra(ListMuonTra &list, MuonTra* newNode) {
    newNode->pNext = nullptr; 

    // danh sach dang rong
    if (list.pHead == nullptr) {
        list.pHead = list.pTail = newNode; 
    } else {
        list.pTail->pNext = newNode; 
        list.pTail = newNode;
    }
}

int laySoThuTuLonNhat(const ListSach& ds)
{
    int maxSTT = 0;

    const Sach* temp = ds.pHead;

    while (temp != nullptr)
    {
        const char* pos = strchr(temp->maSach, '_'); // tao bien pos luu dia chi , strchr tra ve con tro den ky tu '_'

        if (pos != nullptr) // check lai co ky tu '_'
        {
            int stt = atoi(pos + 1); // lay phan sau ky tu '_' va chuyen doi sang so nguyen de lay so thu tu hien tai cua cuon sach do.

            if (stt > maxSTT)
                maxSTT = stt;
        }

        temp = temp->pNext;
    }

    return maxSTT;
}

void themCuonSach(DauSach* dauSach) {
    if (dauSach == nullptr) return; 

    Sach* newSach = new Sach; 
    
    int stt = laySTTMoi(dauSach);
    snprintf(newSach->maSach, sizeof(newSach->maSach), "%s_%04d", dauSach->ISBN, stt); 
    // %s_%04d in 4 chu so neu thieu thi in ra 0, neu stt = 23 thi in ra 0023, neu stt = 1234 thi in ra 1234.

    cout << "Ma sach duoc tao: " << newSach->maSach << endl;
    
    cout << "Nhap vi tri: ";
    cin.getline(newSach->viTri, sizeof(newSach->viTri));

    newSach->trangThai = 0; 
    newSach->pNext = nullptr; 

    addTailSach(dauSach->dsSach, newSach);
    
    cout << "Them cuon sach thanh cong!\n";
}

bool xoaCuonSach(DauSach* dauSach, const char* maSach) {
    if (dauSach == nullptr || dauSach->dsSach.pHead == nullptr) return false; 

    Sach* prev = nullptr; 
    Sach* current = dauSach->dsSach.pHead; 

    while (current != nullptr) {
        if (strcmp(current->maSach, maSach) == 0) {
            if (current->trangThai == 1) {
                cout << "Khong the xoa. Sach dang duoc muon!\n";
                return false;
            }
            // cuon sach duy nhat
            if (current == dauSach->dsSach.pHead && current == dauSach->dsSach.pTail) {
                dauSach->dsSach.pHead = nullptr;
                dauSach->dsSach.pTail = nullptr;
                
            } else {
                // cuon sach dau tien 
                if (current == dauSach->dsSach.pHead) {
                    dauSach->dsSach.pHead = current->pNext; 
                } else {
                    // khong phai cuon dau tien
                    prev->pNext = current->pNext; 
                }
                // cuon sach cuoi cung
                if (current == dauSach->dsSach.pTail) {
                    dauSach->dsSach.pTail = prev;
                }
            }

            const char* pos = strchr(current->maSach, '_'); 
            // sau dong nay da lay duoc STT 

            if (pos != nullptr)
            {
                int stt = atoi(pos + 1);
                pushSTTXoa(dauSach->sttXoa, stt);            
            }

            delete current; 
            dauSach->dsSach.tongSoSach--;
            cout << "Xoa cuon sach thanh cong!\n";
            return true; 
        }
        prev = current; 
        current = current->pNext; 
    }
    return false; 
}

bool xoaDauSach(ListDauSach &ds, const char* ISBN) {
    for (int i = 0; i < ds.n; i++) {
        if (strcmp(ds.nodes[i]->ISBN, ISBN) == 0) {
            DauSach* toDelete = ds.nodes[i]; 
            

            // viet tam ham nay de day, chua co chuc nang.
            if (toDelete->dsSach.tongSoSach > 0) { 
                cout << "Khong the xoa dau sach vi van con cuon sach!\n";
                return false;
            }

            delete toDelete;

            for (int j = i; j < ds.n - 1; j++) {
                ds.nodes[j] = ds.nodes[j + 1];
            }
            
            ds.n--;
            ds.nodes[ds.n] = nullptr; 
            cout << "Xoa dau sach thanh cong!\n";
            return true;
        }
    }


    cout << "Khong tim thay dau sach co ISBN: " << ISBN << ". Xoa that bai.\n";
    return false; 
}

void inDanhSachCuonSach(const DauSach* dauSach) {
    if (dauSach == nullptr) {
        cout << "Dau sach khong ton tai!\n";
        return;
    }

    Sach* temp = dauSach->dsSach.pHead; 
    
    if (temp == nullptr) {
        cout << " DAU SACH NAY CHUA CO CUON SACH NAO!\n";
        return;
    }

    cout << "\n--- DANH SACH CAC CUON SACH (ISBN: " << dauSach->ISBN << ") ---\n";
    cout << "Ten sach: " << dauSach->tenSach << "\n";
    cout << "----------------------------------------------------------\n";

    int stt = 1; 
    while (temp != nullptr) {
        cout << stt << ". Ma sach: " << temp->maSach
            << " | Vi tri: " << temp->viTri
            << " | Trang thai: ";
        switch (temp->trangThai) {
            case 0: cout << "Cho muon duoc"; break;
            case 1: cout << "Da muon"; break;
            case 2: cout << "Thanh ly"; break;
            default: cout << "Khong xac dinh"; break;
        }

        cout << endl;
        stt++;
        temp = temp->pNext;
    }

}

void traCuuTheoTen(const ListDauSach &ds, const char* tenSach) { 
    if (ds.n == 0) {
        cout << "Danh sach dau sach rong!\n";
        return;
    }

    int left = 0;
    int right = ds.n - 1;

    while (left <= right) {
        int mid = left + ((right - left) >> 1);
        int cmp = strcmp(ds.nodes[mid]->tenSach, tenSach);
    
        if (cmp == 0) {
            const DauSach* found = ds.nodes[mid];
            cout << "DA TIM THAY DAU SACH!\n";
            cout << "ISBN: " << found->ISBN << "\n";
            cout << "Ten sach: " << found->tenSach << "\n"; 
            cout << "Tac gia: " << found->tacGia << "\n";
            cout << "The loai: " << found->theLoai << "\n";
            cout << "So trang: " << found->soTrang << "\n";
            cout << "Nam xuat ban: " << found->namXuatBan << "\n";
            cout << "So luot muon: " << found->soLuotMuon << "\n";
            inDanhSachCuonSach(found);
            return;
        } else {
            if (cmp < 0) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
    }
    cout << "Khong tim thay dau sach co ten: " << tenSach << "\n";

}


// tim ten sach thi dung binary search.
// tim theo ISBN thi duyet o(n)
Sach* timSachTheoMa(const ListDauSach& ds, const char* maSach, DauSach*& dauSachTimThay)
{ 
    dauSachTimThay = nullptr;

    // tach isbn tu maSach (AAAA_0001)
    char isbn [20];
    size_t i = 0;
    while (maSach[i] != '_' && maSach[i] != '\0' && i < sizeof(isbn) - 1)  // sizeof(isbn) - 1 la de dam bao rang khi tach phan ISBN tu maSach, chuoi isbn duoc tao ra se khong vuot qua kich thuoc cua mang isbn, va con lai 1 ky tu de gan ky tu null '\0' o cuoi chuoi sau khi tach xong, de dam bao tinh an toan khi su dung sau nay.
    {
        isbn[i] = maSach[i];
        i++;
    }
    isbn[i] = '\0'; 

    // tim dau sach theo ISBN
    for (int j = 0; j < ds.n; j++) {
        if (ds.nodes[j] == nullptr) continue; 
        if (strcmp(ds.nodes[j]->ISBN, isbn) == 0) {
            dauSachTimThay = ds.nodes[j];
            break;
        }
    }

    if (dauSachTimThay == nullptr) {
        return nullptr; 
    }

    Sach* temp = dauSachTimThay->dsSach.pHead;
    while (temp != nullptr) {
        if (strcmp(temp->maSach, maSach) == 0) {
            return temp; 
        }
        temp = temp->pNext; 
    
    }
    return nullptr;

}

void toLowerCase(char* str) { 
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]); // tranh viec truyen ky tu co dau sang ham tolower
    }
}

void traCuuTheoTenNangCao(const ListDauSach &ds, const char* key) {
    bool found = false;
    
    if (ds.n == 0) {
        cout << "Danh sach dau sach rong!\n";
        return;
    }

    char keyLower[100]; 
    strncpy(keyLower, key, sizeof(keyLower)-1); 
    keyLower[sizeof(keyLower)-1] = '\0'; 
    toLowerCase(keyLower); 

    cout << "Cac dau sach co ten tuong doi giong voi: " << key << "\n";
    for (int i = 0; i < ds.n; i++) { 

        if (ds.nodes[i] == nullptr) continue; 

        char tenSachLower[100]; 
        strncpy(tenSachLower, ds.nodes[i]->tenSach, sizeof(tenSachLower)-1);
        tenSachLower[sizeof(tenSachLower)-1] = '\0';
        toLowerCase(tenSachLower);

        if (strstr(tenSachLower, keyLower) != nullptr) {
            cout << "- " << ds.nodes[i]->tenSach << " (ISBN: " << ds.nodes[i]->ISBN << ")\n";
            found = true;
        }
    }
    if (!found) {
        cout << "Khong tim thay dau sach co ten giong voi: " << key << "\n";
    }
}

void giaiPhongDanhSachDauSach(ListDauSach &ds) {
    for (int i = 0; i < ds.n; i++) {
        Sach* temp = ds.nodes[i]->dsSach.pHead;

        while (temp != nullptr) {
            Sach* next = temp->pNext;
            delete temp;
            temp = next;
        }

        delete ds.nodes[i];
        ds.nodes[i] = nullptr; // cho an toan
    }

    ds.n = 0;
}

void pushSTTXoa(StackSTT &s, int stt)
{
    if (s.top < MAX_STT_STACK - 1) { 
        s.data[++s.top] = stt; 
    } else {
        cout << "Stack sttXoa da day! Khong the luu them STT da xoa.\n";
    }
}

int laySTTMoi(DauSach* dauSach)
{
    if (dauSach->sttXoa.top >= 0) // tai su dung lai so thu tu da bi xoa de tao maSach cho cuon sach moi.
        return dauSach->sttXoa.data[dauSach->sttXoa.top--];

    return laySoThuTuLonNhat(dauSach->dsSach) + 1;
}

void hieuChinhDauSach(ListDauSach &ds) {
    char isbn[20];
    cout << "Nhap ISBN cua dau sach can hieu chinh: ";
    cin.getline(isbn, 20);

    DauSach* dauSach = timTheoISBN(ds, isbn);

    if (dauSach == nullptr) {
        cout << "Khong tim thay dau sach co ISBN: " << isbn << "\n";
        return;
    }
    cout << "Nhap ten sach moi (nhan Enter de giu nguyen): ";
    char tenSach[100];
    cin.getline(tenSach, 100);\
}


//
// -- Code Huu Tri -- 
//


//
// -- Code Ho Nghia -- 
//