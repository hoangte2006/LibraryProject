#pragma once
#include "Structs.h"

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

int partitionQH(DocGiaQuaHan arr[], int left, int right) {
    int pivot = arr[right].soNgayQuaHan;
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (arr[j].soNgayQuaHan > pivot) { // giảm dần
            i++;
            swapQH(arr[i], arr[j]);
        }
    }

    swapQH(arr[i + 1], arr[right]);
    return i + 1;
}

void quickSortQH(DocGiaQuaHan arr[], int left, int right) {
    if (left < right) {
        int pi = partitionQH(arr, left, right);
        quickSortQH(arr, left, pi - 1);
        quickSortQH(arr, pi + 1, right);
    }
}