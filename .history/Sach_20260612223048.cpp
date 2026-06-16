#include "Sach.h"
#include <cctype>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <limits>
#include "Docgia.h"
#include <ctime>
#include "InputUtils.h"
#include <conio.h>

using namespace std;

void chuanHoaChuoiSearch(const char* src, char* dest) {
    int i = 0, j = 0;
    bool space = false;
    while (src[i] == ' ') i++; // Bo qua khoang trang dau
    while (src[i] != '\0') { 
        if (src[i] != ' ') {
            dest[j++] = tolower((unsigned char)src[i]);
            space = false;
        } else if (!space) {
            dest[j++] = ' '; // Chi giu 1 khoang trang giua cac tu
            space = true;
        }
        i++;
    }
    if (j > 0 && dest[j - 1] == ' ') j--; // Bo qua khoang trang cuoi
    dest[j] = '\0';
}

void capNhatTenSachSearch(DauSach* dauSach) {
    if (dauSach == nullptr) return;
    chuanHoaChuoiSearch(dauSach->tenSach, dauSach->tenSachSearch);
}

void capNhatTacGiaSearch(DauSach* dauSach) {
    if (dauSach == nullptr) return;
    chuanHoaChuoiSearch(dauSach->tacGia, dauSach->tacGiaSearch);
}

void chuanHoaISBN(char* isbn) {
    int j = 0;
    for (int i = 0; isbn[i] != '\0'; i++) {
        unsigned char c = (unsigned char)isbn[i];
        if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
            isbn[j++] = toupper(c);
        }
    }
    isbn[j] = '\0';
}


int timViTriChen(const ListDauSach &ds, const char* tenSach) {
    int left = 0;
    int right = ds.n - 1;

    while (left <= right) {
        int mid = left + ((right - left) >> 1); 
        int cmp = stricmp(ds.nodes[mid]->tenSach, tenSach);

        if (cmp == 0)
            return mid;
        else if (cmp < 0 ) 
 
        if (cmp < 0) {
            left = mid + 1;
        else if (cmp > 0) 
        } else { // Nếu tên sách ở mid >= tên sách cần chèn, ta tìm ở nửa bên trái (bao gồm cả mid)
            right = mid - 1;
        }
    }
    return left; 
}

// --- QUAN LY MANG DONG DAU SACH ---

// Ham noi bo de noi rong mang dong khi day
static void resizeListDauSach(ListDauSach& ds) {
    int newCapacity = (ds.capacity == 0) ? 16 : ds.capacity * 2;
    
    // Cap phat mang con tro moi
    DauSach** newNodes = new DauSach*[newCapacity];

    // Sao chep con tro tu mang cu sang mang moi
    for (int i = 0; i < ds.n; i++) {
        newNodes[i] = ds.nodes[i];
    }

    // Giai phong mang con tro cu (neu co)
    if (ds.nodes != nullptr) {
        delete[] ds.nodes;
    }

    // Cap nhat con tro va capacity cua ListDauSach
    ds.nodes = newNodes;
    ds.capacity = newCapacity;
}

// Ham them vao cuoi, chi dung cho load file de toi uu toc do
void appendDauSach(ListDauSach& ds, DauSach* p) {
    if (ds.n == ds.capacity) {
        resizeListDauSach(ds);
    }
    ds.nodes[ds.n] = p;
    ds.n++;
}

// Ham them mot DauSach vao mang dong, duy tri sap xep
void addDauSachToList(ListDauSach& ds, DauSach* p) {
    if (ds.n == ds.capacity) {
        resizeListDauSach(ds);
    }

    int pos = timViTriChen(ds, p->tenSach);

    for (int i = ds.n; i > pos; i--) {
        ds.nodes[i] = ds.nodes[i - 1];
    }
    ds.nodes[pos] = p;
    ds.n++;
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
    int tombstoneIdx = -1; // vi tri tombstone dau tien gap duoc de tai su dung

    // Probe qua CA tombstone, chi dung tai o TRONG (daCoDuLieu=false)
    while (ds.hashTable[idx].daCoDuLieu) {
        if (!ds.hashTable[idx].daXoa) {
            if (strcmp(ds.hashTable[idx].isbn, dsPtr->ISBN) == 0) return; // da ton tai
        } else if (tombstoneIdx == -1) {
            tombstoneIdx = idx; // ghi nho tombstone dau tien de tai su dung
        }
        idx = (idx + 1) % HASH_SIZE;
        if (idx == startIdx) {
            if (tombstoneIdx != -1) break; // bang day nhung co tombstone de dung
            cout << "LOI NGHIEM TRONG: Bang bam (Hash Table) da day! Vui long kiem tra lai HASH_SIZE.\n";
            return;
        }
    }

    // Chen vao tombstone neu co (tranh phan manh), nguoc lai chen vao o trong, thieu no thi nó khong biet thang o phia sau luon
    int insertIdx = (tombstoneIdx != -1) ? tombstoneIdx : idx;
    strcpy(ds.hashTable[insertIdx].isbn, dsPtr->ISBN);
    ds.hashTable[insertIdx].dauSachPtr = dsPtr;
    ds.hashTable[insertIdx].daCoDuLieu = true;
    ds.hashTable[insertIdx].daXoa = false;
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

void inDanhSachDauSach(const ListDauSach &ds) {
    if (ds.n == 0) {
        cout << "Danh sach dau sach rong! \n";
        return;
    }

    cout << " ┌" << string(5, '-') << "┬" << string(17, '-') << "┬" << string(32, '-') << "┬" << string(22, '-') << "┬" << string(17, '-') << "┐" << endl;
    cout << " │ " << left << setw(3) << "STT"
         << " │ " << left << setw(15) << "ISBN"
         << " │ " << left << setw(30) << "Ten Sach"
         << " │ " << left << setw(20) << "Tac Gia"
         << " │ " << left << setw(15) << "The Loai" << " │" << endl;
    cout << " ├" << string(5, '-') << "┼" << string(17, '-') << "┼" << string(32, '-') << "┼" << string(22, '-') << "┼" << string(17, '-') << "┤" << endl;

    for (int i = 0; i < ds.n; i++) {
        string tenSach = ds.nodes[i]->tenSach;
        if (tenSach.length() > 30) tenSach = tenSach.substr(0, 27) + "...";
        string tacGia = ds.nodes[i]->tacGia;
        if (tacGia.length() > 20) tacGia = tacGia.substr(0, 17) + "...";

        cout << " │ " << left << setw(3) << i + 1
             << " │ " << left << setw(15) << ds.nodes[i]->ISBN
             << " │ " << left << setw(30) << tenSach
             << " │ " << left << setw(20) << tacGia
             << " │ " << left << setw(15) << ds.nodes[i]->theLoai << " │" << endl;
    }
    cout << " └" << string(5, '-') << "┴" << string(17, '-') << "┴" << string(32, '-') << "┴" << string(22, '-') << "┴" << string(17, '-') << "┘" << endl;
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

void goiYViTriTiepTheo(DauSach* dauSach, char* goiY) {
    goiY[0] = '\0'; // Khoi tao chuoi rong
    if (dauSach == nullptr || dauSach->dsSach.pTail == nullptr) {
        strcpy(goiY, "HANG 1, NGAN 1");
        return;
    }

    const char* viTriCu = dauSach->dsSach.pTail->viTri;
    int hang = 0, ngan = 0;

    // Su dung sscanf de phan tich chuoi vi tri cu
    // Dinh dang "HANG %d, NGAN %d"
    if (sscanf(viTriCu, "HANG %d, NGAN %d", &hang, &ngan) == 2) {
        // Neu phan tich thanh cong, tang ngan len 1
        ngan++;
        // Neu ngan vuot qua 10, reset ngan ve 1 va tang hang len
        if (ngan > 10) {
            ngan = 1;
            hang++;
        }
        sprintf(goiY, "HANG %d, NGAN %d", hang, ngan);
    }
    // Neu khong phan tich duoc (dinh dang sai), khong dua ra goi y
}

void themCuonSach(DauSach* dauSach) { // Y THAY: THEM NHIEU DUOC
    if (dauSach == nullptr) return; 

    int soLuong;
    cout << "Nhap so luong cuon sach can them: ";
    if (!(cin >> soLuong) || soLuong <= 0 || soLuong > 100) {
        cout << "So luong khong hop le (1-100). Da huy.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Tao danh sach tam
    Sach** sachMoiArr = new Sach*[soLuong];
    int sttHienTai = laySTTMoi(dauSach);

    for (int i = 0; i < soLuong; i++) {
        sachMoiArr[i] = new Sach;
        snprintf(sachMoiArr[i]->maSach, sizeof(sachMoiArr[i]->maSach), "%s_%04d", dauSach->ISBN, sttHienTai + i);
        
        // Goi y vi tri
        if (i == 0) {
            goiYViTriTiepTheo(dauSach, sachMoiArr[i]->viTri);
        } else {
            // Goi y dua tren cuon truoc do trong danh sach tam
            int hang = 0, ngan = 0;
            if (sscanf(sachMoiArr[i-1]->viTri, "HANG %d, NGAN %d", &hang, &ngan) == 2) {
                ngan++;
                if (ngan > 10) { ngan = 1; hang++; }
                sprintf(sachMoiArr[i]->viTri, "HANG %d, NGAN %d", hang, ngan);
            } else { // Fallback
                 strcpy(sachMoiArr[i]->viTri, "HANG 1, NGAN 1");
            }
        }
    }

    // UI SUA VI TRI & XAC NHAN THEM NHIEU SACH
    int luaChon = 0;
    while (true) {
        system("cls");
        cout << "--- XAC NHAN & CHINH SUA DANH SACH SACH MOI ---\n";
        cout << "Dau sach: " << dauSach->tenSach << "\n\n";
        cout << "   ┌" << string(26, '-') << "┬" << string(22, '-') << "┐   " << endl;
        cout << "   │ " << left << setw(24) << "Ma Sach" << " │ " << left << setw(20) << "Vi Tri" << " │   " << endl;
        cout << "   ├" << string(26, '-') << "┼" << string(22, '-') << "┤   " << endl;

        for (int i = 0; i < soLuong; i++) {
            if (i == luaChon) { setColor(47); cout << ">> "; } 
            else { cout << "   "; }
            cout << "│ " << left << setw(24) << sachMoiArr[i]->maSach << " │ " << left << setw(20) << sachMoiArr[i]->viTri << " │   ";
            if (i == luaChon) resetColor();
            cout << endl;
        }
        cout << "   └" << string(26, '-') << "┴" << string(22, '-') << "┘   " << endl;
        setColor(36);
        cout << "\n   (Mui ten: Chon | Enter: Sua vi tri | S: Luu | ESC: Huy)\n";
        resetColor();

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) luaChon = max(0, luaChon - 1);
            else if (key == 80) luaChon = min(soLuong - 1, luaChon + 1);
        } else if (key == 13) { // Enter - Sua
            showCursor(true); 
            cout << "\n";
            char prompt[100];
            snprintf(prompt, sizeof(prompt), "Sua vi tri cho %s: ", sachMoiArr[luaChon]->maSach);
            char tempViTri[20];
            strcpy(tempViTri, sachMoiArr[luaChon]->viTri);
            nhapChuoiTuDo(prompt, sachMoiArr[luaChon]->viTri, sizeof(sachMoiArr[luaChon]->viTri), tempViTri);
            showCursor(false);
        } else if (key == 's' || key == 'S') { // S - Luu
            for (int i = 0; i < soLuong; i++) {
                sachMoiArr[i]->trangThai = 0;
                sachMoiArr[i]->pNext = nullptr;
                addTailSach(dauSach->dsSach, sachMoiArr[i]);
            }
            cout << "\nDa them thanh cong " << soLuong << " cuon sach!\n";
            delete[] sachMoiArr; // Giai phong mang con tro
            return;
        } else if (key == 27) { // ESC - Huy
            // Giai phong bo nho da cap phat cho cac sach tam
            for (int i = 0; i < soLuong; i++) {
                delete sachMoiArr[i];
            }
            delete[] sachMoiArr;
            cout << "\nDa huy them sach.\n";
            return;
        }
    }
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
    cout << " ┌" << string(5, '-') << "┬" << string(26, '-') << "┬" << string(22, '-') << "┬" << string(17, '-') << "┐" << endl;
    cout << " │ " << left << setw(3) << "STT" << " │ " << left << setw(24) << "Ma Sach" << " │ " << left << setw(20) << "Vi Tri" << " │ " << left << setw(15) << "Trang Thai" << " │" << endl;
    cout << " ├" << string(5, '-') << "┼" << string(26, '-') << "┼" << string(22, '-') << "┼" << string(17, '-') << "┤" << endl;

    int stt = 1; 
    while (temp != nullptr) {
        string trangThaiStr;
        switch (temp->trangThai) {
            case 0: trangThaiStr = "Cho muon duoc"; break;
            case 1: trangThaiStr = "Da muon"; break;
            case 2: trangThaiStr = "Thanh ly"; break;
            default: trangThaiStr = "Khong xac dinh"; break;
        }
        cout << " │ " << left << setw(3) << stt
             << " │ " << left << setw(24) << temp->maSach
             << " │ " << left << setw(20) << temp->viTri
             << " │ " << left << setw(15) << trangThaiStr << " │" << endl;
        stt++;
        temp = temp->pNext;
    }
    cout << " └" << string(5, '-') << "┴" << string(26, '-') << "┴" << string(22, '-') << "┴" << string(17, '-') << "┘" << endl;
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

void traCuuTheoTenNangCao(const ListDauSach &ds, const char* key) {
    bool found = false;
    
    if (ds.n == 0) {
        cout << "Danh sach dau sach rong!\n";
        return;
    }

    char keyLower[100]; 
    chuanHoaChuoiSearch(key, keyLower);

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

void traCuuTheoTacGia(const ListDauSach &ds, const char* key) {
    bool found = false;
    
    if (ds.n == 0) {
        cout << "Danh sach dau sach rong!\n";
        return;
    }

    char keyLower[100]; 
    chuanHoaChuoiSearch(key, keyLower);

    cout << "Cac dau sach co ten tac gia tuong doi giong voi: " << key << "\n";
    for (int i = 0; i < ds.n; i++) { 

        if (ds.nodes[i] == nullptr) continue; 

        if (strstr(ds.nodes[i]->tacGiaSearch, keyLower) != nullptr) {
            cout << "- " << ds.nodes[i]->tenSach << " (ISBN: " << ds.nodes[i]->ISBN << ")\n";
            cout << "  Tac gia: " << ds.nodes[i]->tacGia << ", The loai: " << ds.nodes[i]->theLoai << "\n";
            found = true;
        }
    }
    if (!found) {
        cout << "Khong tim thay dau sach co ten tac gia giong voi: " << key << "\n";
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

    // Giai phong mang con tro nodes
    if (ds.nodes != nullptr) {
        delete[] ds.nodes;
        ds.nodes = nullptr;
    }

    ds.n = 0;
    ds.capacity = 0;
}

int laySTTMoi(DauSach* dauSach)
{
    if (dauSach->dsSach.pTail == nullptr) return 1;
    const char* pos = strchr(dauSach->dsSach.pTail->maSach, '_'); 
    if (pos != nullptr) {
        return atoi(pos + 1) + 1; 
    }
    return dauSach->dsSach.tongSoSach + 1;
}

//
// Qucik sort theo the loai, trong do ten tang dan
//

int soSanhTheLoai(DauSach* a, DauSach* b) {
    int cmp = stricmp(a->theLoai, b->theLoai);
    if (cmp == 0)
        return stricmp(a->tenSach, b->tenSach);
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

    DauSach** arr = new DauSach*[ds.n];

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
    delete[] arr;
}

// ----------------------------------------------------------