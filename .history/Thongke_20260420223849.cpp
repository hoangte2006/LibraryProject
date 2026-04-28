#include "Thongke.h"
#include "Docgia.h"
#include "Sach.h"
#include "MuonTra.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <cstring>

using namespace std;

// Ham nay tinh so ngay qua han lon nhat cua 1 doc gia.
int tinhQuaHanMax(const DocGia* docGia, const Ngay& ngayHienTai) {
    if (docGia == nullptr) return 0;

    time_t timeHienTai = chuyenNgaySangTimeT(ngayHienTai);
    if (timeHienTai == (time_t)-1) return 0; // timt_i -1 la gia tri loix, neu co loix thi tra ve 0 ngay qua han

    int maxQuaHan = 0;
    MuonTra* temp = docGia->dsMuonTra.pHead;

    while (temp != nullptr) {
        if (temp->trangThai == 0) { 
            time_t timeMuon = chuyenNgaySangTimeT(temp->ngayMuon);
            if (timeMuon == (time_t)-1) {
                temp = temp->pNext;
                continue;
            }

            int soNgay = (int)(difftime(timeHienTai, timeMuon) / (60 * 60 * 24));
            int quaHan = soNgay - HAN_MUON;
            if (quaHan > maxQuaHan) {
                maxQuaHan = quaHan;
            }
        }
        temp = temp->pNext;
    }

    return maxQuaHan;
}

void swapQH(DocGiaQuaHan &a, DocGiaQuaHan &b) {
    DocGiaQuaHan temp = a;
    a = b;
    b = temp;
}

int soSanhQH(DocGiaQuaHan &a, DocGiaQuaHan &b) {
    if (a.soNgayQuaHan > b.soNgayQuaHan) return 1;
    if (a.soNgayQuaHan < b.soNgayQuaHan) return -1;

    // tie-break
    if (a.dg->maThe > b.dg->maThe) return 1;
    if (a.dg->maThe < b.dg->maThe) return -1;

    return 0;
}

int medianOfThreeQH(DocGiaQuaHan arr[], int left, int right) {
    int mid = left + (right - left) / 2;

    if (soSanhQH(arr[left], arr[mid]) < 0)
        swapQH(arr[left], arr[mid]);

    if (soSanhQH(arr[left], arr[right]) < 0)
        swapQH(arr[left], arr[right]);

    if (soSanhQH(arr[mid], arr[right]) < 0)
        swapQH(arr[mid], arr[right]);

    return mid;
}

int partitionQH(DocGiaQuaHan arr[], int left, int right) {
    int pivotIndex = medianOfThreeQH(arr, left, right);

    swapQH(arr[pivotIndex], arr[right]);

    DocGiaQuaHan pivot = arr[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (soSanhQH(arr[j], pivot) > 0) { 
            i++;
            swapQH(arr[i], arr[j]);
        }
    }

    swapQH(arr[i + 1], arr[right]);
    return i + 1;
}

void quickSortQH(DocGiaQuaHan arr[], int left, int right) {
    if (left >= right) return;

    int pivot = partitionQH(arr, left, right);

    quickSortQH(arr, left, pivot - 1);
    quickSortQH(arr, pivot + 1, right);
}

// Ham de quy thu thap doc gia qua han vao mang listQH, dem so doc gia qua han va sap xep giam dan theo so ngay qua han.
static void thuThapDocGiaQuaHan(TREE_DocGia root, Ngay ngayHT, DocGiaQuaHan listQH[], int &countQH) {
    if (root == nullptr) return;

    thuThapDocGiaQuaHan(root->pLeft, ngayHT, listQH, countQH);

    int QH = tinhQuaHanMax(root, ngayHT);
    if (QH > 0) {
        listQH[countQH].dg = root;
        listQH[countQH].soNgayQuaHan = QH;
        countQH++;
    }

    thuThapDocGiaQuaHan(root->pRight, ngayHT, listQH, countQH);
}

void lietKeDocGiaQuaHan(QuanLyDocGia &ql) {
    if (ql.soLuongDocGia == 0) {
        cout << "Danh sach doc gia rong!\n";
        return;
    }

    DocGiaQuaHan* listQH = new DocGiaQuaHan[ql.soLuongDocGia];
    int countQH = 0;
    Ngay ngayHT = layNgayHienTai();

    thuThapDocGiaQuaHan(ql.root, ngayHT, listQH, countQH);

    if (countQH == 0) {
        cout << "Khong co doc gia nao qua han!\n";
        delete[] listQH; 
        return;
    }

    quickSortQH(listQH, 0, countQH - 1);

    cout << "\n--- DANH SACH DOC GIA QUA HAN ---\n";
    cout << " ┌" << string(12, '-') << "┬" << string(27, '-') << "┬" << string(20, '-') << "┐" << endl;
    cout << " │ " << left << setw(10) << "Ma The"
         << " │ " << left << setw(25) << "Ho Ten"
         << " │ " << left << setw(18) << "So Ngay Qua Han" << " │" << endl;
    cout << " ├" << string(12, '-') << "┼" << string(27, '-') << "┼" << string(20, '-') << "┤" << endl;

    for (int i = 0; i < countQH; i++) {
        string hoTen = string(listQH[i].dg->ho) + " " + string(listQH[i].dg->ten);
        if (hoTen.length() > 25) hoTen = hoTen.substr(0, 22) + "...";

        cout << " │ " << left << setw(10) << listQH[i].dg->maThe
             << " │ " << left << setw(25) << hoTen
             << " │ " << left << setw(18) << listQH[i].soNgayQuaHan << " │" << endl;
    }

    cout << " └" << string(12, '-') << "┴" << string(27, '-') << "┴" << string(20, '-') << "┘" << endl;
    
    delete[] listQH; 
}

bool luotMuonLonHon(DauSach* a, DauSach* b) {
    if (a->soLuotMuon != b->soLuotMuon)
        return a->soLuotMuon > b->soLuotMuon;

    return strcmp(a->tenSach, b->tenSach) < 0;
}

void heapify(DauSach* arr[], int n, int i) {
    int largest = i;
    int left = 2*i + 1;
    int right = 2*i + 2;

    if (left < n && luotMuonLonHon(arr[left], arr[largest]))
        largest = left;

    if (right < n && luotMuonLonHon(arr[right], arr[largest]))
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void buildHeap(DauSach* arr[], int n) {
    for (int i = n/2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

void inTop10SachMuonNhieu(const ListDauSach &ds) {
    if (ds.n == 0) {
        cout << "Danh sach rong!\n";
        return;
    }

    DauSach** arr = new DauSach*[ds.n];
    
    for (int i = 0; i < ds.n; i++) {
        arr[i] = ds.nodes[i];
    }

    buildHeap(arr, ds.n);

    cout << "--- TOP 10 SACH MUON NHIEU ---\n";
    cout << " ┌" << string(5, '-') << "┬" << string(42, '-') << "┬" << string(15, '-') << "┐" << endl;
    cout << " │ " << left << setw(3) << "Top"
         << " │ " << left << setw(40) << "Ten Sach"
         << " │ " << left << setw(13) << "Luot Muon" << " │" << endl;
    cout << " ├" << string(5, '-') << "┼" << string(42, '-') << "┼" << string(15, '-') << "┤" << endl;

    int n = ds.n;
    int limit;
    
    // cho an toan, lo it hon 10
    if (n < 10) {
        limit = n;
    } else {
        limit = 10;
    }

    for (int i = 0; i < limit; i++) {
        // root = max
        string tenSach = arr[0]->tenSach;
        if (tenSach.length() > 40) tenSach = tenSach.substr(0, 37) + "...";

        cout << " │ " << left << setw(3) << (i + 1)
             << " │ " << left << setw(40) << tenSach
             << " │ " << left << setw(13) << arr[0]->soLuotMuon << " │" << endl;

        // loại root
        swap(arr[0], arr[n - 1]);
        n--;

        heapify(arr, n, 0);
    }
    cout << " └" << string(5, '-') << "┴" << string(42, '-') << "┴" << string(15, '-') << "┘" << endl;
    delete[] arr;
}

// -------------------------------------------------------------------------------------

// Yeu cau Thầy: chi lay du lieu hien tai (khong tính lich su) -> Nhanh chong diem sach trang thai == 1
static void capNhatSoLuotMuonCuaDauSachDFS(TREE_DocGia root, ListDauSach &ds) {
    if (root == nullptr) return;

    capNhatSoLuotMuonCuaDauSachDFS(root->pLeft, ds);

    MuonTra* mt = root->dsMuonTra.pHead;

    while (mt != nullptr) {
        if (mt->trangThai == 0) { // chi dem sach dang muon nha
            char isbn[20];
            tachMaISBN(mt->maSach, isbn);
            DauSach* dau = timTheoISBN(ds, isbn);
            if (dau != nullptr) {
                dau->soLuotMuon++;
            }
        }
        mt = mt->pNext;
    }

    capNhatSoLuotMuonCuaDauSachDFS(root->pRight, ds);
}

// Ham chay chinh: Nap DauSach -> Quet DFS Cay Doc Gia
void capNhatSoLuotMuonCuaDauSach(TREE_DocGia root, ListDauSach &ds) {
    if (ds.n == 0) return;
    
    for (int i = 0; i < ds.n; i++) {
        if (ds.nodes[i] != nullptr) {
            ds.nodes[i]->soLuotMuon = 0;
        }
    }

    capNhatSoLuotMuonCuaDauSachDFS(root, ds);
    cout << "Da cap nhat so luot muon cua dau sach!\n";
}

// -------------------------------------------------------------------------------------

void thongKeSoLuongTheoTheLoai(const ListDauSach &ds) {
    if (ds.n == 0) {
        cout << "Danh sach rong!\n";
        return;
    }

    DauSach** arr = new DauSach*[ds.n];
    for (int i = 0; i < ds.n; i++) {
        arr[i] = ds.nodes[i];
    }

    hybridSortDauSach(arr, 0, ds.n - 1);

    cout << "\n===== THONG KE THE LOAI =====\n";
    cout << "--- THONG KE SO LUONG THEO THE LOAI ---\n";
    cout << " ┌" << string(5, '-') << "┬" << string(30, '-') << "┬" << string(12, '-') << "┐" << endl;
    cout << " │ " << left << setw(3) << "STT"
         << " │ " << left << setw(28) << "The Loai"
         << " │ " << left << setw(10) << "So Luong" << " │" << endl;
    cout << " ├" << string(5, '-') << "┼" << string(30, '-') << "┼" << string(12, '-') << "┤" << endl;

    int count = 1;
    int stt = 1;

    for (int i = 1; i < ds.n; i++) {
        if (strcmp(arr[i]->theLoai,arr[i-1]->theLoai) == 0) {
        if (strcmp(arr[i]->theLoai, arr[i-1]->theLoai) == 0) {
            count++;
        } else {
            cout << "The loai: " << arr[i-1]->theLoai << " | So luong: " << count << endl;
            string theLoai = arr[i-1]->theLoai;
            if (theLoai.length() > 28) theLoai = theLoai.substr(0, 25) + "...";
            cout << " │ " << left << setw(3) << stt++
                 << " │ " << left << setw(28) << theLoai
                 << " │ " << left << setw(10) << count << " │" << endl;
            count = 1;
        }
    }
    cout << "The loai: " << arr[ds.n-1]->theLoai << " | So luong: " << count << endl;
    string theLoai = arr[ds.n-1]->theLoai;
    if (theLoai.length() > 28) theLoai = theLoai.substr(0, 25) + "...";
    cout << " │ " << left << setw(3) << stt++
         << " │ " << left << setw(28) << theLoai
         << " │ " << left << setw(10) << count << " │" << endl;

    cout << " └" << string(5, '-') << "┴" << string(30, '-') << "┴" << string(12, '-') << "┘" << endl;

    delete[] arr;
}