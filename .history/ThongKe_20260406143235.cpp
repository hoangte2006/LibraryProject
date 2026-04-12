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

    // sort 
    quickSortQH(listQH, 0, countQH - 1);

    cout << "\n--- DANH SACH DOC GIA QUA HAN ---\n";
    for (int i = 0; i < countQH; i++) {
        cout << "Ma the: " << listQH[i].dg->maThe
             << " | Ho ten: " << listQH[i].dg->ho << " " << listQH[i].dg->ten
             << " | So ngay qua han: " << listQH[i].soNgayQuaHan
             << endl;
    }
}

bool luotMuonLonHon(DocGiaQuaHan &a, DocGiaQuaHan &b) {
    if (a.dg->soLuotMuon > b.dg->soLuotMuon) return true;
    if (a.dg->soLuotMuon < b.dg->soLuotMuon) return false;

    // tie-break
    if (a.dg->maThe > b.dg->maThe) return true;
    if (a.dg->maThe < b.dg->maThe) return false;

    return false;
}

void heapify(DauSach* arr[], int n, int i) {
    int largest = i;
    int left = 2*i + 1;
    int right = 2*i + 2;

    if (left < n && lonHon(arr[left], arr[largest]))
        largest = left;

    if (right < n && lonHon(arr[right], arr[largest]))
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}