#include "Sach.h"
#include <cctype>
#include <iostream>
#include <cstring>
#include <limits>
#include "Docgia.h"
#include <ctime>
#include "InputUtils.h"

using namespace std;

//
// -- Code Hoang Te -- 
//

// Ham chuyen chuoi sang chu thuong de tra cuu nhanh hon khi tim kiem theo ten sach, vi khi tim kiem theo ten sach thi nguoi dung se nhap chu thuong
static void capNhatTenSachSearch(DauSach* dauSach) {
    if (dauSach == nullptr) return;

    strncpy(dauSach->tenSachSearch, dauSach->tenSach, sizeof(dauSach->tenSachSearch) - 1);
    dauSach->tenSachSearch[sizeof(dauSach->tenSachSearch) - 1] = '\0';

    for (int i = 0; dauSach->tenSachSearch[i] != '\0'; i++) {
        dauSach->tenSachSearch[i] = tolower((unsigned char)dauSach->tenSachSearch[i]);
    }
}


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

// ---------------------------------------------------------------------------------
// BANG BAM (HASH TABLE) DE TIM ISBN NHANH O(1) THAY VI O(N)
// ---------------------------------------------------------------------------------
unsigned long hashStringISBN(const char* str) {
    unsigned long hash = 5381;
    for (int i = 0; str[i] != '\0'; i++) {
        int c = (unsigned char)str[i];
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % HASH_SIZE; 
}

void insertHashISBN(ListDauSach &ds, DauSach* dsPtr) {
    int idx = hashStringISBN(dsPtr->ISBN);
    int startIdx = idx;
    while (ds.hashTable[idx].daCoDuLieu && !ds.hashTable[idx].daXoa) { // while chạy khi diều kiện đúng: đã có dữ liệu và chưa bị xóa
        if (strcmp(ds.hashTable[idx].isbn, dsPtr->ISBN) == 0) return; 
        idx = (idx + 1) % HASH_SIZE;
        if (idx == startIdx) return; // Bang day (Khong xay ra vi MAX = 1000, HASH_SIZE = 2003)
    }
    strcpy(ds.hashTable[idx].isbn, dsPtr->ISBN); 
    ds.hashTable[idx].dauSachPtr = dsPtr; 
    ds.hashTable[idx].daCoDuLieu = true; 
    ds.hashTable[idx].daXoa = false; // moi khi chen vao doi lai chua xoa
}

void xoaHashISBN(ListDauSach &ds, const char* isbn) {
    int idx = hashStringISBN(isbn); 
    int startIdx = idx;
    while (ds.hashTable[idx].daCoDuLieu) {
        if (!ds.hashTable[idx].daXoa && strcmp(ds.hashTable[idx].isbn, isbn) == 0) { 
            ds.hashTable[idx].daXoa = true; // Danh dau da xoa mem (Tombstone)
            return;
        }
        idx = (idx + 1) % HASH_SIZE; 
        if (idx == startIdx) break; 
    }
}

DauSach* timTheoISBN(const ListDauSach &ds, const char* ISBN) {
    int idx = hashStringISBN(ISBN); 
    int startIdx = idx;
    while (ds.hashTable[idx].daCoDuLieu) {
        if (!ds.hashTable[idx].daXoa && strcmp(ds.hashTable[idx].isbn, ISBN) == 0) {  
            return ds.hashTable[idx].dauSachPtr;
        }
        idx = (idx + 1) % HASH_SIZE; 
        if (idx == startIdx) break; 
    }
    return nullptr;
}
// ---------------------------------------------------------------------------------

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
    capNhatTenSachSearch(p);

    cout << "Nhap tac gia: ";
    cin.getline(p->tacGia, 50);

    cout << "Nhap the loai: ";
    cin.getline(p->theLoai, 30);

    do {
        p->soTrang = nhapSoNguyenHopLe("Nhap so trang (>0): "); 
        if (p->soTrang <= 0) cout << "So trang phai lon hon 0!\n";
    } while (p->soTrang <= 0);

    do {
        p->namXuatBan = nhapSoNguyenHopLe("Nhap nam xuat ban: ");
        if (p->namXuatBan > 2026) cout << "Nam xuat ban khong duoc vuot qua nam hien tai!\n";
    } while (p->namXuatBan > 2026);

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

    insertHashISBN(ds, p); // O(1) chen vao bang bam

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
             << " | Tac gia: " << ds.nodes[i]->tacGia
             << " | The loai: " << ds.nodes[i]->theLoai

             << endl;    }
}

template <typename ListT, typename NodeT>
void addTailLinkedList(ListT &list, NodeT* newNode) {
    newNode->pNext = nullptr;

    if (list.pHead == nullptr) {
        list.pHead = list.pTail = newNode;
    } else {
        list.pTail->pNext = newNode;
        list.pTail = newNode;
    }
}

void addTailSach(ListSach &list, Sach* newSach) {
    addTailLinkedList(list, newSach);
    list.tongSoSach++;
}

void addTailMuonTra(ListMuonTra &list, MuonTra* newNode) {
    addTailLinkedList(list, newNode);
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

    DauSach* toDelete = timTheoISBN(ds, ISBN);
    if (toDelete == nullptr) {
        cout << "Khong tim thay dau sach co ISBN: " << ISBN << ". Xoa that bai.\n";
        return false; 
    }

    Sach* checkSach = toDelete->dsSach.pHead;
    while (checkSach != nullptr) {
        if (checkSach->trangThai == 1) {
            cout << "Khong the xoa dau sach nay vi van con cuon sach dang duoc muon!\n";
            return false;
        }
        checkSach = checkSach->pNext;
    }

    // Tim vi tri cua toDelete trong mang ds.nodes de can xoa
    int indexToDelete = -1;
    for (int i = 0; i < ds.n; i++) { // 
        if (ds.nodes[i] == toDelete) {
            indexToDelete = i;
            break;
        }
    }

    if (indexToDelete < 0) {
        cout << "Loi du lieu: Dau sach ton tai trong hash nhung khong co trong mang nodes.\n";
        return false;
    }

    Sach* tempDelete = toDelete->dsSach.pHead;
    while (tempDelete != nullptr) {
        Sach* next = tempDelete->pNext;
        delete tempDelete;
        tempDelete = next;
    }

    delete toDelete;

    for (int j = indexToDelete; j < ds.n - 1; j++) {
        ds.nodes[j] = ds.nodes[j + 1];
    }

    ds.n--;
    ds.nodes[ds.n] = nullptr;
    
    xoaHashISBN(ds, ISBN); // danh dau xoa trong bang bam

    cout << "Xoa dau sach thanh cong!\n";
    return true;
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

// Ham ho tro tach ma ISBN tu ma sach (VD: "HP01_1" -> "HP01")
void tachMaISBN(const char* maSach, char* isbnOut) {
    int i = 0;
    while (maSach[i] != '_' && maSach[i] != '\0' && i < 19) {
        isbnOut[i] = maSach[i];
        i++;
    }
    isbnOut[i] = '\0';
}

// tim ten sach thi dung binary search.
// tim theo ISBN thi duyet bang bam (hash table) de tim nhanh O(1) sau do moi duyet danh sach cuon sach lien ket don de tim cuon sach co maSach trung khop. Vi vay, ham timSachTheoMa co do phuc tap O(1) + O(M) voi M la so luong cuon sach lien ket don cua dau sach do, thay vi O(N) neu ta duyet toan bo danh sach dau sach va toan bo danh sach cuon sach lien ket don cua tung dau sach de tim theo maSach nhu truoc day.
Sach* timSachTheoMa(const ListDauSach& ds, const char* maSach, DauSach*& dauSachTimThay)
{ 
    dauSachTimThay = nullptr;

    // tach isbn tu maSach (AAAA_0001)
    char isbn [20];
    tachMaISBN(maSach, isbn);

    dauSachTimThay = timTheoISBN(ds, isbn);

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
        str[i] = tolower((unsigned char)str[i]); 
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

        if (ds.nodes[i]->tenSachSearch[0] == '\0') {
            capNhatTenSachSearch(ds.nodes[i]);
        }

        if (strstr(ds.nodes[i]->tenSachSearch, keyLower) != nullptr) {
            cout << "- " << ds.nodes[i]->tenSach << " (ISBN: " << ds.nodes[i]->ISBN << ")\n";
            cout << "  Tac gia: " << ds.nodes[i]->tacGia << ", The loai: " << ds.nodes[i]->theLoai << "\n";
            cout << "Danh sach cuon sach:\n";
            inDanhSachCuonSach(ds.nodes[i]);
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
    cout << "Nhap ISBN can sua: ";
    cin.getline(isbn, 20);

    DauSach* d = timTheoISBN(ds, isbn);
    if (!d) {
        cout << "Khong tim thay!\n";
        return;
    }

    char tenMoi[100];
    cout << "Nhap ten sach moi: ";
    cin.getline(tenMoi, 100);

    cout << "Nhap tac gia moi: ";
    cin.getline(d->tacGia, 50);

    cout << "Nhap the loai moi: ";
    cin.getline(d->theLoai, 30);

    int i;
    for (i = 0; i < ds.n; i++) {
        if (ds.nodes[i] == d) break;
    }

    for (int j = i; j < ds.n - 1; j++) {
        ds.nodes[j] = ds.nodes[j + 1];
    }
    ds.n--;

    strcpy(d->tenSach, tenMoi); 
    capNhatTenSachSearch(d);

    // CHÈN LẠI đúng vị trí
    int pos = timViTriChen(ds, d->tenSach);
    for (int j = ds.n; j > pos; j--) {
        ds.nodes[j] = ds.nodes[j - 1];
    }
    ds.nodes[pos] = d;
    ds.n++;

    cout << "Cap nhat thanh cong!\n";
}
void xoaDauSachUI(ListDauSach &ds) {
    char isbn[20];
    cout << "Nhap ISBN can xoa: ";
    cin.getline(isbn, 20);

    xoaDauSach(ds, isbn);
}

//
// Qucik sort theo the loai, trong do ten tang dan
//

int soSanhTheLoai(DauSach* a, DauSach* b) {
    int cmp = strcmp(a->theLoai, b->theLoai);
    if (cmp == 0)
        return strcmp(a->tenSach, b->tenSach);
    return cmp;
}

void swapDauSach(DauSach* &a, DauSach* &b) {
    DauSach* temp = a;
    a = b;
    b = temp;
}

void insertionSortDauSach(DauSach* arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        DauSach* key = arr[i];
        int j = i - 1;

        while (j >= left && soSanhTheLoai(arr[j], key) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int medianOfThree(DauSach* arr[], int left, int right) {
    int mid = left + (right - left) / 2;

    if (soSanhTheLoai(arr[left], arr[mid]) > 0)
        swapDauSach(arr[left], arr[mid]);

    if (soSanhTheLoai(arr[left], arr[right]) > 0)
        swapDauSach(arr[left], arr[right]);

    if (soSanhTheLoai(arr[mid], arr[right]) > 0)
        swapDauSach(arr[mid], arr[right]);

    return mid;
}

int partitionDauSach(DauSach* arr[], int left, int right) {
    int pivotIndex = medianOfThree(arr, left, right);
    swapDauSach(arr[pivotIndex], arr[right]);

    DauSach* pivot = arr[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (soSanhTheLoai(arr[j], pivot) < 0) {
            i++;
            swapDauSach(arr[i], arr[j]);
        }
    }

    swapDauSach(arr[i + 1], arr[right]);
    return i + 1;
}



void hybridSortDauSach(DauSach* arr[], int left, int right) {
    if (left >= right) return;

    if (right - left + 1 <= INSERTION_THRESHOLD) {
        insertionSortDauSach(arr, left, right);
        return;
    }

    int pivot = partitionDauSach(arr, left, right);

    hybridSortDauSach(arr, left, pivot - 1); 
    hybridSortDauSach(arr, pivot + 1, right);
}

void inTheoTheLoai_TrongDoTenTangDan(ListDauSach &ds) {
    if (ds.n == 0) {
        cout << "Danh sach rong!\n";
        return;
    }

    DauSach* arr[MAX_DAUSACH];

    for (int i = 0; i < ds.n; i++) {
        arr[i] = ds.nodes[i];
    }

    hybridSortDauSach(arr, 0, ds.n - 1);

    cout << "\n===== DANH MUC THE LOAI =====\n";

    char theLoaiHienTai[30] = "";

    for (int i = 0; i < ds.n; i++) {

        if (strcmp(theLoaiHienTai, arr[i]->theLoai) != 0) {
            strcpy(theLoaiHienTai, arr[i]->theLoai);

            cout << "\n=== " << theLoaiHienTai << " ===\n";
        }

        cout << "- " << arr[i]->tenSach
             << " (ISBN: " << arr[i]->ISBN << ")\n";
    }
}

// ----------------------------------------------------------