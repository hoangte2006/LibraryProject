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

int soSanhQH(const DocGiaQuaHan &a, const DocGiaQuaHan &b) {
    return a.soNgayQuaHan - b.soNgayQuaHan;
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


void lietKeDocGiaQuaHan(TREE_DocGia root) {

}
