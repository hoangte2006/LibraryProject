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

void lietKeDocGiaQuaHan(TREE_DocGia root) {
    DocGia* arr[MAX_DOCGIA];
    int n = 0;

    

}
