#include "Thongke.h"
#include "Docgia.h"
#include "Sach.h"
#include "MuonTra.h"
#include <iostream>

using namespace std;

// Ham nay tinh so ngay qua han lon nhat cua 1 doc gia.
int tinhQuaHanMax(const DocGia* docGia, const Ngay& ngayHienTai) {
    if (docGia == nullptr) return 0;

    int maxQuaHan = 0;
    MuonTra* temp = docGia->dsMuonTra.pHead;

    while (temp != nullptr) {
        if (temp->trangThai == 0) { 
            int soNgay = tinhSoNgay(temp->ngayMuon, ngayHienTai);
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

void lietKeDocGiaQuaHan(TREE_DocGia root) {
    DocGia* arr[MAX_DOCGIA];
    int n = 0;

    BSTtoArray(root, arr, n);

    DocGiaQuaHan listQH[MAX_DOCGIA];
    int countQH = 0;

    Ngay ngayHT = layNgayHienTai();

    for (int i = 0; i < n; i++) {
        int QH = tinhQuaHanMax(arr[i], ngayHT);

        if (QH > 0) {
            listQH[countQH].dg = arr[i];
            listQH[countQH].soNgayQuaHan = QH;
            countQH++;
        }
    }

    if (countQH == 0) {
        cout << "Khong co doc gia nao qua han!\n";
        return;
    }

    quickSortQH(listQH, 0, countQH - 1);

    cout << "\n--- DANH SACH DOC GIA QUA HAN ---\n";
    for (int i = 0; i < countQH; i++) {
        cout << "Ma the: " << listQH[i].dg->maThe
             << " | Ho ten: " << listQH[i].dg->ho << " " << listQH[i].dg->ten
             << " | So ngay qua han: " << listQH[i].soNgayQuaHan
             << endl;
    }
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

    DauSach* arr[MAX_DAUSACH];
    
    for (int i = 0; i < ds.n; i++) {
        arr[i] = ds.nodes[i];
    }

    buildHeap(arr, ds.n);

    cout << "--- TOP 10 SACH MUON NHIEU ---\n";

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
        cout << i + 1 << ". "
             << arr[0]->tenSach
             << " | Luot muon: " << arr[0]->soLuotMuon
             << endl;

        // loại root
        swap(arr[0], arr[n - 1]);
        n--;

        heapify(arr, n, 0);
    }
}

void capNhatSoLuotMuonCuaDauSach(TREE_DocGia root, ListDauSach &ds) {
    if (root == nullptr) return;

    capNhatSoLuotMuonCuaDauSach(root->pLeft, ds);

    MuonTra* mt = root->dsMuonTra.pHead;

    while (mt != nullptr) {
        DauSach* dau = nullptr;
        Sach* s = timSachTheoMa(ds, mt->maSach, dau);

        if (dau != nullptr) {
            dau->soLuotMuon++;
        }

        mt = mt->pNext;
    }

    capNhatSoLuotMuonCuaDauSach(root->pRight, ds);
}

void thongKeSoLuongTheoTheLoai(const ListDauSach &ds) {
    if (ds.n == 0) {
        cout << "Danh sach rong!\n";
        return;
    }

    DauSach* 

}